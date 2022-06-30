/**
 * c.f.: https://raw.githubusercontent.com/rabauke/mpl/master/examples/parallel_sort_mpi.c
 * parallel sort algorithm for distributed memory computers
 *
 * algorithm works as follows:
 *   1) each process draws (size-1) random samples from its local data
 *   2) all processes gather local random samples => size*(size-1) samples
 *   3) size*(size-1) samples are sorted locally
 *   4) pick (size-1) pivot elements from the globally sorted sample
 *   5) partition local data with respect to the pivot elements into size bins
 *   6) redistribute data such that data in bin i goes to process with rank i
 *   7) sort redistributed data locally
 *
 * Note that the amount of data at each process changes during the algorithm.
 * In worst case, a single process may hold finally all data.
 *
 */

#include "parallel_sort.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "mpi.h"
#include <unistd.h>
#include <assert.h>



void fill_random(CVector v) {
  for (int i = 0; i < v.N; ++i) {
#ifdef DO_DOUBLE
    v.data[i] = (double)rand() / (RAND_MAX + 1.);
#else
    v.data[i] = rand();
#endif
  }
}



static int cmp(const void *p1_, const void *p2_) {
  const datatype *const p1 = p1_;
  const datatype *const p2 = p2_;
  return (*p1 == *p2) ? 0 : (*p1 < *p2 ? -1 : 1);
}



datatype *partition(datatype *first, datatype *last, datatype pivot) {
  for (; first != last; ++first)
    if (!((*first) < pivot))
      break;

  if (first == last)
    return first;

  for (datatype *i = first + 1; i != last; ++i) {
    if ((*i) < pivot) {
      datatype temp = *i;
      *i = *first;
      *first = temp;
      ++first;
    }
  }
  return first;
}



bool is_unique(CVector v) {
  int i;

  if (v.N == 1)
    return true;

  for (i=1; i<v.N; i++) {
    assert (v.data[i] >= v.data[i-1]);
    if (v.data[i] == v.data[i-1])
      return false;
  }

  return true;
}



CVector parallel_sort(CVector v) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  datatype *local_pivots = malloc(size * sizeof(*local_pivots));
  if (local_pivots == NULL)
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);

  datatype *pivots = malloc(size * (size + 1) * sizeof(*pivots));
  if (pivots == NULL)
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);

  for (int i = 0; i < size - 1; ++i)
    local_pivots[i] = v.data[(int)(v.N * (double)rand() / (RAND_MAX + 1.))];

  MPI_Allgather(local_pivots, size - 1, MY_MPI_DATATYPE,
                pivots, size - 1, MY_MPI_DATATYPE,
                MPI_COMM_WORLD);

  qsort(pivots, size * (size - 1), sizeof(datatype), cmp);

  for (int i = 1; i < size; ++i)
    local_pivots[i - 1] = pivots[i * (size - 1)];

  datatype **pivot_pos = malloc((size + 1) * sizeof(*pivot_pos));
  if (pivot_pos == NULL)
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);

  pivot_pos[0] = v.data;
  for (int i = 0; i < size - 1; ++i)
    pivot_pos[i + 1] = partition(pivot_pos[i], v.data + v.N, local_pivots[i]);
  pivot_pos[size] = v.data + v.N;

  int *local_block_sizes = malloc(size * sizeof(*local_block_sizes));
  if (local_block_sizes == NULL)
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);

  int *block_sizes = malloc(size * size * sizeof(*block_sizes));
  if (block_sizes == NULL)
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);

  for (int i = 0; i < size; ++i)
    local_block_sizes[i] = pivot_pos[i + 1] - pivot_pos[i];

  MPI_Allgather(local_block_sizes, size, MPI_INT, block_sizes, size, MPI_INT, MPI_COMM_WORLD);

  int send_pos = 0, recv_pos = 0;
  int sendcounts[size], sdispls[size], recvcounts[size], rdispls[size];

  for (int i = 0; i < size; ++i) {
    sendcounts[i] = block_sizes[rank * size + i];
    sdispls[i] = send_pos;
    send_pos += block_sizes[rank * size + i];
    recvcounts[i] = block_sizes[rank + size * i];
    rdispls[i] = recv_pos;
    recv_pos += block_sizes[rank + size * i];
  }

  datatype *v2 = malloc(recv_pos * sizeof(*v2));
  if (v2 == NULL)
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);

  MPI_Alltoallv(v.data, sendcounts, sdispls, MY_MPI_DATATYPE,
                v2, recvcounts, rdispls, MY_MPI_DATATYPE,
                MPI_COMM_WORLD);
  qsort(v2, recv_pos, sizeof(datatype), cmp);

  free(block_sizes);
  free(local_block_sizes);
  free(pivot_pos);
  free(pivots);
  free(local_pivots);

  return (CVector){v2, recv_pos};
}



int run_unique_check()
{
  //MPI_Init(&argc, &argv);
  int rank, size, i, r;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  srand(time(NULL) * rank);

  //const int N = 100000000 / size;
  const int N = 5000 / size;
  datatype *v = malloc(N * sizeof(*v));
  if (v == NULL)
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);

  fill_random((CVector){v, N});
  if (rank == (size-1)) v[0] = v[1];

  /**
  for (r=0; r<size; r++)
    {
      MPI_Barrier(MPI_COMM_WORLD);
      if (r == rank)
        {
          printf("\nRank %d, data (%d)", rank, N);
          if (is_unique((CVector){v, N}))
            printf(", is unique:\n");
          else
            printf(" ** is NOT unique ** :\n");

          for (i=0; i<N; i++)
#ifdef DO_DOUBLE
            printf("%g ", v[i]);
#else
            printf("%d ", v[i]);
#endif
          printf("\n");
        }
      fflush(stdout);
      MPI_Barrier(MPI_COMM_WORLD);
    }
  */


  CVector sorted = parallel_sort((CVector){v, N});

  int i_have_dups = is_unique(sorted) ? 0 : 1;
  int global_dups;
  MPI_Allreduce(&i_have_dups, &global_dups, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

  for (r=0; r<size; r++)
    {
      MPI_Barrier(MPI_COMM_WORLD);
      if (r == rank)
        {
          printf("\nRank %d, sorted (%d)", rank, sorted.N);
          if (i_have_dups == 0)
            printf(", is unique:\n");
          else
            printf(" *** is NOT unique *** :\n");

          for (i=0; i<sorted.N; i++) {
              printf(
#ifdef DO_DOUBLE
                     "%g%s ",
#else
                     "%d%s ",
#endif
                     sorted.data[i],
                     (i != 0 && (sorted.data[i-1] == sorted.data[i])) ? "<---" : "");
          }
          printf("\n");
        }
      fflush(stdout);
      MPI_Barrier(MPI_COMM_WORLD);
    }

  if (rank == 0)
    if (global_dups == 1)
      printf("\nDetected Duplicates\n");
    else
      printf("\nGlobally Unique\n");

  free(sorted.data);
  free(v);


  //MPI_Finalize();
  return EXIT_SUCCESS;
}
