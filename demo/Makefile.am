## Process this file with automake to produce Makefile.in

INCLUDES = \
	-I$(top_srcdir)/src \
	-I$(top_builddir)/src \
	@PACKAGE_CFLAGS@

#	-DG_DISABLE_DEPRECATED -DPANGO_DISABLE_DEPRECATED \
#	-DGDK_DISABLE_DEPRECATED -DGDK_PIXBUF_DISABLE_DEPRECATED \
#	-DGTK_DISABLE_DEPRECATED

noinst_PROGRAMS = demo table-demo generic-position-demo simple-demo scalability-demo units-demo widgets-demo mv-demo mv-table-demo mv-generic-position-demo mv-simple-demo mv-scalability-demo

demo_SOURCES = \
	demo.c demo-fifteen.c demo-scalability.c demo-grabs.c \
	demo-arrowhead.c demo-features.c demo-events.c \
	demo-paths.c demo-focus.c demo-item.h demo-item.c demo-animation.c \
	demo-clipping.c demo-table.c demo-large-line.h demo-large-line.c \
	demo-large-items.c demo-large-rect.h demo-large-rect.c

demo_LDADD = $(top_builddir)/src/libgoocanvas.la @PACKAGE_LIBS@ $(INTLLIBS) -lm

mv_demo_SOURCES = \
	mv-demo.c mv-demo-fifteen.c mv-demo-scalability.c mv-demo-grabs.c \
	mv-demo-arrowhead.c mv-demo-features.c mv-demo-events.c \
	mv-demo-paths.c mv-demo-focus.c mv-demo-animation.c \
	mv-demo-clipping.c mv-demo-table.c

mv_demo_LDADD = $(top_builddir)/src/libgoocanvas.la @PACKAGE_LIBS@ $(INTLLIBS) -lm

table_demo_SOURCES = \
	table-demo.c

table_demo_LDADD = $(top_builddir)/src/libgoocanvas.la @PACKAGE_LIBS@ $(INTLLIBS)

mv_table_demo_SOURCES = \
	mv-table-demo.c

mv_table_demo_LDADD = $(top_builddir)/src/libgoocanvas.la @PACKAGE_LIBS@ $(INTLLIBS)

simple_demo_SOURCES = \
	simple-demo.c

simple_demo_LDADD = $(top_builddir)/src/libgoocanvas.la @PACKAGE_LIBS@ $(INTLLIBS)

mv_simple_demo_SOURCES = \
	mv-simple-demo.c

mv_simple_demo_LDADD = $(top_builddir)/src/libgoocanvas.la @PACKAGE_LIBS@ $(INTLLIBS)

scalability_demo_SOURCES = \
	scalability-demo.c

scalability_demo_LDADD = $(top_builddir)/src/libgoocanvas.la @PACKAGE_LIBS@ $(INTLLIBS)

mv_scalability_demo_SOURCES = \
	mv-scalability-demo.c

mv_scalability_demo_LDADD = $(top_builddir)/src/libgoocanvas.la @PACKAGE_LIBS@ $(INTLLIBS)

units_demo_SOURCES = \
	units-demo.c

units_demo_LDADD = $(top_builddir)/src/libgoocanvas.la @PACKAGE_LIBS@ $(INTLLIBS)

widgets_demo_SOURCES = \
	widgets-demo.c

widgets_demo_LDADD = $(top_builddir)/src/libgoocanvas.la @PACKAGE_LIBS@ $(INTLLIBS)

generic_position_demo_SOURCES = \
	generic-position-demo.c

generic_position_demo_LDADD = $(top_builddir)/src/libgoocanvas.la @PACKAGE_LIBS@ $(INTLLIBS)

mv_generic_position_demo_SOURCES = \
	mv-generic-position-demo.c

mv_generic_position_demo_LDADD = $(top_builddir)/src/libgoocanvas.la @PACKAGE_LIBS@ $(INTLLIBS)

EXTRA_DIST = flower.png toroid.png

