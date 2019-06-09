#!/usr/bin/env python

from mpi4py import MPI
import numpy as np
from random import randint, seed
from time import sleep
import sys

comm   = MPI.COMM_WORLD
rank   = comm.Get_rank()
nranks = comm.Get_size()

i_am_root =  True if (rank == 0) else False


np.set_printoptions(threshold=7)

#############################################
def random_rank_in_range (nentries=3, nmax=nranks):

    if 'initialized' not in random_rank_in_range.__dict__:
        seed(rank)
        random_rank_in_range.initialized = True

    vals=[]

    while len(vals) != nentries:
        vals.append(randint(0,nmax) % nranks)

    return np.array(vals)



#############################################
def nonblocking_consensus():

    seed(rank+nranks)

    srcs=set()
    dests = random_rank_in_range(nentries = randint(0, 2*nranks))

    # We don't need this long term.
    send_map = np.zeros(nranks, dtype=np.int)
    for dest in dests: send_map[dest] += 1
    send_map = comm.allreduce(send_map, MPI.SUM)

    # print start message
    for p in range(0,nranks):
        comm.Barrier()
        sys.stdout.flush()
        if p == rank:
            if i_am_root:
                print("-"*80)
                print(send_map)
                print("-"*80)
            print("-s-> rank {:3d} sending {:3d} messages to ranks {}".format(rank, len(dests), dests))


    sendval = np.full(10000, 12345, dtype=np.int)
    recvval = np.zeros_like(sendval)
    recv_cnt = 0
    recv_loop = 0
    requests = []
    barrier = None
    done = False

    tstart = MPI.Wtime()

    # start sends
    for dest in dests:
        r = comm.Issend(sendval, dest=dest, tag=100)
        requests.append(r)

    status = MPI.Status()

    # enter recv loop
    while not done:

        recv_loop += 1

        # message waiting?
        if comm.Iprobe(source=MPI.ANY_SOURCE, tag=100, status=status):
            source = status.Get_source()
            srcs.add(source)
            recvval[:] = 0
            comm.Recv(recvval, source=source, tag=100)
            assert np.array_equal(recvval, sendval)
            assert status.Get_tag() == 100
            recv_cnt += 1

        # barrier not active
        if not barrier:
            # activate barrier when all my sends complete
            all_sent, msg = MPI.Request.testall(requests)
            if all_sent:  barrier = comm.Ibarrier()

        # otherwise see if barrier completed
        else:
            done, msg = MPI.Request.test(barrier)

    # complete
    tstop = MPI.Wtime()

    assert recv_cnt == send_map[rank]

    max_steps = comm.allreduce(recv_loop, MPI.MAX)

    # print end message
    for p in range(0,nranks):
        comm.Barrier()
        sys.stdout.flush()
        if p == rank:
            if i_am_root:
                print("-"*80)
                print("Completed in {:4f} seconds on {} ranks in max {} steps".format(tstop-tstart,
                                                                                      nranks,
                                                                                      max_steps))
                print("-"*80)
            print("-r-> rank {:3d} received {:3d} messages from {:3d} ranks in {:5d} steps from {}".format(rank,
                                                                                                           recv_cnt,
                                                                                                           len(srcs),
                                                                                                           recv_loop,
                                                                                                           np.array(list(srcs))))

    return



if __name__ == "__main__":
    nonblocking_consensus()
