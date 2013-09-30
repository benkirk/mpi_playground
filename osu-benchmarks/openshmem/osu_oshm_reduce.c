#define BENCHMARK "OSU OpenSHMEM Reduce Latency Test"
/*
 * Copyright (C) 2002-2013 the Network-Based Computing Laboratory
 * (NBCL), The Ohio State University.
 *
 * Contact: Dr. D. K. Panda (panda@cse.ohio-state.edu)
 */

/*
This program is available under BSD licensing.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

(1) Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

(2) Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

(3) Neither the name of The Ohio State University nor the names of
their contributors may be used to endorse or promote products derived
from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>
#include <shmem.h>
#include "osu_common.h"
#include "osu_coll.h"
#include <stdlib.h>

long pSyncBarrier[_SHMEM_BARRIER_SYNC_SIZE];
long pSyncRed[_SHMEM_REDUCE_SYNC_SIZE];

double pWrk[_SHMEM_REDUCE_SYNC_SIZE];
float pWrkF[_SHMEM_REDUCE_SYNC_SIZE];


int main(int argc, char *argv[])
{
    int i, numprocs, rank, size, align_size;
    int skip;
    static double latency = 0.0;
    int64_t t_start = 0, t_stop = 0, timer=0;
    static double avg_time = 0.0, max_time = 0.0, min_time = 0.0;
    float *sendbuf, *recvbuf, *s_buf1, *r_buf1;
    int max_msg_size = 1048576, full = 0, t;

    for ( t = 0; t < _SHMEM_BARRIER_SYNC_SIZE; t += 1) pSyncBarrier[t] = _SHMEM_SYNC_VALUE;
    for ( t = 0; t < _SHMEM_REDUCE_SYNC_SIZE; t += 1) pSyncRed[t] = _SHMEM_SYNC_VALUE;

    start_pes(0);
    rank = _my_pe();
    numprocs = _num_pes();

    if (process_args(argc, argv, rank, &max_msg_size, &full)) {
        return EXIT_SUCCESS;
    }

    if(numprocs < 2) {
        if(rank == 0) {
            fprintf(stderr, "This test requires at least two processes\n");
        }
        return EXIT_FAILURE;
    }

    print_header(rank, full);

    s_buf1 = r_buf1 = NULL;
    s_buf1 = (float *) shmalloc(sizeof(float)*(max_msg_size/sizeof(float)) + MAX_ALIGNMENT);
    if(NULL == s_buf1) {
        fprintf(stderr, "s_buf1 malloc failed.\n");
        exit(1);
    }
    r_buf1 = (float *) shmalloc(sizeof(float)*(max_msg_size/sizeof(float)) + MAX_ALIGNMENT);
    if(NULL == r_buf1) {
        fprintf(stderr, "r_buf2 malloc failed.\n");
        exit(1);
    }

    align_size = getpagesize();
    sendbuf = (float *)(((unsigned long) s_buf1 + (align_size - 1)) / align_size
                    * align_size);
    recvbuf = (float *)(((unsigned long) r_buf1 + (align_size - 1)) / align_size
                    * align_size);
    memset(sendbuf, 1, max_msg_size);
    memset(recvbuf, 0, max_msg_size);

    for(size=1; size*sizeof(float)<= max_msg_size; size *= 2) {

      if(size > LARGE_MESSAGE_SIZE) {
	skip = SKIP_LARGE;
	iterations = iterations_large;
      } else {
	skip = SKIP;
      }

      shmem_barrier(0, 0, numprocs, pSyncBarrier);
        
      timer=0;
      for(i=0; i < iterations + skip ; i++) {
	t_start = TIME();

	shmem_float_sum_to_all(recvbuf, sendbuf, size, 0, 0, numprocs, pWrkF, pSyncRed);

	t_stop=TIME();

	if(i>=skip){
	  timer+=t_stop-t_start;
	}
	shmem_barrier(0, 0, numprocs, pSyncBarrier);
      }
      latency = (double)(timer * 1.0) / iterations;


      shmem_double_min_to_all(&min_time, &latency, 1, 0, 0, numprocs, pWrk, pSyncRed);
      shmem_double_max_to_all(&max_time, &latency, 1, 0, 0, numprocs, pWrk, pSyncRed);
      shmem_double_sum_to_all(&avg_time, &latency, 1, 0, 0, numprocs, pWrk, pSyncRed);
      avg_time = avg_time/numprocs;

      print_data(rank, full, sizeof(float)*size, avg_time, min_time, max_time, iterations);

      shmem_barrier(0, 0, numprocs, pSyncBarrier);
    }

    shmem_barrier_all();

    shfree(s_buf1);
    shfree(r_buf1);

    return EXIT_SUCCESS;
}
