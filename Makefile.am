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

LIBCOSUPPORT_INITIALIZER_LIBS   := $(shell PKG_CONFIG_PATH=$(builddir)/pkgconfig $(PKG_CONFIG) libcosupport-initializer --libs --static)
LIBCOSUPPORT_INITIALIZER_CFLAGS := $(shell PKG_CONFIG_PATH=$(builddir)/pkgconfig $(PKG_CONFIG) libcosupport-initializer --cflags)

dummy:
	echo "LIBCOSUPPORT_INITIALIZER_LIBS:   $(LIBCOSUPPORT_INITIALIZER_LIBS)"
	echo "LIBCOSUPPORT_INITIALIZER_CFLAGS: $(LIBCOSUPPORT_INITIALIZER_CFLAGS)"

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

%.pc: pkgconfig/%.pc
	@sed -e 's@^\(prefix\s*=\).*@\1$(prefix)@'		\
	     -e 's@^\(exec_prefix\s*=\).*@\1$(exec_prefix)@'	\
	     -e 's@^\(libdir\s*=\).*@\1$(libdir)@'		\
	     -e 's@^\(includedir\s*=\).*@\1$(includedir)@'	\
	  < $^ > $@

clean-local: clean-pkgconfig

clean-pkgconfig:
	@rm -f $(nodist_pkgconfig_DATA)

pkgconfigdir = $(libdir)/pkgconfig

nodist_pkgconfig_DATA =			\
	libcosupport.pc			\
	libcosupport-initializer.pc	\
	libcosupport-smartptr.pc	\
	libcosupport-streams.pc		\
	libcosupport-string.pc		\
	libcosupport-math.pc		\
	libcosupport-allocators.pc	\
	libcosupport-tracing.pc

lib_LTLIBRARIES=			\
	libcosupport.la			\
	libcosupport-initializer.la	\
	libcosupport-smartptr.la	\
	libcosupport-streams.la		\
	libcosupport-string.la		\
	libcosupport-math.la		\
	libcosupport-allocators.la	\
	libcosupport-tracing.la

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
	test_vector			\
	test_projection			\
	test_po

libcosupport_la_SOURCES =
libcosupport_la_LIBADD =			\
	libcosupport-initializer.la		\
	libcosupport-smartptr.la		\
	libcosupport-streams.la			\
	libcosupport-string.la			\
	libcosupport-allocators.la		\
	libcosupport-tracing.la

if COSUPPORT_ENABLE_XERCES
nodist_pkgconfig_DATA +=			\
	libcosupport-xerces.pc
libcosupport_la_LIBADD +=			\
	libcosupport-xerces.la
lib_LTLIBRARIES +=				\
	libcosupport-xerces.la
endif
if COSUPPORT_ENABLE_XALAN
nodist_pkgconfig_DATA +=			\
	libcosupport-xalan.pc
libcosupport_la_LIBADD +=			\
	libcosupport-xalan.la
lib_LTLIBRARIES +=				\
	libcosupport-xalan.la
endif
if COSUPPORT_ENABLE_SYSTEMC
nodist_pkgconfig_DATA +=			\
	libcosupport-systemc.pc
libcosupport_la_LIBADD +=			\
	libcosupport-systemc.la
lib_LTLIBRARIES +=				\
	libcosupport-systemc.la
noinst_PROGRAMS +=				\
	test_event_and_list			\
	test_event_or_list			\
	test_nested_event_lists
endif

libcosupport_initializer_la_SOURCES =		\
	BasicInitializer.cpp
libcosupport_initializer_la_LIBADD =		\
	-lboost_thread$(BOOST_LIBMTPOSTFIX)

libcosupport_smartptr_la_SOURCES =		\
	RefCountObject.cpp
libcosupport_smartptr_la_LIBADD =		\
	$(refcount_CH_LTOBJS)			\
	$(refcount_object_CH_LTOBJS)		\
	-lboost_thread$(BOOST_LIBMTPOSTFIX)

if COSUPPORT_ENABLE_XERCES
libcosupport_xerces_la_SOURCES =		\
	SMXIdManager.cpp			\
	StdIstreamInputSource.cpp		\
	StdIstreamInputStream.cpp		\
	StdOstreamFormatTarget.cpp
libcosupport_xerces_la_LIBADD =			\
	-lboost_thread$(BOOST_LIBMTPOSTFIX)	\
	libcosupport-initializer.la		\
	-lxerces-c $(XERCES_AUXLIBS)
endif

