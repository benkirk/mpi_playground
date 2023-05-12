#include "mpi.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <unistd.h>



int main (int argc, char **argv)
{
  int nranks, myrank, mylocalrank;

  char hn[64], hns[4096*64];

  const std::size_t
    bufcnt =  1000*1000*8,
    bufsize = bufcnt*(sizeof(unsigned int)),
    nrep = 10;

  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &nranks);
  MPI_Comm_rank (MPI_COMM_WORLD, &myrank);

  {
    MPI_Comm shmcomm;
    MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0,
                        MPI_INFO_NULL, &shmcomm);
    MPI_Comm_rank(shmcomm, &mylocalrank);

    gethostname(hn, sizeof(hn) / sizeof(char));

    std::ostringstream oss;
    oss << myrank << ":" << hn << ":" << mylocalrank;
    std::string str = oss.str();

    MPI_Allgather(&str[0],  64, MPI_CHAR,
                  hns, 64, MPI_CHAR,
                  MPI_COMM_WORLD);
  }

  if (0 == myrank)
    {
      time_t now = time(0);

      std::cout << "# --> BEGIN execution\n"
                << "# " << ctime(&now)
                << "# " << argv[0] << "\n"
                << "# nranks = " << nranks << "\n";

      for (unsigned int idx=0, cnt=0; cnt<nranks; cnt++, idx+=64)
        {
          std::cout << std::string(&hns[idx]);

          (cnt != (nranks-1)) ? std::cout << ", " : std::cout << "\n";
        }

      std::cout << "# bufcnt  = " << bufcnt  << " elements\n"
                << "# bufsize = " << bufsize << " (bytes)\n"
                << "# myrank, procup, procdn=\n";
    }

  std::vector<unsigned int> sbuf, rbuf(bufcnt,0);
  sbuf.reserve(bufcnt);
  for (unsigned int i=myrank, j=0; j<bufcnt; i++, j++)
    sbuf.push_back(i);

  std::vector<double>
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

          MPI_Irecv(&rbuf[0],  bufcnt, MPI_UNSIGNED, procdn, /* tag = */ 1*nranks + step, MPI_COMM_WORLD, &rreq);

          MPI_Barrier(MPI_COMM_WORLD);

          // overlap Irecv / Ssend
          {
            const double starttime = MPI_Wtime();

            MPI_Ssend(&sbuf[0], bufcnt, MPI_UNSIGNED, procup, /* tag = */ 1*nranks + step, MPI_COMM_WORLD);

            snd[procup] += (MPI_Wtime() - starttime);

            MPI_Wait(&rreq, MPI_STATUS_IGNORE);

            // check correctness
            for (unsigned int i=procdn, j=0; j<bufcnt; i++, j++)
              assert(rbuf[j] == i);
          }
        } // end avg loop over nrep

      // compute average over nrep
      snd[procup] /= static_cast<double>(nrep);

      if (0 == myrank)
        std::cout << "# "
                  << std::setw(5) << myrank
                  << ", " << std::setw(5) << procup
                  << ", " << std::setw(5) << procdn
                  << " / " << std::setw(12) << snd[procup] << "\t (sec)"
                  << " / " << std::setw(12) << static_cast<double>(bufsize) / snd[procup] << " (b/sec)"
                  << std::endl;
    }

  {
    const double starttime = MPI_Wtime();

    MPI_Allgather(&snd[0],    nranks, MPI_DOUBLE,
                  &allsnd[0], nranks, MPI_DOUBLE,
                  MPI_COMM_WORLD);

    const double elapsed = (MPI_Wtime() - starttime);

    if (0 == myrank)
      {
        std::cout << "# MPI_Allgather() on "
                  << nranks << " ranks required " << elapsed << " (sec)\n";

        for (unsigned int i=0; i<nranks; ++i)
          {
            std::cout << std::string(&hns[i*64]) << ", ";
            for (unsigned int j=0; j<nranks; ++j)
              {
                std::cout << std::setprecision(8) << allsnd[j*nranks + i];
                (j != (nranks-1)) ? std::cout << ", " : std::cout << "\n";
              }
          }

        std::cout << "# --> END execution" << std::endl;
      }
  }

  MPI_Finalize();


  return 0;
}
