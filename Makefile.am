## Process this file with automake to produce Makefile.in
# vim: set noet sw=2 ts=8 sts=0:
#
# Copyright (C) Joachim Falk <joachim.falk@gmx.de> $Date: 2003/01/15 16:43:20 $
#
# Makefile.am is part of the jflib distribution of Joachim Falk;
# you can redistribute it and/or modify it under the terms of the
# GNU General Public License as published by the Free Software Foundation;
# either version 2 of the License, or (at your option) any later version.
#
# The jflib distributio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public
# License along with this program; If not, write to
# the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# $Log$

include $(auxdir)/jfrules.mk
include ../BuildSystem/doxygen.am

SUBDIRS=pkginclude .

# This is only needed if there is a configure.in in this directory
ACLOCAL_AMFLAGS=-I ../BuildSystem/m4

AM_CPPFLAGS=				\
	@AM_CPPFLAGS@			\
	$(BOOST_INCLUDE)		\
	$(SYSTEMC_INCLUDE)		\
	$(XERCES_INCLUDE)		\
	-I$(builddir)/pkginclude	\
	-I$(srcdir)/pkginclude

AM_LDFLAGS=				\
	@AM_LDFLAGS@			\
	$(BOOST_LDFLAGS)		\
	$(SYSTEMC_LDFLAGS)		\
	$(XERCES_LDFLAGS)

pkgconfigdir = $(libdir)/pkgconfig

nodist_pkgconfig_DATA =			\
	libcosupport.pc			\
	libcosupport-allocators.pc	\
	libcosupport-initializer.pc	\
	libcosupport-math.pc		\
	libcosupport-path.pc		\
	libcosupport-random.pc		\
	libcosupport-smartptr.pc	\
	libcosupport-streams.pc		\
	libcosupport-string.pc		\
	libcosupport-tracing.pc
if COSUPPORT_ENABLE_XERCES
nodist_pkgconfig_DATA +=		\
	libcosupport-xerces.pc
endif
if COSUPPORT_ENABLE_SYSTEMC
nodist_pkgconfig_DATA +=		\
	libcosupport-systemc.pc
endif

%.pc: pkgconfig/%.pc
	@sed -e '/^srctreemode_\w\+\s*=/d'				\
	     -e 's/\$${srctreemode_libdir}\/lib\([^ 	]*\).la/-l\1/g'	\
	     -e 's/\$${srctreemode_\(\w\+\)}/\$${\1}/g'			\
	  < $^ > $@

lib_LTLIBRARIES=			\
	libcosupport-initializer.la	\
	libcosupport-smartptr.la	\
	libcosupport-streams.la		\
	libcosupport-string.la		\
	libcosupport-math.la		\
	libcosupport-random.la		\
	libcosupport-allocators.la	\
	libcosupport-tracing.la		\
	libcosupport-path.la
if COSUPPORT_ENABLE_XERCES
lib_LTLIBRARIES +=			\
	libcosupport-xerces.la
endif
if COSUPPORT_ENABLE_SYSTEMC
lib_LTLIBRARIES +=			\
	libcosupport-systemc.la
endif

noinst_PROGRAMS=			\
	test_dbgstream			\
	test_maybevalue			\
	test_arrayfifo			\
	test_interval			\
	test_idpool			\
	test_stl_output_for_xxx		\
	test_facade			\
	test_typename			\
	test_value			\
	test_list			\
	test_set			\
	test_vector			\
	test_modulo			\
	test_projection			\
	test_po				\
	test_path
if COSUPPORT_ENABLE_SYSTEMC
noinst_PROGRAMS +=			\
	test_event_and_list		\
	test_event_or_list		\
	test_nested_event_lists
endif

libcosupport_initializer_la_SOURCES =			\
	implementation/BasicInitializer.cpp
libcosupport_initializer_la_LDFLAGS = $(AM_LDFLAGS)	\
	$(filter-out %/libcosupport-initializer.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-initializer))

libcosupport_smartptr_la_SOURCES =			\
	implementation/RefCountObject.cpp
libcosupport_smartptr_la_LDFLAGS = $(AM_LDFLAGS)	\
	$(filter-out %/libcosupport-smartptr.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-smartptr))

if COSUPPORT_ENABLE_XERCES
libcosupport_xerces_la_SOURCES =				\
	implementation/XML/Xerces/StdIstreamInputSource.cpp	\
	implementation/XML/Xerces/StdIstreamInputStream.cpp	\
	implementation/XML/Xerces/StdOstreamFormatTarget.cpp	\
	implementation/XML/Xerces/Handler.cpp
libcosupport_xerces_la_DEPENDENCIES =				\
	libcosupport-initializer.la
libcosupport_xerces_la_CPPFLAGS = $(AM_CPPFLAGS) -std=gnu++0x
libcosupport_xerces_la_LDFLAGS = $(AM_LDFLAGS)			\
	$(filter-out %/libcosupport-xerces.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-xerces))
endif

if COSUPPORT_ENABLE_SYSTEMC
libcosupport_systemc_la_SOURCES =			\
	implementation/par_manager.cpp			\
	implementation/PerformanceEvaluation.cpp
libcosupport_systemc_la_LDFLAGS = $(AM_LDFLAGS)		\
	$(filter-out %/libcosupport-systemc.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-systemc))

test_event_and_list_SOURCES		= test_event_and_list.cpp
test_event_and_list_DEPENDENCIES	= libcosupport-systemc.la
test_event_and_list_LDFLAGS		= $(AM_LDFLAGS)	\
	$(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --libs libcosupport-systemc)

