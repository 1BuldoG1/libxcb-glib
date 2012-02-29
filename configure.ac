AC_PREREQ([2.65])
AC_INIT([libxcb-glib],[0.1],[],[])

AC_CONFIG_HEADERS([include/config.h])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([.aux])

AM_INIT_AUTOMAKE([1.11 subdir-objects dist-xz no-dist-gzip tar-ustar foreign])

AM_SILENT_RULES([yes])

AC_PROG_CC
AC_PROG_CC_C99
AM_PROG_CC_C_O
AC_DISABLE_STATIC
LT_INIT

GETTEXT_PACKAGE=$PACKAGE_NAME
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Gettext Package])
AC_SUBST(GETTEXT_PACKAGE)
dnl Not yet useful
dnl IT_PROG_INTLTOOL([0.40.0])

AC_PROG_INSTALL

glib_min_version="2.28.0"

AM_CFLAGS="-Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers"
AM_LDFLAGS=""

AC_DEFINE([LIBXCB_GLIB_COMPILATION], [1], [Defined during libxcb-glib compilation])
AC_DEFINE([G_LOG_DOMAIN], ["GXcb"], [GLib log domain])


AC_ARG_ENABLE(git-version, AS_HELP_STRING([--disable-git-version], [Do not append git version information to the version string]), enable_git_version=$enableval, enable_git_version=yes)
if test x$enable_git_version = xyes; then
    AC_DEFINE_UNQUOTED([PACKAGE_VERSION], ["${PACKAGE_VERSION} − `git describe --tags --always`, branch `git describe --tags --always --all | sed -e 's:heads/::'`"], [Full version with git information])
fi

PKG_CHECK_MODULES(GLIB, [glib-2.0 >= $glib_min_version])
PKG_CHECK_MODULES(XCB, [xcb])

# Debug flags
AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug], [Enable debug]), enable_debug=$enableval, enable_debug=no)
if test x$enable_debug = xyes; then
    AC_DEFINE([DEBUG], [1], [Activate debug mode])
    AM_CFLAGS="${AM_CFLAGS} -O0 -ggdb3"
fi
AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_LDFLAGS)

AC_CONFIG_FILES([
    data/libxcb-glib.pc
    Makefile
])
AC_OUTPUT
