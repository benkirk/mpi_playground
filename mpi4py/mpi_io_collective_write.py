#!/usr/bin/env python

from mpi4py import MPI
import numpy as np

comm   = MPI.COMM_WORLD
rank   = comm.Get_rank()
nranks = comm.Get_size()

i_am_root =  True if (rank == 0) else False

self_comm = MPI.COMM_SELF

# create a slave-only communicator for collective IO
isize = np.full(1, 0,  dtype=np.int).itemsize
fsize = np.full(1, 0., dtype=np.float).itemsize

fname = './collective_data.contig'


#############################################
def fill_buffer():

    from random import randint, seed
    seed(rank)
    pow = randint (4, 6)
    return np.full(10**pow, rank, np.float)



#############################################
def write():

    lengths = np.zeros(nranks, dtype=np.int)

    head_offset = (1 + nranks)*isize

    status = MPI.Status()

    fd = MPI.File.Open(comm, fname, MPI.MODE_WRONLY|MPI.MODE_CREATE)

    buf = fill_buffer()
    bsize = buf.size
    print("-> rank {:3d} created array of len {:.0e}".format(rank,bsize))

    bsizes      = np.array(comm.allgather(bsize))
    data_size   = np.sum(bsizes)*fsize
    offsets     = np.cumsum(np.insert(bsizes, 0, 0))
    data_offset = offsets[rank]*fsize + head_offset

    fd.Preallocate(head_offset+data_size)

    fd.Write_at_all(data_offset, buf)

    # rank 0 writes header
    if i_am_root:
        fd.Seek(0)
        fd.Write(np.full(1, nranks, dtype=np.int))
        fd.Write(bsizes)
        print("Data size={}, head_offset={}, head_offset+data_size={}".format(fd.Get_size(),
                                                                              head_offset,
                                                                              head_offset+data_size))

    fd.Close()

    return



if __name__ == "__main__":
    write()
