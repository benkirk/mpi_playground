#!/usr/bin/env python

from mpi4py import MPI
import numpy as np

amode = MPI.MODE_WRONLY|MPI.MODE_CREATE
comm = MPI.COMM_WORLD
rank = comm.Get_rank()
fh = MPI.File.Open(comm, "./datafile.contig", amode)

buffer = np.empty(100, dtype=np.int)
buffer[:] = rank



for cnt in range(0,comm.Get_size()):
    if cnt == rank:
        print("Writing {} bytes from rank {}".format(buffer.nbytes,rank))
    comm.Barrier()



offset = rank*buffer.nbytes
fh.Write_at_all(offset, buffer)

fh.Close()
