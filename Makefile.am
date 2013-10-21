SUBDIRS = osu-benchmarks

bin_PROGRAMS =

bin_PROGRAMS     += ibarrier one_sided nonblocking_consensus alltoall

ibarrier_SOURCES  = ibarrier.C
one_sided_SOURCES = one_sided.C

nonblocking_consensus_SOURCES = nonblocking_consensus.C random_in_range.C random_in_range.h process_cmdline.C process_cmdline.h
alltoall_SOURCES              = alltoall.C random_in_range.C random_in_range.h process_cmdline.C process_cmdline.h

DISTCLEANFILES = *~
