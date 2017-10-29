#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([mpi_play], [0.0.1], [benjamin.kirk@nasa.gov])
AC_MSG_RESULT(---------------------------------------------)
AC_MSG_RESULT(-------- Configuring MPI Playground ---------)
AC_MSG_RESULT(---------------------------------------------)
AC_CONFIG_SRCDIR([ibarrier.C])
AC_CONFIG_HEADERS([mpi_play_config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([dist-bzip2 color-tests 1.11])

CXX_try="mpicxx mpic++ mpiCC"
CC_try="mpicc"
FC_try="mpif90"

#----------------
# Libmesh Config
#----------------
AX_PATH_LIBMESH([have_libmesh=yes], [have_libmesh=no])

# prefer libMesh compilers when available
if test "$have_libmesh" = "yes"; then
   CXX_try=`$LIBMESH_CONFIG --cxx`
   CC_try=`$LIBMESH_CONFIG --cc`
   FC_try=`$LIBMESH_CONFIG --fc`
   F77_try=`$LIBMESH_CONFIG --fc`
   CXXFLAGS="$CXXFLAGS `$LIBMESH_CONFIG --cppflags` `$LIBMESH_CONFIG --cxxflags`"
   CFLAGS="$CFLAGS `$LIBMESH_CONFIG --cppflags` `$LIBMESH_CONFIG --cflags`"
   FCFLAGS="$FCFLAGS `$LIBMESH_CONFIG --fflags`"
else
   # Checks for programs.
   AC_MSG_RESULT([No functional libMesh found])
fi

AM_CONDITIONAL(ENABLE_LIBMESH, test x$have_libmesh = xyes)

# Checks for programs.
AC_PROG_CXX($CXX_try)
AC_PROG_CC($CC_try)
AC_PROG_FC($FC_try)

# Checks for libraries.

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
                 ])
AC_OUTPUT
AC_MSG_RESULT(---------------------------------------------)
AC_MSG_RESULT(------ Done Configuring MPI Playground ------)
AC_MSG_RESULT(---------------------------------------------)
