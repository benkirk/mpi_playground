AC_PREREQ([2.69])

AC_INIT(hpl, 2.3, hpl@icl.utk.edu)
AC_CONFIG_SRCDIR([include/hpl.h])
AC_CONFIG_HEADERS([include/hplconfig.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([dist-bzip2 subdir-objects color-tests 1.11])
# use silent rules - automake 1.11
AM_SILENT_RULES(yes)

AX_PROG_CC_MPI

AC_PROG_RANLIB

AC_PROG_INSTALL

AM_PROG_CC_C_O
AC_LANG_C

dnl
dnl AX_BLAS requires Fortran compiler and detects fortran libraries in $FLIBS
dnl
dnl AX_BLAS(LIBS="$BLAS_LIBS $LIBS $FLIBS")
dnl

HPL_BLAS(LIBS="$BLAS_LIBS $LIBS",AC_MSG_ERROR([BLAS not found]))

dnl FIXME: test for CBLAS: Atlas, MKL, OpenBLAS, ESSL, ...
dnl FIXME: test for GSL CBLAS

AC_CHECK_FUNCS([dgemm_])

AC_CHECK_HEADERS([mpi.h])

AC_CONFIG_FILES([Makefile src/Makefile testing/Makefile])

AC_OUTPUT
