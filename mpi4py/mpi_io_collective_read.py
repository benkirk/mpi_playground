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
def read():

    fd = MPI.File.Open(comm, fname, MPI.MODE_RDONLY)

    rbuf = np.empty(1, np.int)
    fd.Read_all(rbuf)
    nranks_read = rbuf[0]
    head_offset = (1 + nranks_read)*isize
    n_steps = 1 if nranks_read <= nranks else 1+nranks_read/nranks

    bsizes = np.empty(nranks_read, dtype=np.int)
    fd.Read_all(bsizes)
    assert bsizes.size == nranks_read

    if i_am_root:
        print("nranks={}, nranks_read={}, read steps={}\nbsizes={}\nData size={}".format(nranks,
                                                                                         nranks_read,
                                                                                         n_steps,
                                                                                         bsizes,
                                                                                         fd.Get_size()))

    bsizes  = np.pad(bsizes, (0,n_steps*nranks-nranks_read), 'constant', constant_values=(0,0))
    offsets = np.cumsum(np.insert(bsizes, 0, 0))

    for rstep in range(0,n_steps):
        assert rstep*nranks+rank < bsizes.size
        bsize = bsizes[rstep*nranks+rank]
        buf = np.empty(bsize, dtype=np.float)
        data_offset = offsets[rstep*nranks+rank]*fsize + head_offset
        fd.Read_at_all(data_offset, buf)
        if bsize: print("-> rank {:3d} created array of len {:.0e}, vals={}".format(rank,bsize,buf))

    fd.Close()

    return



if __name__ == "__main__":
    read()
