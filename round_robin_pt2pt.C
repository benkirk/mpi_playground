#include "mpi.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <unistd.h>



int main (int argc, char **argv)
{
  int nranks, myrank;
  char hn[64], hns[4096*64];

  gethostname(hn, sizeof(hn) / sizeof(char));
  //std::cout << hn << std::endl;

  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &nranks);
  MPI_Comm_rank (MPI_COMM_WORLD, &myrank);

  MPI_Allgather(hn,  64, MPI_CHAR,
                hns, 64, MPI_CHAR,
                MPI_COMM_WORLD);

  if (0 == myrank)
    for (unsigned int idx=0, cnt=0; cnt<nranks; cnt++, idx+=64)
      {
        for (unsigned int c=0; c<64 && hns[idx+c]!='\0'; c++)
          std::cout << hns[idx+c];
        std::cout << std::endl;
      }

  const std::size_t
    bufcnt =  1000*1000*8,
    bufsize = bufcnt*(sizeof(unsigned int)),
    nrep = 10;

  if (0 == myrank)
    std::cout << "bufcnt  = " << bufcnt << std::endl
              << "bufsize = " << bufsize << std::endl;

  std::vector<unsigned int> sbuf, rbuf(bufcnt,0);
  sbuf.reserve(bufcnt);
  for (unsigned int i=myrank, j=0; j<bufcnt; i++, j++)
    sbuf.push_back(i);

  std::vector<double>
    //up(nranks,0.), allup(nranks*nranks,0.),
    //dn(nranks,0.), alldn(nranks*nranks,0.),
    snd(nranks,0.), allsnd(nranks*nranks,0.);

  for (unsigned int rc=0; rc<nranks; rc++)
    {
      const int
        procup = (myrank + rc) % nranks,
        procdn = (nranks + myrank - rc) % nranks;

      assert (procup >= 0 && procup < nranks);
      assert (procdn >= 0 && procdn < nranks);

      for (std::size_t step=0; step<nrep; ++step)
        {
          MPI_Request rreq;

          MPI_Irecv(&rbuf[0],  bufcnt, MPI_UNSIGNED, procdn, /* tag = */ 200, MPI_COMM_WORLD, &rreq);

          MPI_Barrier(MPI_COMM_WORLD);

          // // procup
          // {
          //   const double starttime = MPI_Wtime();

          //   MPI_Sendrecv (&sbuf[0], bufcnt, MPI_UNSIGNED, procup, /* sendtag = */ 99,
          //                 &rbuf[0], bufcnt, MPI_UNSIGNED, procdn, /* recvtag = */ 99,
          //                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

          //   dn[procdn] += (MPI_Wtime() - starttime);

          //   // check correctness
          //   for (unsigned int i=procdn, j=0; j<bufcnt; i++, j++)
          //     assert(rbuf[j] == i);
          // }

          // // procdn
          // {
          //   const double starttime = MPI_Wtime();

          //   MPI_Sendrecv (&sbuf[0], bufcnt, MPI_UNSIGNED, procdn, /* sendtag = */ 100,
          //                 &rbuf[0], bufcnt, MPI_UNSIGNED, procup, /* recvtag = */ 100,
          //                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

          //   up[procup] += (MPI_Wtime() - starttime);

          //   // check correctness
          //   for (unsigned int i=procup, j=0; j<bufcnt; i++, j++)
          //     assert(rbuf[j] == i);
          // }

          // overlap Irecv / Ssend
          {
            const double starttime = MPI_Wtime();

            MPI_Ssend(&sbuf[0], bufcnt, MPI_UNSIGNED, procup, /* tag = */ 200, MPI_COMM_WORLD);

            snd[procup] += (MPI_Wtime() - starttime);

            MPI_Wait(&rreq, MPI_STATUS_IGNORE);

            // check correctness
            for (unsigned int i=procdn, j=0; j<bufcnt; i++, j++)
              assert(rbuf[j] == i);
          }
        } // end avg loop over nrep

      //dn[procdn] /= static_cast<double>(nrep);
      //up[procup] /= static_cast<double>(nrep);
      snd[procup] /= static_cast<double>(nrep);

      // if (0 == myrank)
      //   std::cout << "myrank, procup, procdn="
      //             << myrank
      //             << ", " << procup
      //             << ", " << procdn
      //             << " / " <<  dn[procdn] << " " << up[procup] << " " << snd[procup] << " (sec)"
      //             << " / " << static_cast<double>(bufsize) / dn[procdn] << " " << static_cast<double>(bufsize) / up[procup] << " " << static_cast<double>(bufsize) / snd[procup] << " (b/sec)"
      //             << std::endl;

      if (0 == myrank)
        std::cout << "myrank, procup, procdn="
                  << std::setw(3) << myrank
                  << ", " << std::setw(3) << procup
                  << ", " << std::setw(3) << procdn
                  << " / " << std::setw(12) << snd[procup] << "\t (sec)"
                  << " / " << std::setw(12) << static_cast<double>(bufsize) / snd[procup] << " (b/sec)"
                  << std::endl;
    }

  // MPI_Allgather(&up[0],    nranks, MPI_DOUBLE,
  //               &allup[0], nranks, MPI_DOUBLE,
  //               MPI_COMM_WORLD);

  // MPI_Allgather(&dn[0],    nranks, MPI_DOUBLE,
  //               &alldn[0], nranks, MPI_DOUBLE,
  //               MPI_COMM_WORLD);

  MPI_Allgather(&snd[0],    nranks, MPI_DOUBLE,
                &allsnd[0], nranks, MPI_DOUBLE,
                MPI_COMM_WORLD);

  if (0 == myrank)
    {
      // for (unsigned int i=0; i<nranks; ++i)
      //   {
      //     for (unsigned int j=0; j<nranks; ++j)
      //       std::cout << alldn[j*nranks + i] << " ";

      //     std::cout << std::endl;
      //   }
      // std::cout << std::endl;
      for (unsigned int i=0; i<nranks; ++i)
        {
          for (unsigned int j=0; j<nranks; ++j)
            std::cout << allsnd[j*nranks + i] << " ";

          std::cout << std::endl;
        }
    }

  MPI_Finalize();

  return 0;
}
