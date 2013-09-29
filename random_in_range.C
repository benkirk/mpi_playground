#include "random_in_range.h"
#include <set>
#include <ctime>
#include <cstdlib>
#include "mpi.h"

namespace MPI_Playground {

  void random_in_range (unsigned int nentries,
			unsigned int nmax,
			std::set<unsigned int> &vals)
  {
    static bool initialized = false;

    if (!initialized)
      {
	int procid;
	MPI_Comm_rank (MPI_COMM_WORLD, &procid);
	std::srand(std::time(NULL) + procid);
      }


    vals.clear();

    while (vals.size() != nentries)
      {
	vals.insert (std::rand() % nmax);
      }
  }
}
