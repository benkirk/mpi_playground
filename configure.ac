#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([mpi_play],[0.0.1],[benjamin.kirk@nasa.gov])
AC_MSG_RESULT(---------------------------------------------)
AC_MSG_RESULT(-------- Configuring MPI Playground ---------)
AC_MSG_RESULT(---------------------------------------------)
#AC_CONFIG_SRCDIR([ibarrier.C])
top_srcdir=`dirname $0`
#srcdir=`dirname $0`
AC_CHECK_FILE(ibarrier.C, [vpath_build=no], [vpath_build=yes])
AC_CONFIG_HEADERS([mpi_play_config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([dist-bzip2 tar-pax subdir-objects color-tests 1.11])
# use silent rules - automake 1.11
AM_SILENT_RULES(yes)

# Checks for programs.
AC_PROG_CXX([mpicxx mpic++ mpiCC])
AC_PROG_CC([mpicc])
AC_PROG_FC([mpif90])
AC_PROG_F77([mpif90])
AC_CHECK_PROGS([MPIEXEC], [mpiexec mpiexec_mpt mpirun], [mpiexec])

AC_FC_LIBRARY_LDFLAGS
AC_FC_WRAPPERS
AM_PATH_PYTHON
AC_ARG_VAR(PYTHONPATH, compile-time PYTHONPATH (e.g. for mpi4py))

#----------------
# Libmesh Config
#----------------
AX_PATH_LIBMESH([have_libmesh=yes], [have_libmesh=no])

# prefer libMesh compilers when available
if test "$have_libmesh" = "yes"; then
   CXX=$($LIBMESH_CONFIG --cxx)
   CC=$($LIBMESH_CONFIG --cc)
   FC=$($LIBMESH_CONFIG --fc)
   F77=$($LIBMESH_CONFIG --f77)

   CXXFLAGS="$CXXFLAGS `$LIBMESH_CONFIG --cppflags` `$LIBMESH_CONFIG --cxxflags`"
   CFLAGS="$CFLAGS `$LIBMESH_CONFIG --cppflags` `$LIBMESH_CONFIG --cflags`"
   FCFLAGS="$FCFLAGS `$LIBMESH_CONFIG --fflags`"
else
   # Checks for programs.
   AC_MSG_RESULT([No functional libMesh found])
fi

AM_CONDITIONAL(ENABLE_LIBMESH, test x$have_libmesh = xyes)

# Checks for libraries.
AC_CHECK_FILE($top_srcdir/mpl/mpl/mpl.hpp, [have_mpl=yes], [have_mpl=no])
if test "x$have_mpl" = xyes; then
   AC_DEFINE(HAVE_MPL,1,[Define if the C++ MPI MPL layer is available])
fi
AM_CONDITIONAL(ENABLE_MPL, test x$have_mpl = xyes)

# Checks for MPI features
AC_CHECK_FUNC([MPI_Win_create], [mpi_win_create=true])
AC_CHECK_FUNC([MPI_Ibarrier],   [mpi_ibarrier=true], [mpi_ibarrier=false])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Check for external libraries
## Check for HDF5 support
AX_LIB_HDF5()

# Configure subpackages
AC_CONFIG_SUBDIRS([osu-benchmarks])

# Set up output files
AC_CONFIG_FILES([Makefile
                 libmesh_apps/Makefile
                 mpl_examples/Makefile
                 mpi4py/Makefile
                 mpi4py/py_wrapper
                 ])
AC_OUTPUT
AC_MSG_RESULT(---------------------------------------------)
AC_MSG_RESULT(------ Done Configuring MPI Playground ------)
AC_MSG_RESULT(---------------------------------------------)

AX_SUMMARIZE_CONFIG
