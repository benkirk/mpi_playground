#!/usr/bin/env python

from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

if rank == 0:
    data = np.empty(0,dtype=np.int)
    req = comm.isend(data, dest=1, tag=11)
    req.wait()

elif rank == 1:
    req = comm.irecv(source=0, tag=11)
    data = req.wait()
    print(data)
