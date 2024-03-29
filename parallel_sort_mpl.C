#include <execution>
#include <cstdlib>
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include <mpl/mpl.hpp>

auto policy = std::execution::par_unseq;

static std::random_device rd;
static std::mt19937_64 mt(rd());

double get_random_double() {
  static std::uniform_real_distribution<double> dist(0, 1);
  return dist(mt);
}

long get_random_long() {
  static std::uniform_int_distribution<long> dist;
  return dist(mt);
}

void fill_random(std::vector<double> &v) {
  std::generate(policy, std::begin(v), std::end(v), get_random_double);
}

void fill_random(std::vector<long> &v) {
  std::generate(policy, std::begin(v), std::end(v), get_random_long);
}

template <typename T>
void make_unique(std::vector<T> &v) {
  std::sort(policy, begin(v), end(v));
  auto last = std::unique(policy, begin(v), end(v));
  v.erase(last, end(v));
  static std::mt19937 g(rd());
  std::shuffle(begin(v), end(v), g);
}

// parallel sort algorithm for distributed memory computers
//
// algorithm works as follows:
//   1) each process draws (size-1) random samples from its local data
//   2) all processes gather local random samples => size*(size-1) samples
//   3) size*(size-1) samples are sorted locally
//   4) pick (size-1) pivot elements from the globally sorted sample
//   5) partition local data with respect to the pivot elements into size bins
//   6) redistribute data such that data in bin i goes to process with rank i
//   7) sort redistributed data locally
//
// Note that the amount of data at each process changes during the algorithm.
// In worst case, a single process may hold finally all data.
//
template<typename T>
void parallel_sort(std::vector<T> &v)
{
  auto &comm_world{mpl::environment::comm_world()};
  const int rank{comm_world.rank()};
  const int size{comm_world.size()};

  std::vector<T> local_pivots, pivots(size * (size - 1));
  std::sample(begin(v), end(v), std::back_inserter(local_pivots), size - 1, mt);
  comm_world.allgather(local_pivots.data(), mpl::vector_layout<T>(size - 1), pivots.data(),
                       mpl::vector_layout<T>(size - 1));
  std::sort(policy, begin(pivots), end(pivots));

  local_pivots.clear();
  for (std::size_t i{1}; i < static_cast<std::size_t>(size); ++i)
    local_pivots.push_back(pivots[i * (size - 1)]);
  std::swap(local_pivots, pivots);

  std::vector<typename std::vector<T>::iterator> pivot_pos;
  pivot_pos.push_back(begin(v));
  for (T p : pivots)
    pivot_pos.push_back(std::partition(pivot_pos.back(), end(v), [p](T x) { return x < p; }));
  pivot_pos.push_back(end(v));

  std::vector<int> local_block_sizes, block_sizes(size * size);
  for (std::size_t i{0}; i < pivot_pos.size() - 1; ++i)
    local_block_sizes.push_back(
        static_cast<int>(std::distance(pivot_pos[i], pivot_pos[i + 1])));

  comm_world.allgather(local_block_sizes.data(), mpl::vector_layout<int>(size),
                       block_sizes.data(), mpl::vector_layout<int>(size));

  mpl::layouts<T> send_layouts, recv_layouts;
  int send_pos{0}, recv_pos{0};

  for (int i{0}; i < size; ++i) {
    send_layouts.push_back(mpl::indexed_layout<T>({{block_sizes[rank * size + i], send_pos}}));
    send_pos += block_sizes[rank * size + i];
    recv_layouts.push_back(mpl::indexed_layout<T>({{block_sizes[rank + size * i], recv_pos}}));
    recv_pos += block_sizes[rank + size * i];
  }

  std::vector<T> v_2(recv_pos);
  comm_world.alltoallv(v.data(), send_layouts, v_2.data(), recv_layouts);

  std::sort(policy, begin(v_2), end(v_2));

  std::swap(v, v_2);
}



#ifdef BUILD_APP
int main (int argc, char **argv)
{
  const auto &comm_world{mpl::environment::comm_world()};
  const int size{comm_world.size()};
  const int rank{comm_world.rank()};

  //const std::size_t N{100000000 / static_cast<std::size_t>(size)};
  const std::size_t N{50000 / static_cast<std::size_t>(size)};
  std::vector<long> v(N);
  fill_random(v);
  make_unique(v);
  //v[0] = v[1];
  parallel_sort(v);

  bool i_have_dups = false;
  for (auto r=0; r<size; r++) {
    comm_world.barrier();
    if (rank == r) {
      std::cout << "Rank " << rank << ", sorted (" << v.size() << "):" << std::endl;

      for (std::size_t i=0; i<v.size(); i++)
        {
          std::string sep{" "};
          if (i != 0 && (v[i-1] == v[i])) {
            i_have_dups = true;
            sep = "<--- ";
          }
          std::cout << v[i] << sep;
        }
      if (i_have_dups)
        std::cout << "*** is NOT unique ***";
      else
        std::cout << "is unique";

      std::cout << std::endl;
    }
  }

  // check for global duplicates
  bool global_dups{i_have_dups};
  comm_world.allreduce(mpl::max<bool>(), i_have_dups, global_dups);
  if (rank == 0) {
    std::string result = global_dups ?
      "\nDetected Duplicates\n" :
      "\nGlobally Unique\n";
    std::cout << result << std::endl;
  }


  return EXIT_SUCCESS;
}
#endif
