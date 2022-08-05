#!/usr/bin/env python3

from mpi4py import MPI
import numpy as np

comm   = MPI.COMM_WORLD
rank   = comm.Get_rank()
nranks = comm.Get_size()

i_am_root =  True if (rank == 0) else False

int_size   = np.zeros(1, dtype=np.int64).itemsize
float_size = np.zeros(1, dtype=np.float64).itemsize

fname = './collective_data.contig'


#############################################
def fill_buffer():

    from random import randint, seed
    seed(rank)
    pow = randint(8, 8)
    lead = float(randint(100,130))/100.
    size = int(lead*10**pow)
    buf = np.zeros(size, np.float64)
    buf[:] = rank
    return buf



#############################################
def write():

    lengths = np.zeros(nranks, dtype=np.int64)

    head_offset = (1 + nranks)*int_size

    buf = fill_buffer()
    print("-> rank {:3d} created {:.2e} length {:.3f} GB array".format(rank,
                                                                       buf.size,
                                                                       buf.size*float_size/1.e9))

    tstart = MPI.Wtime()

    fd = MPI.File.Open(comm, fname, MPI.MODE_WRONLY|MPI.MODE_CREATE)

    # get buffer sizes per rank. total data size, and rank offsets
    bsizes      = np.array(comm.allgather(buf.size))
    data_size   = np.sum(bsizes)*float_size
    offsets     = np.cumsum(np.insert(bsizes, 0, 0))
    data_offset = offsets[rank]*float_size + head_offset

    fd.Preallocate(head_offset+data_size)

    # rank 0 writes header
    if i_am_root:
        rbuf = np.zeros(1, dtype=np.int64); rbuf[:] = nranks
        fd.Write(rbuf)
        fd.Write(bsizes)

    # collective write, each rank at their own offset
    print("-> rank {:3d} writing array of len {:.2e}".format(rank,buf.size))
    fd.Write_at_all(data_offset, buf)

    filesize = fd.Get_size()

    fd.Close()

    elapsed = MPI.Wtime() - tstart

    # rank 0 writes summary
    if i_am_root:
        print("NRanks = {}".format(nranks))
        print("  File size = {} bytes, {:.3f} GB".format(filesize,filesize/1.e9))
        print("  head_offset = {}".format(head_offset))
        print("  head_offset+data_size = {}".format(head_offset+data_size))
        print("  elapsed = {:.3f} sec".format(elapsed))
        print("Write Rate = {:.3f} GB/sec (excl. allocation)".format(float(filesize)/1.e9/elapsed))

    return



if __name__ == "__main__":
    write()
