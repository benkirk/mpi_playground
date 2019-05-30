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

hname = './mc_header.contig'
dname = './mc_data.contig'


#############################################
def fill_buffer(step):

    from random import randint, seed
    seed(step)
    pow = randint (0, 2)
    return np.full(10**pow, step, np.float)



#############################################
def write():

    nruns = comm.bcast(47)
    lengths = np.zeros(nruns, dtype=np.int)

    head_offset = (1 + nruns)*isize

    status = MPI.Status()

    ########################################################
    # Master will write header: [ nruns [s0 s1 ... sNRUNS) ]
    if i_am_master:

        nsteps = nruns if not nruns%n_slaves else (nruns/n_slaves+1)*n_slaves

        print("Writing output from {} mock MC runs, nsteps={}".format(nruns,nsteps))

        data_offset = 0
        for step in range(0,nsteps):
            result   = comm.recv(source=MPI.ANY_SOURCE, tag=1, status=status)
            ready_rank = status.Get_source()

            # do something useful with the result.
            if result:
                rstep=result[0]
                if (rstep < nruns):
                    lengths[rstep] = result[1]
                    len = result[1]
                else:
                    len = 0
                print(result[2])
                data_offset += len*fsize

            instruct = (step, data_offset, "step_{:05d}".format(step))
            comm.send(instruct, dest=ready_rank, tag=10)

        # cleanup loop, send 'terminate' tag to each slave rank in
        # whatever order they become ready.
        requests = []
        for s in range(1,size):
            result = comm.recv(source=MPI.ANY_SOURCE, tag=1, status=status)
            # do something useful with the result.
            if result:
                rstep=result[0]
                if (rstep < nruns):
                    lengths[rstep] = result[1]
                    print(result[2])

            # send terminate tag, but no need to wait
            requests.append(
                comm.isend(None, dest=status.Get_source(), tag=100))

        # OK, messages sent, wait for all to complete
        MPI.Request.waitall(requests)

        # reopen the file on all ranks, master will write header: [ nruns [s0 s1 ... sNRUNS) ]
        fh = MPI.File.Open(self_comm, hname, MPI.MODE_WRONLY|MPI.MODE_CREATE)
        fh.Write(np.full(1, nruns, dtype=np.int))
        fh.Write(lengths)
        data_size = np.sum(lengths)*fsize
        print(lengths)
        print("Header size={}, header={}, data={}, header+data={}".format(fh.Get_size(),
                                                                          head_offset,
                                                                          data_size,
                                                                          head_offset+data_size))
        fh.Close()


    ########################################################################
    # slave comm
    else:
        fd = MPI.File.Open(slave_comm, dname, MPI.MODE_WRONLY|MPI.MODE_CREATE)

        result = None
        last_offset = 0
        while True:
            # signal Master we are ready for the next task.
            # receive instructions from Master
            instruct = comm.sendrecv(sendobj=result, dest=0, sendtag=1,
                                     source=0, recvtag=MPI.ANY_TAG, status=status)

            # choose proper action based on message tag
            if status.Get_tag() == 100: break

            step = instruct[0]
            if step < nruns:
                buf = fill_buffer(step)
            else:
                buf = np.empty(0,dtype=np.float)

            bsize = buf.size
            result = (step, bsize, "  -> step {:3d}, rank {:3d} created array of len {:.0e}".format(step,rank,bsize))

            sizes   = slave_comm.allgather(bsize)
            offsets = np.cumsum(np.insert(sizes, 0, 0))

            data_offset = last_offset + offsets[slave_rank]*fsize
            fd.Write_at_all(data_offset, buf)
            last_offset += np.sum(sizes)
            #fd.Write_ordered(buf)


        slave_comm.barrier()
        if not slave_rank:
            print("Data size={}".format(fd.Get_size()))

        fd.Close()



    return



if __name__ == "__main__":
    write()
