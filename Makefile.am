SUBDIRS = osu-benchmarks

if ENABLE_LIBMESH
  SUBDIRS += libmesh_apps
endif

AM_LDFLAGS = $(HDF5_LDFLAGS)
LIBS       = $(FCLIBS) $(HDF5_LIBS)


bin_PROGRAMS =

bin_PROGRAMS     += ibarrier one_sided nonblocking_consensus alltoall abort test_io

ibarrier_SOURCES  = ibarrier.C
one_sided_SOURCES = one_sided.C
abort_SOURCES     = abort.C

nonblocking_consensus_SOURCES = nonblocking_consensus.C random_in_range.C random_in_range.h process_cmdline.C process_cmdline.h
alltoall_SOURCES              = alltoall.C random_in_range.C random_in_range.h process_cmdline.C process_cmdline.h


highfive_sources = \
     H5/include/highfive/bits/H5Annotate_traits.hpp \
     H5/include/highfive/bits/H5Annotate_traits_misc.hpp \
     H5/include/highfive/bits/H5Attribute_misc.hpp \
     H5/include/highfive/bits/H5Converter_misc.hpp \
     H5/include/highfive/bits/H5DataSet_misc.hpp \
     H5/include/highfive/bits/H5Dataspace_misc.hpp \
     H5/include/highfive/bits/H5DataType_misc.hpp \
     H5/include/highfive/bits/H5Exception_misc.hpp \
     H5/include/highfive/bits/H5File_misc.hpp \
     H5/include/highfive/bits/H5FileDriver_misc.hpp \
     H5/include/highfive/bits/H5Group_misc.hpp \
     H5/include/highfive/bits/H5Iterables_misc.hpp \
     H5/include/highfive/bits/H5Node_traits.hpp \
     H5/include/highfive/bits/H5Node_traits_misc.hpp \
     H5/include/highfive/bits/H5Object_misc.hpp \
     H5/include/highfive/bits/H5PropertyList_misc.hpp \
     H5/include/highfive/bits/H5Selection_misc.hpp \
     H5/include/highfive/bits/H5Slice_traits.hpp \
     H5/include/highfive/bits/H5Slice_traits_misc.hpp \
     H5/include/highfive/bits/H5Utils.hpp \
     H5/include/highfive/H5Attribute.hpp \
     H5/include/highfive/H5DataSet.hpp \
     H5/include/highfive/H5DataSpace.hpp \
     H5/include/highfive/H5DataType.hpp \
     H5/include/highfive/H5Exception.hpp \
     H5/include/highfive/H5File.hpp \
     H5/include/highfive/H5FileDriver.hpp \
     H5/include/highfive/H5Group.hpp \
     H5/include/highfive/H5Object.hpp \
     H5/include/highfive/H5PropertyList.hpp \
     H5/include/highfive/H5Selection.hpp \
     H5/include/highfive/H5Utility.hpp



test_io_SOURCES = test_io.C \
		  test_fortran_io.f90 test_fortran_io.h \
                  process_cmdline.C process_cmdline.h \
	          $(highfive_sources)
test_io_CXXFLAGS = -std=c++11 -O2
test_io_CPPFLAGS = $(HDF5_CPPFLAGS) -I$(top_srcdir)/H5/include

DISTCLEANFILES = *~

ouput_files = test.*.xdr test.*.bin test.*.h5 test.*.fbin

MOSTLYCLEANFILES = $(ouput_files) slurm-*.out

cleandata:
	rm -f $(ouput_files)
