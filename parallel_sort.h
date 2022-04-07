#ifndef __parallel_sort_h__
#define __parallel_sort_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

//#define DO_DOUBLE
#ifdef DO_DOUBLE
  typedef double datatype;
#  define MY_MPI_DATATYPE MPI_DOUBLE
#else
  typedef long datatype;
#  define MY_MPI_DATATYPE MPI_LONG
#endif

typedef struct {
  datatype *data;
  size_t N;
} CVector;

bool is_unique(CVector v);

CVector parallel_sort(CVector v);

int run_unique_check();

#ifdef __cplusplus
}
#endif

#endif // #define __parallel_sort_h__
