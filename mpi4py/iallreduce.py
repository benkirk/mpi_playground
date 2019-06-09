#!/usr/bin/env python

import numpy as np
from random import randint, seed
from time import sleep
from mpi4py import MPI

comm   = MPI.COMM_WORLD
rank   = comm.Get_rank()
nranks = comm.Get_size()

send = np.full(1, 1, dtype=np.int)
recv = np.empty_like(send)

allreduce = comm.Iallreduce(send, recv)

sleep(randint(1,3))

allreduce.wait()

print("rank {}, result {}".format(rank,recv))
