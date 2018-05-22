#include <iostream>
#include "mpi.h"

int main (int argc, char **argv)
{
  int numprocs, rank, errorcode=1;

  MPI_Init(&argc, &argv);

  MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);


  if (!rank)
    {
      std::cout << " *** Running " << argv[0] << " on " << numprocs << " ranks...\n"
                << "  --> calling MPI_Abort from rank " << rank << std::endl;

      MPI_Abort(MPI_COMM_WORLD, errorcode);

      std::cout << " *** Control returned on rank " << rank << " ???" << std::endl;
    }

  // Prevent the other ranks from exiting naturally
  MPI_Barrier(MPI_COMM_WORLD);

  std::cout << " *** Control returned to main() after MPI_Abort()??" << std::endl;

  return 0;
}
