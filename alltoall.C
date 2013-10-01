#include "mpi.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include "random_in_range.h"
#include "process_cmdline.h"




int main (int argc, char **argv)
{
  int numprocs, procid;
  unsigned int nrep;

  MPI_Playground::process_command_line (argc, argv, nrep);

  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank (MPI_COMM_WORLD, &procid);


  std::set<unsigned int> dests;
  MPI_Playground::random_in_range (std::min(5,numprocs),
				   numprocs,
				   dests);


  for (unsigned int proc=0; proc<numprocs; proc++)
    {
      MPI_Barrier(MPI_COMM_WORLD);
      if (proc == procid)
	{
	  std::cout << "Rank " << std::setw(3) << procid << " sends to \t{ ";
	  for (std::set<unsigned int>::const_iterator it = dests.begin();
	       it!=dests.end(); ++it)
	    std::cout << std::setw(3) << *it << " ";
	  std::cout << " }\n";
	}
    }

  std::vector<unsigned int> sends(numprocs,0), recvs(numprocs,0);

  for (std::set<unsigned int>::const_iterator it = dests.begin();
       it!=dests.end(); ++it)
    sends[*it] = 1;


  //--------------------------------------------------------
  double elapsed_time=0;

  for (unsigned int cnt=0; cnt<nrep; cnt++)
    {

      const double starttime = MPI_Wtime();

      MPI_Alltoall (&sends[0], 1, MPI_UNSIGNED,
		    &recvs[0], 1, MPI_UNSIGNED,
		    MPI_COMM_WORLD);

      const double enddtime = MPI_Wtime();

      elapsed_time += (enddtime - starttime);
    }
  //--------------------------------------------------------


  unsigned int recv_cnt=0;

  for (unsigned int proc=0; proc<numprocs; proc++)
    if (recvs[proc]) recv_cnt++;

  for (unsigned int proc=0; proc<numprocs; proc++)
    {
      MPI_Barrier(MPI_COMM_WORLD);

      if (proc == procid)
	std::cout << "Rank " << procid << " is done, received " << recv_cnt << " values in "
		  << (elapsed_time/nrep)*1e6 << " (us).\n";
    }

  MPI_Finalize();

  return 0;
}
