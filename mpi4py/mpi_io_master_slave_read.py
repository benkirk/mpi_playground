#!/usr/bin/env python

from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()
assert size > 1

i_am_master =  True if (rank == 0) else False

self_comm = MPI.COMM_SELF

# create a slave-only communicator for collective IO
slave_comm = comm.Create_group(comm.group.Excl([0]))
slave_rank = None     if i_am_master else slave_comm.Get_rank()
n_slaves   = (size-1) if i_am_master else slave_comm.Get_size()

isize = np.full(1, 0,  dtype=np.int).itemsize
fsize = np.full(1, 0., dtype=np.float).itemsize

#fname = './mc_data.contig'
hname = './mc_header.contig'
dname = './mc_data.contig'



#############################################
def read():

    nruns = None
    lengths = None

    # master read header
    if i_am_master:
        fh = MPI.File.Open(self_comm, hname, MPI.MODE_RDONLY)
        rbuf = np.empty(1, np.int)
        fh.Read(rbuf)
        nruns = rbuf[0]
        lengths = np.empty(nruns, dtype=np.int)
        fh.Read(lengths)
        fh.Close()


    nruns   = comm.bcast(nruns)
    lengths = comm.bcast(lengths)

    head_offset = (1 + nruns)*isize

    offsets = np.cumsum(np.insert(lengths, 0, 0))

    status = MPI.Status()

    ########################################################################
    # master comm
    if i_am_master:
        nsteps = nruns if not nruns%n_slaves else (nruns/n_slaves+1)*n_slaves
        print("Reading data from {} mock MC runs, nsteps={}".format(nruns,nsteps))
        print(lengths)

        for step in range(0,nsteps):
            if step<nruns:
                len = lengths[step]
            else:
                len = 0
            instruct = (step, len, "step_{:05d}".format(step))
            result = comm.recv(source=MPI.ANY_SOURCE, tag=1, status=status)
            ready_rank = status.Get_source()

            # do something useful with the result.
            if result:
                rstep=result[0]
                print(result[2])

            comm.send(instruct, dest=ready_rank, tag=10)

        # cleanup loop, send 'terminate' tag to each slave rank in
        # whatever order they become ready.
        requests = []
        for s in range(1,size):
            result = comm.recv(source=MPI.ANY_SOURCE, tag=1, status=status)

            # send terminate tag, but no need to wait
            requests.append(
                comm.isend(None, dest=status.Get_source(), tag=100))

        # OK, messages sent, wait for all to complete
        MPI.Request.waitall(requests)


    ########################################################################
    # slave comm
    else:
        fd = MPI.File.Open(slave_comm, dname, MPI.MODE_RDONLY)

        result = None
        while True:
            # signal Master we are ready for the next task.
            # receive instructions from Master
            instruct = comm.sendrecv(sendobj=result, dest=0, sendtag=1,
                                     source=0, recvtag=MPI.ANY_TAG, status=status)

            # choose proper action based on message tag
            if status.Get_tag() == 100: break

            step = instruct[0]
            len  = instruct[1]
            rbuf = np.empty(len, dtype=np.float)
            data_offset = offsets[step]*fsize
            #data_offset = step*len*fsize
            fd.Read_at_all(data_offset, rbuf)
            #fd.Read_ordered(rbuf)
            print ("step {:3d}, rank {:3d}, read vals={}".format(step, rank, rbuf))

            result = (step, len, "  -> step {:3d}, rank {:3d} read array of len {}".format(step,rank,len))

        fd.Close();

    return



if __name__ == "__main__":
    read()
