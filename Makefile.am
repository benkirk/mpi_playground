SUBDIRS = osu-benchmarks mpi4py

if ENABLE_LIBMESH
  SUBDIRS += libmesh_apps
endif

if ENABLE_MPL
  SUBDIRS += mpl_examples
endif

AM_LDFLAGS = $(HDF5_LDFLAGS)
LIBS       = $(HDF5_LIBS) $(FCLIBS)


bin_PROGRAMS =

bin_PROGRAMS     += ibarrier one_sided nonblocking_consensus alltoall alltoall_avg abort round_robin_pt2pt test_io is_list_unique

ibarrier_SOURCES  = ibarrier.C
one_sided_SOURCES = one_sided.C
abort_SOURCES     = abort.C
round_robin_pt2pt_SOURCES = round_robin_pt2pt.C

nonblocking_consensus_SOURCES = nonblocking_consensus.C random_in_range.C random_in_range.h process_cmdline.C process_cmdline.h
alltoall_SOURCES              = alltoall.C random_in_range.C random_in_range.h process_cmdline.C process_cmdline.h
alltoall_avg_SOURCES          = alltoall_avg.C

highfive_sources = \
     H5/include/highfive/H5PropertyList.hpp \
     H5/include/highfive/H5Attribute.hpp \
     H5/include/highfive/H5Reference.hpp \
     H5/include/highfive/H5DataSpace.hpp \
     H5/include/highfive/h5easy_bits/H5Easy_opencv.hpp \
     H5/include/highfive/h5easy_bits/H5Easy_misc.hpp \
     H5/include/highfive/h5easy_bits/H5Easy_public.hpp \
     H5/include/highfive/h5easy_bits/H5Easy_Eigen.hpp \
     H5/include/highfive/h5easy_bits/H5Easy_vector.hpp \
     H5/include/highfive/h5easy_bits/H5Easy_xtensor.hpp \
     H5/include/highfive/h5easy_bits/H5Easy_scalar.hpp \
     H5/include/highfive/H5Selection.hpp \
     H5/include/highfive/H5File.hpp \
     H5/include/highfive/H5Utility.hpp \
     H5/include/highfive/H5DataSet.hpp \
     H5/include/highfive/H5Object.hpp \
     H5/include/highfive/H5Version.hpp.in \
     H5/include/highfive/bits/H5Attribute_misc.hpp \
     H5/include/highfive/bits/H5Utils.hpp \
     H5/include/highfive/bits/H5Dataspace_misc.hpp \
     H5/include/highfive/bits/H5DataType_misc.hpp \
     H5/include/highfive/bits/H5_definitions.hpp \
     H5/include/highfive/bits/H5PropertyList_misc.hpp \
     H5/include/highfive/bits/H5Annotate_traits_misc.hpp \
     H5/include/highfive/bits/H5Path_traits_misc.hpp \
     H5/include/highfive/bits/H5Exception_misc.hpp \
     H5/include/highfive/bits/H5DataSet_misc.hpp \
     H5/include/highfive/bits/H5Iterables_misc.hpp \
     H5/include/highfive/bits/H5Node_traits_misc.hpp \
     H5/include/highfive/bits/H5Node_traits.hpp \
     H5/include/highfive/bits/H5Converter_misc.hpp \
     H5/include/highfive/bits/H5Slice_traits.hpp \
     H5/include/highfive/bits/H5Selection_misc.hpp \
     H5/include/highfive/bits/H5File_misc.hpp \
     H5/include/highfive/bits/H5ReadWrite_misc.hpp \
     H5/include/highfive/bits/H5Object_misc.hpp \
     H5/include/highfive/bits/H5Annotate_traits.hpp \
     H5/include/highfive/bits/H5Path_traits.hpp \
     H5/include/highfive/bits/H5Slice_traits_misc.hpp \
     H5/include/highfive/bits/H5FileDriver_misc.hpp \
     H5/include/highfive/bits/H5Reference_misc.hpp \
     H5/include/highfive/H5Exception.hpp \
     H5/include/highfive/H5Easy.hpp \
     H5/include/highfive/H5FileDriver.hpp \
     H5/include/highfive/H5DataType.hpp \
     H5/include/highfive/H5Group.hpp

test_io_SOURCES = test_io.C \
		  test_fortran_io.f90 test_fortran_io.h \
                  process_cmdline.C process_cmdline.h \
	          $(highfive_sources)
test_io_CXXFLAGS = -std=c++11 -O2
test_io_CPPFLAGS = $(HDF5_CPPFLAGS) -I$(top_srcdir)/H5/include

is_list_unique_SOURCES = is_list_unique.C is_list_unique.h process_cmdline.C \
                         parallel_sort.c parallel_sort.h
is_list_unique_CCFLAGS = -Wall

if ENABLE_MPL
  bin_PROGRAMS += parallel_sort_mpl

  parallel_sort_mpl_SOURCES = parallel_sort_mpl.C
  parallel_sort_mpl_CPPFLAGS = -I$(top_srcdir)/mpl -DBUILD_APP $(TBB_INCLUDE)
  parallel_sort_mpl_LDADD = $(TBB_LIBRARY)
  parallel_sort_mpl_CXXFLAGS = -Wall
endif

DISTCLEANFILES = *~

#
# special things to do when running 'make clean/distclean'
clean-local:
	rm -rf `find $(builddir) -type d -name "*.dSYM"`

ouput_files = test.*.xdr test.*.bin test.*.h5 test.*.fbin out_?.e
log_files   = slurm-*.out MPI-*.log

MOSTLYCLEANFILES = $(ouput_files) $(logfiles)

cleandata:
	rm -f $(ouput_files)

cleanlogs:
	rm -f $(log_files)
