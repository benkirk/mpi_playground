#include "mpi.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include "random_in_range.h"

int main (int argc, char **argv)
{
  int numprocs, procid;
  // MPI_Win win;

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
  MPI_Barrier(MPI_COMM_WORLD);

  std::vector<MPI_Request> requests(dests.size());
  MPI_Request barrier_request;

  unsigned int send_val = 12345, recv_val=0, send_step=0;
  int tag = 100;


  // Start sends
  for (std::set<unsigned int>::const_iterator it = dests.begin();
       it!=dests.end(); ++it)
    MPI_Issend (&send_val, 1, MPI_UNSIGNED, *it,
		tag, MPI_COMM_WORLD, &requests[send_step++]);

  bool done=false, barrier_act=false;

  MPI_Status status;

  while (!done)
    {
      int flag=0;

      MPI_Iprobe (MPI_ANY_SOURCE, tag, MPI_COMM_WORLD,
		  &flag, &status);

      if (flag)
	{
	  MPI_Recv (&recv_val, 1, MPI_UNSIGNED,
		    status.MPI_SOURCE, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	  //std::cout << send_val << " " << recv_val << std::endl;
	}

      if (barrier_act)
	{
	  flag=0;
	  MPI_Test (&barrier_request, &flag, MPI_STATUS_IGNORE);
	  if (flag)
	    {
	      std::cout << "Rank " << procid << " is done.\n";
	      done = true;
	    }
	}
      else
	{
	  flag=0;
	  MPI_Testall (requests.size(), &requests[0], &flag, MPI_STATUS_IGNORE);

	  if (flag)
	    {
	      std::cout << "Rank " << procid << " reached barrier.\n";
	      MPI_Ibarrier (MPI_COMM_WORLD, &barrier_request);
	      barrier_act = true;
	    }
	}
    }

  // MPI_Win_free (&win);
  MPI_Finalize();

  return 0;
}
