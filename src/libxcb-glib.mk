LIBXCB_GLIB_CURRENT=0
LIBXCB_GLIB_REVISION=0
LIBXCB_GLIB_AGE=0

lib_LTLIBRARIES += \
	libxcb-glib.la

pkginclude_HEADERS += \
	include/libxcb-glib.h

libxcb_glib_la_SOURCES = \
	src/libxcb-glib-source.c

libxcb_glib_la_CFLAGS = \
	$(AM_CFLAGS) \
	$(XCB_CFLAGS) \
	$(GLIB_CFLAGS)

libxcb_glib_la_LDFLAGS = \
	$(AM_LDFLAGS) \
	-version-info $(LIBXCB_GLIB_CURRENT):$(LIBXCB_GLIB_REVISION):$(LIBXCB_GLIB_AGE) \
	-export-dynamic

libxcb_glib_la_LIBADD = \
	$(XCB_LIBS) \
	$(GLIB_LIBS)

pkgconfig_DATA += \
	$(top_builddir)/data/libxcb-glib.pc
