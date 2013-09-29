#ifndef RANDOM_IN_RANGE_H
#define RANDOM_IN_RANGE_H

#include <set>

namespace MPI_Playground {

  void random_in_range (unsigned int nentries,
			unsigned int nmax,
			std::set<unsigned int> &vals);
}

#endif
