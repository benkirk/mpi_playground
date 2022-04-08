//#include "process_cmdline.h"
#include "is_list_unique.h"
#include "parallel_sort.h"

#include <algorithm>
#include <iterator>
#include <iostream>
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <rpc/rpc.h>
#include <cstddef>
#include <stdio.h>
#include <numeric>
//#include <boost/timer.hpp>
#include <mpi.h>

using namespace MPI_Playground;

namespace
{
  int rank, nprocs;
  std::string io_basename = "test.";
  const unsigned int bytes_per_MB = 1024 * 1024;
  const unsigned int bytes_per_GB = bytes_per_MB * 1024;
  const std::string DATASET_NAME("dset");
}



template <typename T> void print_container(const T &data)
{
  for (auto i: data) std::cout << i << ' ';
  std::cout << std::endl;
}



int main (int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  //process_command_line (argc, argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  //const bool print_aggregate = (rank == 0) && (nprocs > 1);


  //std::vector<long int> ll;
  //append_random_list<long int>(ll, 10);
  //append_random_list<long int>(ll, 10, 0, static_cast<long int>(std::numeric_limits<short int>::max()));
  //std::sort(ll.begin(), ll.end());
  //print_container(ll);
  //
  //std::vector<short int> ls;
  //append_random_list(ls, 10);
  //std::sort(ls.begin(), ls.end());
  //print_container(ls);

  run_unique_check();

  MPI_Finalize();

  return 0;
}
