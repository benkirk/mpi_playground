#include "mpi.h"
#include <vector>
#include <iostream>

int main (int argc, char **argv)
{
  int numprocs, procid;
  MPI_Win win;

  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank (MPI_COMM_WORLD, &procid);

  std::vector<unsigned int> recv_buf(numprocs, 0);

  MPI_Win_create (&recv_buf[0], recv_buf.size()*sizeof(unsigned int), sizeof(unsigned int),
		  MPI_INFO_NULL, MPI_COMM_WORLD,
		  &win);

  MPI_Win_fence (MPI_MODE_NOPRECEDE /* 0 */, win);

  for (unsigned int proc=0; proc<numprocs; proc++)
    MPI_Put (&procid, 1, MPI_UNSIGNED,
	     proc, procid, 1, MPI_UNSIGNED,
	     win);

  MPI_Win_fence (0, win);


  for (unsigned int proc=0; proc<numprocs; proc++)
    {
      MPI_Barrier(MPI_COMM_WORLD);

      if (proc == procid)
	{
	  for (unsigned int i=0; i<recv_buf.size(); i++)
	    std::cout << recv_buf[i] << " ";
	  std::cout << std::endl;
	}
    }

  MPI_Win_free (&win);
  MPI_Finalize();

  return 0;
}