if COSUPPORT_ENABLE_XALAN
libcosupport_xalan_la_SOURCES =
libcosupport_xalan_la_LIBADD =			\
	-lboost_thread$(BOOST_LIBMTPOSTFIX)	\
	libcosupport-initializer.la		\
	-lxalan-c
endif

if COSUPPORT_ENABLE_SYSTEMC
libcosupport_systemc_la_SOURCES =		\
	par_manager.cpp				\
	PerformanceEvaluation.cpp
libcosupport_systemc_la_LDFLAGS=		\
	$(AM_LDFLAGS)
libcosupport_systemc_la_LIBADD =		\
	-lboost_thread$(BOOST_LIBMTPOSTFIX)	\
	-lsystemc

test_event_and_list_SOURCES	  = test_event_and_list.cpp
test_event_and_list_LDADD	  = libcosupport-systemc.la

test_event_or_list_SOURCES	  = test_event_or_list.cpp
test_event_or_list_LDADD	  = libcosupport-systemc.la

test_nested_event_lists_SOURCES	  = test_nested_event_lists.cpp
test_nested_event_lists_LDADD	  = libcosupport-systemc.la libcosupport-streams.la
endif

libcosupport_streams_la_SOURCES =		\
	DebugOStream.cpp			\
	FilterOStream.cpp			\
	ColorStreambuf.cpp			\
	DebugStreambuf.cpp			\
	FilterStreambuf.cpp			\
	HeaderFooterStreambuf.cpp		\
	IndentStreambuf.cpp			\
	LineNumberStreambuf.cpp			\
	NullStreambuf.cpp			\
	TranslationStreambuf.cpp		\
	SocketTCPClient.cpp
libcosupport_streams_la_LIBADD =		\
	-lboost_iostreams$(BOOST_LIBMTPOSTFIX)	\
	-lboost_system$(BOOST_LIBMTPOSTFIX)

libcosupport_string_la_SOURCES =		\
	TemplateProcessor.cpp			\
	UniquePool.cpp				\
	UniqueCIdentifierPool.cpp		\
	DoubleQuotedString.cpp			\
	QuotedString.cpp			\
	SingleQuotedString.cpp			\
	UnQuotedString.cpp

libcosupport_string_la_LIBADD =			\
	-lboost_regex$(BOOST_LIBMTPOSTFIX)

libcosupport_math_la_SOURCES =			\
	flog2.cpp
libcosupport_math_la_LIBADD =

libcosupport_allocators_la_SOURCES =
libcosupport_allocators_la_LIBADD =

libcosupport_tracing_la_SOURCES =		\
	PtpTracer.cpp				\
	Tracer.cpp				\
	TracingFactory.cpp

test_dbgstream_SOURCES	= test_dbgstream.cpp
test_dbgstream_LDADD	= libcosupport-streams.la

test_maybevalue_SOURCES	= test_maybevalue.cpp
test_maybevalue_LDADD	=

test_arrayfifo_SOURCES	= test_arrayfifo.cpp
test_arrayfifo_LDADD	=

test_interval_SOURCES	= test_interval.cpp
test_interval_LDADD	=

test_idpool_SOURCES	= test_idpool.cpp
test_idpool_LDADD	= libcosupport-allocators.la

test_stl_output_for_xxx_SOURCES	= test_stl_output_for_xxx.cpp
test_stl_output_for_xxx_LDADD	=

test_facade_SOURCES	= test_facade.cpp test_facade_lib.cpp
test_facade_LDADD	= libcosupport-smartptr.la

test_typename_SOURCES	= test_typename.cpp
test_typename_LDADD	= libcosupport-string.la

test_value_SOURCES	= test_value.cpp

test_list_SOURCES	= test_list.cpp

test_vector_SOURCES	= test_vector.cpp

test_projection_SOURCES	= test_projection.cpp

test_po_SOURCES		= test_po.cpp

# autotest script is build by some source file. #! line is generated by
#  automake to guarantee proper path. Other scripting languages are easy
#  possible, too (e.g., Perl).
noinst_SCRIPTS=autotest.sh

CLEANFILES=$(noinst_SCRIPTS)

autotest.sh: $(srcdir)/autotest-sh
	rm -f $@
	echo "#!" $(BASH) > $@
	cat autovars.sh $(srcdir)/autotest-sh >> $@
	chmod +x $@

# to check we just call the 'autotest.sh' script
check: autotest.sh $(noinst_PROGRAMS)
	./autotest.sh
