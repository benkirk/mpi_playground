#!/usr/bin/env python3

from mpi4py import MPI
import numpy as np

comm   = MPI.COMM_WORLD
rank   = comm.Get_rank()
nranks = comm.Get_size()

i_am_root =  True if (rank == 0) else False

# create a slave-only communicator for collective IO
int_size =   np.zeros(1, dtype=np.int).itemsize
float_size = np.zeros(1, dtype=np.float).itemsize

fname = './collective_data.contig'


#############################################
def read():

    tstart = MPI.Wtime()

    fd = MPI.File.Open(comm, fname, MPI.MODE_RDONLY)

    filesize = fd.Get_size()

    # read header, includes number of write ranks and buffer sizes
    rbuf = np.empty(1, np.int)
    fd.Read_all(rbuf)
    nranks_read = rbuf[0]
    head_offset = (1 + nranks_read)*int_size

    # number of 'steps' required to read nranks_read data on
    # nranks.  At least 1, but can be more when nranks_read > nranks
    n_steps = 1 if nranks_read <= nranks else 1+int(nranks_read/nranks)

    bsizes = np.empty(nranks_read, dtype=np.int)
    fd.Read_all(bsizes)

    if i_am_root:
        print("nranks={}, nranks_read={}, read steps={}\nbsizes={}\nFile size={}".format(nranks,
                                                                                         nranks_read,
                                                                                         n_steps,
                                                                                         bsizes,
                                                                                         fd.Get_size()))

    # in general nranks_read != nranks.  n_steps is an integer multiple of nranks.
    # we must pad the bsizes array with 0s to be the proper size
    bsizes  = np.pad(bsizes, (0,n_steps*nranks-nranks_read), 'constant', constant_values=(0,0))

    # rank per step buffer offsets, the partial sum of bsizes
    offsets = np.cumsum(np.insert(bsizes, 0, 0))

    for rstep in range(0,n_steps):
        step_rank = rstep*nranks+rank
        assert  step_rank < bsizes.size
        bsize = bsizes[step_rank]
        buf = np.empty(bsize, dtype=np.float)
        data_offset = offsets[step_rank]*float_size + head_offset

        # collective read, each rank at their own offset
        fd.Read_at_all(data_offset, buf)

        if bsize: print("-> rank {:3d} read {:.2e} length {:.3f} GB array, vals={}".format(rank,
                                                                                           bsize,
                                                                                           bsize*float_size/1.e9,
                                                                                           buf))

    elapsed = MPI.Wtime() - tstart
    fd.Close()

    # rank 0 writes summary
    if i_am_root:
        print("NRanks = {} read output from {} ranks".format(nranks, nranks_read))
        print("Read Rate = {:.3f} GB/sec (incl. allocation)".format(float(filesize)/1.e9/elapsed))

    return



if __name__ == "__main__":
    read()