test_event_or_list_SOURCES		= test_event_or_list.cpp
test_event_or_list_DEPENDENCIES		= libcosupport-systemc.la
test_event_or_list_LDFLAGS		= $(AM_LDFLAGS)	\
	$(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --libs libcosupport-systemc)

test_nested_event_lists_SOURCES		= test_nested_event_lists.cpp
test_nested_event_lists_DEPENDENCIES	= libcosupport-systemc.la libcosupport-streams.la
test_nested_event_lists_LDFLAGS		= $(AM_LDFLAGS)	\
	$(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --libs libcosupport-systemc libcosupport-streams)
endif

libcosupport_streams_la_SOURCES =			\
	implementation/DebugOStream.cpp			\
	implementation/FilterOStream.cpp		\
	implementation/ColorStreambuf.cpp		\
	implementation/DebugStreambuf.cpp		\
	implementation/FilterStreambuf.cpp		\
	implementation/HeaderFooterStreambuf.cpp	\
	implementation/IndentStreambuf.cpp		\
	implementation/LineNumberStreambuf.cpp		\
	implementation/NullStreambuf.cpp		\
	implementation/TranslationStreambuf.cpp		\
	implementation/SocketTCPClient.cpp
libcosupport_streams_la_LDFLAGS = $(AM_LDFLAGS)		\
	$(filter-out %/libcosupport-streams.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-streams))

libcosupport_string_la_SOURCES =			\
	implementation/TemplateProcessor.cpp		\
	implementation/UniquePool.cpp			\
	implementation/UniqueCIdentifierPool.cpp	\
	implementation/DoubleQuotedString.cpp		\
	implementation/identifiermanipulation.cpp	\
	implementation/QuotedString.cpp			\
	implementation/SingleQuotedString.cpp		\
	implementation/UnQuotedString.cpp
libcosupport_string_la_LDFLAGS = $(AM_LDFLAGS)		\
	$(filter-out %/libcosupport-string.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-string))

libcosupport_math_la_SOURCES =				\
	implementation/flog2.cpp
libcosupport_math_la_LDFLAGS = $(AM_LDFLAGS)		\
	$(filter-out %/libcosupport-math.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-math))

libcosupport_random_la_SOURCES =			\
	implementation/Random/randomSource.cpp
libcosupport_random_la_LDFLAGS = $(AM_LDFLAGS)		\
	$(filter-out %/libcosupport-random.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-random))

libcosupport_allocators_la_SOURCES =
libcosupport_allocators_la_LDFLAGS = $(AM_LDFLAGS)	\
	$(filter-out %/libcosupport-allocators.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-allocators))

libcosupport_tracing_la_SOURCES =			\
	implementation/PtpTracer.cpp			\
	implementation/Tracer.cpp			\
	implementation/TracingFactory.cpp
libcosupport_tracing_la_LDFLAGS = $(AM_LDFLAGS)		\
	$(filter-out %/libcosupport-tracing.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-tracing))

libcosupport_path_la_SOURCES =				\
	implementation/Path/manipulation.cpp		\
	implementation/Path/resourcelocations.cpp
libcosupport_path_la_LDFLAGS = $(AM_LDFLAGS)		\
	$(filter-out %/libcosupport-path.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-path))

test_dbgstream_SOURCES		= test_dbgstream.cpp
test_dbgstream_DEPENDENCIES	= libcosupport-streams.la
test_dbgstream_LDFLAGS		= $(AM_LDFLAGS)		\
	$(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --libs libcosupport-streams)

test_maybevalue_SOURCES		= test_maybevalue.cpp

test_arrayfifo_SOURCES		= test_arrayfifo.cpp

test_interval_SOURCES		= test_interval.cpp

test_idpool_SOURCES		= test_idpool.cpp
test_idpool_DEPENDENCIES	= libcosupport-allocators.la
test_idpool_LDFLAGS		= $(AM_LDFLAGS)		\
	$(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --libs libcosupport-allocators)

test_stl_output_for_xxx_SOURCES	= test_stl_output_for_xxx.cpp

test_facade_SOURCES		= test_facade.cpp test_facade_lib.cpp
test_facade_DEPENDENCIES	= libcosupport-smartptr.la
test_facade_LDFLAGS		= $(AM_LDFLAGS)		\
	$(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --libs libcosupport-smartptr)

test_typename_SOURCES		= test_typename.cpp
test_typename_DEPENDENCIES	= libcosupport-string.la
test_typename_LDFLAGS		= $(AM_LDFLAGS)		\
	$(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --libs libcosupport-string)

test_value_SOURCES		= test_value.cpp

test_list_SOURCES		= test_list.cpp

test_set_SOURCES		= test_set.cpp

test_vector_SOURCES		= test_vector.cpp

test_modulo_SOURCES		= test_modulo.cpp

test_projection_SOURCES		= test_projection.cpp

test_po_SOURCES			= test_po.cpp

test_path_SOURCES		= test_path.cpp
test_path_DEPENDENCIES		= libcosupport-path.la
test_path_LDFLAGS		= $(AM_LDFLAGS)			\
	$(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --libs libcosupport-path)

# autotest script is build by some source file. #! line is generated by
#  automake to guarantee proper path. Other scripting languages are easy
#  possible, too (e.g., Perl).
noinst_SCRIPTS=autotest.sh

CLEANFILES=$(noinst_SCRIPTS) $(nodist_pkgconfig_DATA)

autotest.sh: $(srcdir)/autotest-sh
	rm -f $@
	echo "#!" $(BASH) > $@
	cat autovars.sh $(srcdir)/autotest-sh >> $@
	chmod +x $@

# to check we just call the 'autotest.sh' script
check: autotest.sh $(noinst_PROGRAMS)
	./autotest.sh
