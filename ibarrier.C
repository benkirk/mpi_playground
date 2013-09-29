#include "mpi.h"

int main (int argc, char **argv)
{
  MPI_Request req;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Ibarrier(MPI_COMM_WORLD, &req);
  MPI_Wait(&req, &status);
  MPI_Finalize();

  return 0;
}
