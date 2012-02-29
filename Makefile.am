AM_DISTCHECK_CONFIGURE_FLAGS = \
	--disable-debug \
	--disable-git-version

ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
AM_MAKEFLAGS = --no-print-directory

pkgconfigdir = $(libdir)/pkgconfig

lib_LTLIBRARIES =
pkginclude_HEADERS =
pkgconfig_DATA =

EXTRA_DIST =
CLEANFILES =

AM_CFLAGS += \
	-D LOCALEDIR=\"$(localedir)\" \
	--include config.h \
	-I $(top_srcdir)/include/ \
	-I $(top_builddir)/include/

include src/libxcb-glib.mk
