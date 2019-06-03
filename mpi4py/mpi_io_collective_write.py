#!/usr/bin/env python3

from mpi4py import MPI
import numpy as np

comm   = MPI.COMM_WORLD
rank   = comm.Get_rank()
nranks = comm.Get_size()

i_am_root =  True if (rank == 0) else False

# create a slave-only communicator for collective IO
int_size   = np.full(1, 0,  dtype=np.int).itemsize
float_size = np.full(1, 0., dtype=np.float).itemsize

fname = './collective_data.contig'


#############################################
def fill_buffer():

    from random import randint, seed
    seed(rank)
    pow = randint (4, 6)
    lead = float(randint(10,14))/10.
    size = int(lead*10**pow)
    return np.full(size, rank, np.float)



#############################################
def write():

    lengths = np.zeros(nranks, dtype=np.int)

    head_offset = (1 + nranks)*int_size

    fd = MPI.File.Open(comm, fname, MPI.MODE_WRONLY|MPI.MODE_CREATE)

    buf = fill_buffer()
    print("-> rank {:3d} created array of len {:.1e}".format(rank,buf.size))

    # get buffer sizes per rank. total data size, and rank offsets
    bsizes      = np.array(comm.allgather(buf.size))
    data_size   = np.sum(bsizes)*float_size
    offsets     = np.cumsum(np.insert(bsizes, 0, 0))
    data_offset = offsets[rank]*float_size + head_offset

    fd.Preallocate(head_offset+data_size)

    # rank 0 writes header
    if i_am_root:
        fd.Write(np.full(1, nranks, dtype=np.int))
        fd.Write(bsizes)
        print("File size={}, head_offset={}, head_offset+data_size={}".format(fd.Get_size(),
                                                                              head_offset,
                                                                              head_offset+data_size))
    # collective write, each rank at their own offset
    fd.Write_at_all(data_offset, buf)

    fd.Close()
    return



if __name__ == "__main__":
    write()
