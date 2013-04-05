dnl Process this file with autoconf to produce a configure script.

AC_INIT([PlibC], [0.1.7],[plibc-devel@lists.sf.net])
dnl Every other copy of the package version number gets its value from here
AM_INIT_AUTOMAKE(PlibC, 0.1.7)
AM_SILENT_RULES([yes])

dnl create a config.h file (Automake will add -DHAVE_CONFIG_H)
AM_CONFIG_HEADER(config.h)

AC_SUBST(VERSION)

CPPFLAGS="$CPPFLAGS -D_POSIX_SOURCE -D__USE_MINGW_ANSI_STDIO -D_CRT_NON_CONFORMING_SWPRINTFS -DUSE_IPV6"

ISODATE=`date +%Y-%m-%d`
AC_SUBST(ISODATE)

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

# Check system type
case "$target_os" in
*cygwin*)
     AC_DEFINE_UNQUOTED(WINDOWS,1,[This is a Windows system])
     AC_DEFINE_UNQUOTED(_WIN32,1,[This is a Windows system])
     ;;
*mingw*)
     AC_DEFINE_UNQUOTED(WINDOWS,1,[This is a Windows system])
     AC_DEFINE_UNQUOTED(_WIN32,1,[This is a Windows system])
     ;;
*)
;;
esac

# We define the paths here, because MinGW/GCC expands paths
# passed through the command line ("-DLOCALEDIR=..."). This would
# lead to hard-coded paths ("C:\mingw\mingw\bin...") that do
# not contain the actual installation.
AC_DEFINE_DIR([LOCALEDIR], [datadir/locale], [gettext catalogs])

dnl Checks for programs.
AC_PROG_INSTALL
AC_PROG_CC

# dynamic libraries
AC_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AC_LIBLTDL_INSTALLABLE
AC_PROG_LIBTOOL
AC_LIB_LTDL
AC_SUBST(LTDLINCL)
AC_SUBST(LIBLTDL)

AC_CHECK_LIB(intl, gettext)

dnl Checks for libraries.

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h sys/param.h sys/time.h time.h sys/mkdev.h sys/sysmacros.h string.h memory.h fcntl.h dirent.h sys/ndir.h ndir.h alloca.h locale.h )
AC_CHECK_FUNCS(ftruncate gettimeofday)
AC_CHECK_DECLS(getnameinfo, [], [], [[#include <tcpip.h>]])
AC_CHECK_DECLS([getaddrinfo, freeaddrinfo], [], [], [[#include <ws2tcpip.h>]])

AC_HEADER_MAJOR
AC_FUNC_ALLOCA
AC_STRUCT_TM
AC_STRUCT_ST_BLOCKS
AC_FUNC_CLOSEDIR_VOID
AC_CHECK_FUNCS(mkfifo)
AC_CHECK_FUNC(mknod)

dnl Checks for typedefs, structures, and compiler characteristics.

dnl Checks for library functions.

AC_OUTPUT(Makefile src/Makefile src/include/Makefile plibc.lsm plibc.spec)
