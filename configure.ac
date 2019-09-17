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

AM_INIT_AUTOMAKE([dist-bzip2 subdir-objects color-tests 1.11])
# use silent rules - automake 1.11
AM_SILENT_RULES(yes)

# Checks for programs.
AC_PROG_CXX([mpicxx mpic++ mpiCC])
AC_PROG_CC([mpicc])
AC_PROG_FC([mpif90])
AC_PROG_F77([mpif90])
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
AC_ARG_ENABLE(hpl,
              AS_HELP_STRING([--disable-hpl],
                             [build without HPL benchmark support]),
              [AS_CASE("${enableval}",
                       [yes], [enablehpl=yes],
                       [no],  [enablehpl=no],
                       [AC_MSG_ERROR(bad value ${enableval} for --enable-hpl)])],
              [enablehpl=yes])
AS_IF([test "$enablehpl" = "yes"], [ AC_CONFIG_SUBDIRS([hpl]) ])
AM_CONDITIONAL(ENABLE_HPL, test x$enablehpl = xyes)


# Set up output files
AC_CONFIG_FILES([Makefile
                 libmesh_apps/Makefile
                 mpi4py/Makefile
                 mpi4py/py_wrapper
                 ])
AC_OUTPUT
AC_MSG_RESULT(---------------------------------------------)
AC_MSG_RESULT(------ Done Configuring MPI Playground ------)
AC_MSG_RESULT(---------------------------------------------)

AX_SUMMARIZE_CONFIG
