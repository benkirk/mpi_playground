#!/usr/bin/env python

from mpi4py import MPI
import numpy as np
from time import sleep


comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()
assert size > 1

isize = np.iinfo(np.int).bits/8
fsize = np.finfo(np.float).bits/8


fname = './mc_data.contig'



#############################################
def fill_buffer(step=0):

    from random import randint, seed
    from os import urandom
    seed(step)
    len = randint (5, 5)
    return np.full(len, step, np.float)



#############################################
def write():

    amode = MPI.MODE_WRONLY|MPI.MODE_CREATE
    fh = MPI.File.Open(comm, fname, amode)

    nruns = comm.bcast(20)
    lengths = np.zeros(nruns, dtype=np.int)

    head_offset = (1 + nruns)*isize

    status = MPI.Status()

    ########################################################
    # Master will write header: [ nruns [s0 s1 ... sNRUNS) ]
    if not rank:
        print("Writing output from {} mock MC runs".format(nruns))
        wbuf = np.full(1, nruns, dtype=np.int)
        fh.Write(wbuf)

        for step in range(0,nruns):
            result = comm.recv(source=MPI.ANY_SOURCE, tag=1, status=status)
            ready_rank = status.Get_source()

            # do something useful with the result.
            if result:
                rstep=result[0]
                lengths[rstep] = result[1]
                print(result[2])

            instruct = (step, "step_{:05d}".format(step))

            comm.send(instruct, dest=ready_rank, tag=10)
            #print("Running step {} on rank {}".format(step,ready_rank))

        # cleanup loop, send 'terminate' tag to each slave rank in
        # whatever order they become ready.
        # Don't forget to catch their final 'result'
        #print("  --> Finished dispatch, Terminating ranks")
        requests = []
        for s in range(1,size):
            result = comm.recv(source=MPI.ANY_SOURCE, tag=1, status=status)
            # do something useful with the result.
            if result:
                rstep=result[0]
                lengths[rstep] = result[1]
                print(result[2])

            # send terminate tag, but no need to wait
            requests.append(
                comm.isend(None, dest=status.Get_source(), tag=100))

        # OK, messages sent, wait for all to complete
        MPI.Request.waitall(requests)
        fh.Write(lengths)


    ########################################################################
    # slave comm
    else:
        result = None
        while True:
            # signal Master we are ready for the next task. We can do this
            # asynchronously, without a request, because we can infer completion
            # with the subsequent recv.
            comm.isend(result, dest=0, tag=1)

            # receive instructions from Master
            instruct = comm.recv(source=0, tag=MPI.ANY_TAG, status=status)

            # choose proper action based on message tag
            if status.Get_tag() == 100: break

            step = instruct[0]
            buf = fill_buffer(step)
            bsize = buf.size
            result = (step, bsize, "  -> rank {:3d}, step {:3d} created array of len {}".format(rank,step,bsize))

            body_offset = (10*step)*fsize
            fh.Write_at(head_offset + body_offset, buf)


    fh.Close();
    return



#############################################
def read():

    amode = MPI.MODE_RDONLY
    fh = MPI.File.Open(comm, fname, amode)

    nruns = None
    lengths = None

    # collective read at header
    rbuf = np.empty(1, np.int)
    fh.Read_all(rbuf)
    nruns = rbuf[0]
    lengths = np.empty(nruns, dtype=np.int)
    fh.Read_all(lengths)

    if not rank:
        print("Reading data from {} mock MC runs".format(nruns))
        print(lengths)

    head_offset = (1 + nruns)*isize

    status = MPI.Status()

    if not rank:
        for step in range(0,nruns):
            result = comm.recv(source=MPI.ANY_SOURCE, tag=1, status=status)
            ready_rank = status.Get_source()

            instruct = (step, lengths[step], "step_{:05d}".format(step))
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
        result = None
        while True:
            comm.isend(result, dest=0, tag=1)

            instruct = comm.recv(source=0, tag=MPI.ANY_TAG, status=status)

            # choose proper action based on message tag
            if status.Get_tag() == 100: break

            step = instruct[0]
            len  = instruct[1]
            rbuf = np.empty(len, dtype=np.float)
            body_offset = (10*step)*fsize
            fh.Read_at(head_offset + body_offset, rbuf)
            print ("step {}, rank, {}, vals={}".format(step, rank, rbuf))

            result = (step, len, "  -> rank {}, step {} read array of len {}".format(rank,step,len))

    fh.Close();
    return



if __name__ == "__main__":
    write()

    comm.Barrier()
    if not rank: print("-"*80)
    sleep(2)

    read()
