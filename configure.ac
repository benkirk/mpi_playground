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
#AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([dist-bzip2 color-tests 1.11])

# use silent rules - automake 1.11
AM_SILENT_RULES(yes)

# Checks for programs.
AC_PROG_CXX([mpicxx mpic++ mpiCC])
AC_PROG_CC([mpicc])

# Checks for libraries.

# Checks for MPI features
AC_CHECK_FUNC([MPI_Win_create], [mpi_win_create=true])
AC_CHECK_FUNC([MPI_Ibarrier],   [mpi_ibarrier=true], [mpi_ibarrier=false])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Configure subpackages
AC_CONFIG_SUBDIRS([osu-benchmarks])

# Set up output files
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
AC_MSG_RESULT(---------------------------------------------)
AC_MSG_RESULT(------ Done Configuring MPI Playground ------)
AC_MSG_RESULT(---------------------------------------------)
