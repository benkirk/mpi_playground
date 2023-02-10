# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([mpi_play],[1.0.0],[benkirk@ucar.edu])
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
AM_PROG_CC_C_O # mpl_examples/Makefile.am:74: warning: compiling ... in subdir requires 'AM_PROG_CC_C_O'
AM_PATH_PYTHON
AC_ARG_VAR(PYTHONPATH, compile-time PYTHONPATH (e.g. for mpi4py))
AC_LANG([C++])

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
AC_ARG_ENABLE(mpl,
              AS_HELP_STRING([--disable-mpl],
                             [build without MPL]),
              enablempl=$enableval,
              enablempl=yes)

if test "x$enable_mpl" = xno; then
  have_mpl=no
else
  AC_CHECK_FILE($top_srcdir/mpl/mpl/mpl.hpp, [have_mpl=yes], [have_mpl=no])
  if test "x$have_mpl" = xyes; then
     AC_DEFINE(HAVE_MPL,1,[Define if the C++ MPI MPL layer is available])
  fi
fi
AM_CONDITIONAL(ENABLE_MPL, test x$have_mpl = xyes)

# Checks for MPI features
AC_CHECK_FUNC([MPI_Win_create], [mpi_win_create=true])
AC_CHECK_FUNC([MPI_Ibarrier],   [mpi_ibarrier=true], [mpi_ibarrier=false])

CONFIGURE_TBB
AM_CONDITIONAL(HAVE_TBB, test x$enabletbb = xyes)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

# Check for external libraries
# --------------------------------------------------------------
# XDR binary IO support - enabled by default
# This used to be tested in libmesh_core_features.m4 since your
# system either had it or it didn't. Now it's possible for the
# XDR headers to be in different places, so it's more convenient
# to test for it here.
# --------------------------------------------------------------

dnl Setting --enable-xdr-required causes an error to be emitted
dnl during configure if XDR headers are not detected successfully during
dnl configure.  This is useful for app codes which require XDR (like
dnl MOOSE-based apps).
AC_ARG_ENABLE(xdr-required,
              AC_HELP_STRING([--enable-xdr-required],
                             [Error if XDR is not detected by configure]),
              [AS_CASE("${enableval}",
                       [yes], [xdrrequired=yes],
                       [no],  [xdrrequired=no],
                       [AC_MSG_ERROR(bad value ${enableval} for --enable-xdr-required)])],
                   [xdrrequired=no])


AC_ARG_ENABLE(xdr,
              AS_HELP_STRING([--disable-xdr],
                             [build without XDR platform-independent binary I/O]),
              enablexdr=$enableval,
              enablexdr=yes)

AS_IF([test "$enablexdr" != no],
      [
      dnl Check whether the system/compiler has xdr.h in /usr/include and glibc.
      AC_MSG_CHECKING([for built-in XDR support])
      CONFIGURE_XDR

      dnl Check for headers in /usr/include/tirpc. (Fedora 28 does this.)
      AS_IF([test "x$enablexdr" = "xno"],
            [
              AC_MSG_CHECKING([for XDR support in /usr/include/tirpc])
              old_CPPFLAGS="$CPPFLAGS"
              old_LIBS="$LIBS"
              CPPFLAGS="$CPPFLAGS -I/usr/include/tirpc"
              LIBS="$LIBS -ltirpc"

              CONFIGURE_XDR

              dnl If that worked, append the required include paths and libraries as necessary.
              AS_IF([test "x$enablexdr" = "xyes"],
                    [
                      libmesh_INCLUDES="$libmesh_INCLUDES -I/usr/include/tirpc"
                      libmesh_LIBS="$libmesh_LIBS -ltirpc"
                    ])

              dnl Reset flags after testing
              CPPFLAGS="$old_CPPFLAGS"
              LIBS="$old_LIBS"
           ])
      ])
AS_IF([test "x$enablexdr" = "xno" && test "x$xdrrequired" = "xyes"],
      [AC_MSG_ERROR([*** XDR was not found, but --enable-xdr-required was specified.], 4)])

# -------------------------------------------------------------



## Check for HDF5 support
#AX_LIB_HDF5()
CONFIGURE_HDF5

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
