#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.57)
m4_define([os2linux_MAJOR],[0])
m4_define([os2linux_MINOR],[2])
m4_define([os2linux_MICRO],[0])
m4_define([os2linux_CURRENT],[0])
m4_define([os2linux_REVISION],[2])
m4_define([os2linux_AGE],[0])
m4_define([os2linux_VERSION],[os2linux_MAJOR.os2linux_MINOR.os2linux_MICRO])
m4_define([os2linux_LTVERSION],[os2linux_CURRENT:os2linux_REVISION:os2linux_AGE])
AC_INIT(os2linux, os2linux_VERSION)
AM_INIT_AUTOMAKE(os2linux, os2linux_VERSION)
OS2LINUX_VERSION=os2linux_VERSION
OS2LINUX_LTVERSION=os2linux_LTVERSION
AC_SUBST(OS2LINUX_VERSION)
AC_SUBST(OS2LINUX_LTVERSION)

AC_CONFIG_SRCDIR([os2/os2.c])
AM_CONFIG_HEADER(config.h)

# Checks for programs.
AC_PROG_CC
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_INSTALL

# Checks for libraries.
AC_CHECK_LIB([dl],[dlopen])
AC_CHECK_LIB([rt],[sem_open])
AC_CHECK_LIB([curses], [initscr])

# Checks for header files.
AC_FUNC_ALLOCA
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([fcntl.h nl_types.h stdlib.h string.h sys/time.h syslog.h unistd.h utime.h ncurses.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_STRUCT_ST_BLOCKS
AC_HEADER_TIME
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_STAT
AC_FUNC_UTIME_NULL
AC_CHECK_FUNCS([ftruncate getcwd gettimeofday localtime_r memset mkdir mkfifo pathconf putenv rmdir stime strchr strspn strstr utime])

AC_OUTPUT(Makefile os2/Makefile include/Makefile test/Makefile os2linux.pc)
