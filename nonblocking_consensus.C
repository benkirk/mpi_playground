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


  //--------------------------------------------------------
  double elapsed_time=0.;
  unsigned int recv_cnt=0;
  unsigned int recv_loop=0;

  for (unsigned int cnt=0; cnt<nrep; cnt++)
    {
      recv_cnt=0;
      std::vector<MPI_Request> requests(dests.size());
      MPI_Request barrier = MPI_REQUEST_NULL;

      unsigned int send_val = 12345, recv_val=0, send_step=0;
      int tag = 100;

      const double starttime = MPI_Wtime();

      // Start sends
      for (std::set<unsigned int>::const_iterator it = dests.begin();
	   it!=dests.end(); ++it)
	MPI_Issend (&send_val, 1, MPI_UNSIGNED,
		    *it, tag, MPI_COMM_WORLD,
		    &requests[send_step++]);


      // enter recv loop
      for (int done=0, msg_waiting=0, all_sent=0; !done; msg_waiting=0, ++recv_loop)
	{
	  MPI_Iprobe (MPI_ANY_SOURCE, tag, MPI_COMM_WORLD,
		      &msg_waiting, MPI_STATUS_IGNORE);

	  if (msg_waiting)
	    {
	      MPI_Recv (&recv_val, 1, MPI_UNSIGNED,
			MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	      recv_cnt++;
	    }

	  if (barrier == MPI_REQUEST_NULL)
	    {
	      MPI_Testall (requests.size(), &requests[0], &all_sent, MPI_STATUSES_IGNORE);

	      if (all_sent)
		MPI_Ibarrier (MPI_COMM_WORLD, &barrier);
	    }
	  else
	    {
	      MPI_Test (&barrier, &done, MPI_STATUS_IGNORE);
	    }

	  // if ((MPI_Wtime() - starttime) > 5)
	  // 	{
	  // 	  std::cerr << "\nRank " << procid << " has not completed in 5 sec!!\n";
	  // 	  for (unsigned int i=0; i<requests.size(); i++)
	  // 	    {
	  // 	      int finished=0;
	  // 	      MPI_Test (&requests[i], &finished, MPI_STATUS_IGNORE);
	  // 	      if (!finished)
	  // 		std::cout << " comm " << i << " not finished\n";
	  // 	    }
	  // 	  MPI_Abort(MPI_COMM_WORLD, 1);
	  // 	}
	}

      const double enddtime = MPI_Wtime();

      elapsed_time += (enddtime - starttime);
    }

  for (unsigned int proc=0; proc<numprocs; proc++)
    {
      MPI_Barrier(MPI_COMM_WORLD);

      if (proc == procid)
	std::cout << "Rank " << procid << " is done, received " << recv_cnt << " values in "
		  << (elapsed_time/nrep)*1e6 << " (us), recv_loop = " << recv_loop << "\n";
    }


  MPI_Finalize();

  return 0;
}
