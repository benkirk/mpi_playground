bin_PROGRAMS = osu_bibw osu_bw osu_latency osu_mbw_mr osu_multi_lat

if MPI2_LIBRARY
    bin_PROGRAMS += osu_latency_mt
endif

if OPENACC
    AM_CFLAGS = -acc
endif
