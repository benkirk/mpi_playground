#ifndef __TEST_FORTRAN_IO_H__
#define __TEST_FORTRAN_IO_H__

#include "mpi_play_config.h"

//--------------------------------------------
#define WRITE_FORTRAN FC_FUNC(write_f, WRITE_F)

#ifdef __cplusplus
extern "C" // no name mangling
#endif

void WRITE_FORTRAN(int *rank, unsigned int *length, double *data);



//--------------------------------------------
#define READ_FORTRAN FC_FUNC(read_f, READ_F)

#ifdef __cplusplus
extern "C" // no name mangling
#endif

void READ_FORTRAN(int *rank, unsigned int *length, double *data);




#endif
