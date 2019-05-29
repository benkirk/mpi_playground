#!/usr/bin/env python3

from mpi4py import MPI
import numpy as np
from time import sleep

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()
assert size > 1

isize = np.full(1, 0,  dtype=np.int).itemsize
fsize = np.full(1, 0., dtype=np.float).itemsize

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
    offsets = np.cumsum(np.insert(lengths, 0, 0))
    data_offset = offsets[rank]*fsize

    fh.Write_at_all(head_offset + data_offset, obuf)

    # rank 0 writes the header
    if not rank:
        fh.Seek(0)
        print(lengths)
        #print(offsets)
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
    head_offset = (1 + lengths.size)*isize
    offsets = np.cumsum(np.insert(lengths, 0, 0))
    data_offset = offsets[rank]*fsize

    if not rank:
        print(lengths)
        #print(offsets)

    ibuf = np.empty(lengths[rank], dtype=np.float)

    fh.Read_at_all(head_offset + data_offset, ibuf)
    print("{:3} {:.0e} {}".format(rank, ibuf.size, ibuf))

    fh.Close()




#############################################
if __name__ == "__main__":
    write()

    comm.Barrier()
    if not rank: print("-"*80)
    sleep(2)

    read()
