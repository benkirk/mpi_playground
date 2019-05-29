#!/usr/bin/env python3

from mpi4py import MPI
import numpy as np
from time import sleep

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()
assert size > 1

isize = np.iinfo(np.int).bits/8
fsize = np.finfo(np.float).bits/8


fname = './datafile.contig'

#############################################
def fill_buffer(step=0):

    from random import randint, seed
    seed(step)
    pow = randint (4, 6)
    return np.full(10**pow, step, np.float)




#############################################
def write():

    amode = MPI.MODE_WRONLY|MPI.MODE_CREATE
    fh = MPI.File.Open(comm, fname, amode)

    head_offset = (1 + size)*isize

    obuf = fill_buffer(rank)
    lengths = np.array(comm.allgather(obuf.size))

    fh.Seek_shared(head_offset)
    fh.Write_ordered(obuf)

    # rank 0 writes the header
    if not rank:
        fh.Seek(0)
        print(lengths)
        fh.Write(np.full(1, size, dtype=np.int))
        fh.Write(lengths)
        print("File size={}".format(fh.Get_size()))

    fh.Close()




#############################################
def read():

    amode = MPI.MODE_RDONLY #|MPI.MODE_DELETE_ON_CLOSE
    fh = MPI.File.Open(comm, fname, amode)

    if not rank: print("File size={}".format(fh.Get_size()))

    lengths = None

    insize = np.empty(1, dtype=np.int)
    fh.Read_all(insize)
    lengths = np.empty(insize[0], dtype=np.int)
    fh.Read_all(lengths)
    if not rank: print(lengths)

    head_offset = (1 + lengths.size)*isize
    ibuf = np.empty(lengths[rank], dtype=np.float)

    fh.Seek_shared(head_offset)
    fh.Read_ordered(ibuf)
    print("{:3} {:.0e} {}".format(rank, ibuf.size, ibuf))

    fh.Close()




#############################################
if __name__ == "__main__":
    write()

    comm.Barrier()
    if not rank: print("-"*80)
    sleep(2)

    read()
