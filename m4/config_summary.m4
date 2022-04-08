# SYNOPSIS
#
#   Summarizes configuration settings.
#
#   AX_SUMMARIZE_CONFIG([, ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
#
# DESCRIPTION
#
#   Outputs a summary of relevant configuration settings.
#
# LAST MODIFICATION
#
#   2009-11-19
#

AC_DEFUN([AX_SUMMARIZE_CONFIG],
[

######################################################################################
echo
echo '----------------------------------- SUMMARY -----------------------------------'
echo
echo Package version............... : $PACKAGE-$VERSION
echo
echo C++ compiler.................. : $CXX
echo CXXFLAGS.......................: $CXXFLAGS
echo C compiler.................... : $CC
echo CFLAGS.........................: $CFLAGS
echo Fortran compiler.............. : $FC
echo Fortran 77 compiler........... : $F77
echo FCFLAGS........................: $FCFLAGS
echo libmesh dir................... : $LIBMESH_PREFIX
echo libmesh-config................ : $LIBMESH_CONFIG
echo Install dir................... : $prefix
echo Python install dir............ : $pythondir
echo PYTHONPATH.................... : $PYTHONPATH
echo Build user.................... : $USER
#echo Build host.................... : $BUILD_HOST
#echo Configure date................ : $BUILD_DATE
#echo Build architecture............ : $BUILD_ARCH
#echo SVN revision number........... : $BUILD_VERSION
#echo SVN status.................... : $BUILD_DEVSTATUS

######################################################################################
echo
echo Optional Dependencies:

######################################################################################
if test "x$have_libmesh" = "xyes"; then
  echo '   'libMesh.................... : yes
  echo '      'dir..................... : $LIBMESH_PREFIX
  echo '      'config.................. : $LIBMESH_CONFIG
else
  echo '   'libMesh.................... : no
fi

######################################################################################
if test "x$with_hdf5" = "xyes"; then
  echo '   'HDF5:...................... : yes
  echo '      'version/type............ : $HDF5_VERSION $HDF5_TYPE
  echo '      'CFLAGS.................. : $HDF5_CFLAGS
  echo '      'CPPFLAGS................ : $HDF5_CPPFLAGS
  echo '      'LDFLAGS................. : $HDF5_LDFLAGS
  echo '      'LIBS.................... : $HDF5_LIBS
else
  echo '   'HDF5:...................... : no
fi

######################################################################################
if test "x$have_mpl" = "xyes"; then
  echo '   'MPL:....................... : yes
else
  echo '   'MPL:....................... : no
fi

# ######################################################################################
# echo
# echo Chemistry Configuration:

# if test $enable_cantera	!= no; then
#   echo '   'Link with Cantera.......... : yes
#   echo '      'CANTERA_INCLUDE......... : $CANTERA_INCLUDE
#   echo '      'CANTERA_LIBS............ : $CANTERA_LIBS
# #  echo
# else
#   echo '   'Link with Cantera.......... : no
# fi

echo
echo '-------------------------------------------------------------------------------'

echo
echo Configure complete, now type \'make\' and then \'make install\'.
echo

])
