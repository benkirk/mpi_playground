dnl -------------------------------------------------------------
dnl Threading Building Blocks
dnl -------------------------------------------------------------
AC_DEFUN([CONFIGURE_TBB],
[
  AC_ARG_ENABLE(tbb,
                AS_HELP_STRING([--disable-tbb],
                               [build without threading support via Threading Building Blocks]),
                [AS_CASE("${enableval}",
                         [yes], [enabletbb=yes],
                         [no],  [enabletbb=no],
                         [AC_MSG_ERROR(bad value ${enableval} for --enable-tbb)])],
                [enabletbb=yes])

  AS_IF([test "x$enabletbb" = "xyes"],
        [
          AC_ARG_WITH(tbb,
                      AS_HELP_STRING([--with-tbb=PATH],[Specify the path where Threading Building Blocks is installed]),
                      withtbb=$withval,
                      withtbb=$TBB_DIR)

          AC_ARG_WITH(tbb-lib,
                      AS_HELP_STRING([--with-tbb-lib=PATH],[Specify the path to Threading Building Blocks libraries]),
                      withtbblib=$withval,
                      withtbblib=$TBB_LIB_PATH)

          AS_IF([test "$withtbb" != no],
                [
                  AS_IF([test "x$withtbb" = "x"], [withtbb=/usr])
                  AC_CHECK_HEADER($withtbb/include/tbb/tbb.h, TBB_INCLUDE_PATH=$withtbb/include)
                  AS_IF([test "x$withtbblib" != "x"], [TBB_LIBS=$withtbblib], [TBB_LIBS=$withtbb/lib])
                ])

          AS_IF([test -r $TBB_INCLUDE_PATH/tbb/tbb.h],
                [
                  TBB_LIBRARY="-L$TBB_LIBS -ltbb"
                  TBB_INCLUDE=-I$TBB_INCLUDE_PATH

                  dnl Add rpath flags to the link line.
                  AS_IF([test "x$RPATHFLAG" != "x" && test -d $TBB_LIBS], [TBB_LIBRARY="${RPATHFLAG}${TBB_LIBS} $TBB_LIBRARY"])
                ],
                [enabletbb=no])

          dnl If TBB is still enabled at this point, make sure we can compile
          dnl a test code which uses it
          AS_IF([test "x$enabletbb" != "xno"],
                [
                  AC_MSG_CHECKING(for tbb::info::default_concurrency() support)
                  AC_LANG_PUSH([C++])

                  dnl Add TBB headers to CXXFLAGS, which will be used by AC_COMPILE_IFELSE.
                  saveCXXFLAGS="$CXXFLAGS"
                  CXXFLAGS="$saveCXXFLAGS $TBB_INCLUDE"

                  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([
                      @%:@include <tbb/info.h>
                    ],
                    [
                      int num_threads = tbb::info::default_concurrency();
                    ])],
                    [
                      AC_MSG_RESULT(yes)
                      enabletbb=yes
                    ],
                    [
                      AC_MSG_RESULT(no)
                      enabletbb=no
                    ])

                  dnl Restore original flags
                  CXXFLAGS=$saveCXXFLAGS

                  AC_LANG_POP([C++])
                ])

          dnl If TBB is still enabled at this point, set all the necessary defines and print
          dnl a success message.
          AS_IF([test "x$enabletbb" != "xno"],
                [
                  AC_DEFINE_UNQUOTED(DETECTED_TBB_VERSION_MAJOR, [$tbbmajor], [TBB's major version number, as detected by tbb.m4])
                  AC_DEFINE_UNQUOTED(DETECTED_TBB_VERSION_MINOR, [$tbbminor], [TBB's minor version number, as detected by tbb.m4])

                  AC_SUBST(TBB_LIBRARY)
                  AC_SUBST(TBB_INCLUDE)
                  AC_DEFINE(HAVE_TBB, 1, [Flag indicating whether the library shall be compiled to use the Threading Building Blocks])
                  AC_MSG_RESULT(<<< Configuring with Intel TBB threading support >>>)
                ])
        ])
])
