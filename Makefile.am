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

SUBDIRS=@SUBDIRS@

AUTOMAKE_OPTIONS = nostdinc
# This is only needed if there is a configure.ac in this directory
ACLOCAL_AMFLAGS=-I ../BuildSystem/m4

AM_CPPFLAGS=						\
	@AM_CPPFLAGS@					\
	$(BOOST_INCLUDE)				\
	$(SYSTEMC_INCLUDE)				\
	$(XERCES_INCLUDE)				\
	-I$(builddir)/src/cosupport-base/headers	\
	-I$(srcdir)/src/cosupport-base/headers		\
	-I$(srcdir)/src/cosupport-allocators/headers	\
	-I$(srcdir)/src/cosupport-initializer/headers	\
	-I$(srcdir)/src/cosupport-math/headers		\
	-I$(srcdir)/src/cosupport-path/headers		\
	-I$(srcdir)/src/cosupport-random/headers	\
	-I$(srcdir)/src/cosupport-smartptr/headers	\
	-I$(srcdir)/src/cosupport-streams/headers	\
	-I$(srcdir)/src/cosupport-string/headers	\
	-I$(srcdir)/src/cosupport-systemc/headers	\
	-I$(srcdir)/src/cosupport-tracing/headers	\
	-I$(srcdir)/src/cosupport-xerces/headers

AM_LDFLAGS=				\
	@AM_LDFLAGS@			\
	$(BOOST_LDFLAGS)		\
	$(SYSTEMC_LDFLAGS)		\
	$(XERCES_LDFLAGS)

pkgconfigdir = $(libdir)/pkgconfig

nodist_pkgconfig_DATA =			\
	libcosupport.pc			\
	libcosupport-allocators.pc	\
	libcosupport-base.pc		\
	libcosupport-initializer.pc	\
	libcosupport-math.pc		\
	libcosupport-path.pc		\
	libcosupport-random.pc		\
	libcosupport-smartptr.pc	\
	libcosupport-streams.pc		\
	libcosupport-string.pc
if COSUPPORT_ENABLE_XERCES
nodist_pkgconfig_DATA +=		\
	libcosupport-xerces.pc
endif
if COSUPPORT_ENABLE_SYSTEMC
nodist_pkgconfig_DATA +=		\
	libcosupport-tracing.pc		\
	libcosupport-systemc.pc
endif

%.pc: pkgconfig/%.pc
	@sed -e '/^srctreemode_\w\+\s*=/d'				\
	     -e 's/\$${srctreemode_libdir}\/lib\([^ 	]*\).la/-l\1/g'	\
	     -e 's/\$${srctreemode_\(\w\+\)}/\$${\1}/g'			\
	  < $^ > $@

lib_LTLIBRARIES=			\
	libcosupport-base.la		\
	libcosupport-initializer.la	\
	libcosupport-smartptr.la	\
	libcosupport-streams.la		\
	libcosupport-string.la		\
	libcosupport-math.la		\
	libcosupport-random.la		\
	libcosupport-allocators.la	\
	libcosupport-path.la
if COSUPPORT_ENABLE_XERCES
lib_LTLIBRARIES +=			\
	libcosupport-xerces.la
endif
if COSUPPORT_ENABLE_SYSTEMC
lib_LTLIBRARIES +=			\
	libcosupport-tracing.la		\
	libcosupport-systemc.la
endif

libcosupport_initializer_la_SOURCES =				\
	src/cosupport-initializer/cpp/BasicInitializer.cpp
libcosupport_initializer_la_DEPENDENCIES =			\
	libcosupport-base.la
libcosupport_initializer_la_LDFLAGS = $(AM_LDFLAGS)		\
	-Wl,--no-undefined					\
	$(filter-out %/libcosupport-initializer.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-initializer))

libcosupport_smartptr_la_SOURCES =				\
	src/cosupport-smartptr/cpp/RefCountObject.cpp
libcosupport_smartptr_la_DEPENDENCIES =				\
	libcosupport-base.la
libcosupport_smartptr_la_LDFLAGS = $(AM_LDFLAGS)		\
	-Wl,--no-undefined					\
	$(filter-out %/libcosupport-smartptr.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-smartptr))

if COSUPPORT_ENABLE_XERCES
libcosupport_xerces_la_SOURCES =				\
	src/cosupport-xerces/cpp/common.cpp			\
	src/cosupport-xerces/cpp/Handler.cpp			\
	src/cosupport-xerces/cpp/StdIstreamInputSource.cpp	\
	src/cosupport-xerces/cpp/StdIstreamInputStream.cpp	\
	src/cosupport-xerces/cpp/StdOstreamFormatTarget.cpp
libcosupport_xerces_la_DEPENDENCIES =				\
	libcosupport-base.la					\
	libcosupport-initializer.la				\
	libcosupport-string.la
libcosupport_xerces_la_CPPFLAGS = $(AM_CPPFLAGS)
libcosupport_xerces_la_LDFLAGS = $(AM_LDFLAGS)			\
	-Wl,--no-undefined					\
	$(filter-out %/libcosupport-xerces.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-xerces))
endif

if COSUPPORT_ENABLE_SYSTEMC
libcosupport_systemc_la_SOURCES =				\
	src/cosupport-systemc/cpp/par_manager.cpp		\
	src/cosupport-systemc/cpp/PerformanceEvaluation.cpp	\
	src/cosupport-systemc/cpp/systemc_time.cpp
libcosupport_systemc_la_DEPENDENCIES =				\
	libcosupport-base.la					\
	libcosupport-smartptr.la				\
	libcosupport-string.la
libcosupport_systemc_la_LDFLAGS = $(AM_LDFLAGS)			\
	-Wl,--no-undefined					\
	$(filter-out %/libcosupport-systemc.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-systemc))
endif

libcosupport_streams_la_SOURCES =				\
	src/cosupport-streams/cpp/AlternateStream.cpp		\
	src/cosupport-streams/cpp/DebugOStream.cpp		\
	src/cosupport-streams/cpp/FilterOStream.cpp		\
	src/cosupport-streams/cpp/ColorStreambuf.cpp		\
	src/cosupport-streams/cpp/DebugStreambuf.cpp		\
	src/cosupport-streams/cpp/FilterStreambuf.cpp		\
	src/cosupport-streams/cpp/HeaderFooterStreambuf.cpp	\
	src/cosupport-streams/cpp/IndentStreambuf.cpp		\
	src/cosupport-streams/cpp/LineNumberStreambuf.cpp	\
	src/cosupport-streams/cpp/NullStreambuf.cpp		\
	src/cosupport-streams/cpp/TranslationStreambuf.cpp	\
	src/cosupport-streams/cpp/SocketTCPClient.cpp
libcosupport_streams_la_DEPENDENCIES =				\
	libcosupport-base.la
libcosupport_streams_la_LDFLAGS = $(AM_LDFLAGS)			\
	-Wl,--no-undefined					\
	$(filter-out %/libcosupport-streams.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-streams))

libcosupport_string_la_SOURCES =				\
	src/cosupport-string/cpp/TemplateProcessor.cpp		\
	src/cosupport-string/cpp/color.cpp			\
	src/cosupport-string/cpp/UniquePool.cpp			\
	src/cosupport-string/cpp/UniqueCIdentifierPool.cpp	\
	src/cosupport-string/cpp/DoubleQuotedString.cpp		\
	src/cosupport-string/cpp/identifiermanipulation.cpp	\
	src/cosupport-string/cpp/QuotedString.cpp		\
	src/cosupport-string/cpp/SingleQuotedString.cpp		\
	src/cosupport-string/cpp/UnQuotedString.cpp
libcosupport_string_la_DEPENDENCIES =				\
	libcosupport-base.la
libcosupport_string_la_LDFLAGS = $(AM_LDFLAGS)			\
	-Wl,--no-undefined					\
	$(filter-out %/libcosupport-string.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-string))

libcosupport_math_la_SOURCES =					\
	src/cosupport-math/cpp/flog2.cpp			\
	src/cosupport-math/cpp/Interval.cpp
libcosupport_math_la_DEPENDENCIES =				\
	libcosupport-base.la
libcosupport_math_la_LDFLAGS = $(AM_LDFLAGS)			\
	-Wl,--no-undefined					\
	$(filter-out %/libcosupport-math.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-math))

libcosupport_random_la_SOURCES =				\
	src/cosupport-random/cpp/randomSource.cpp
libcosupport_random_la_DEPENDENCIES =				\
	libcosupport-base.la
libcosupport_random_la_LDFLAGS = $(AM_LDFLAGS)			\
	-Wl,--no-undefined					\
	$(filter-out %/libcosupport-random.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-random))

libcosupport_base_la_SOURCES =
libcosupport_base_la_LDFLAGS = $(AM_LDFLAGS)			\
	-Wl,--no-undefined					\
	$(filter-out %/libcosupport-base.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-base))

libcosupport_allocators_la_SOURCES =
libcosupport_allocators_la_DEPENDENCIES =			\
	libcosupport-base.la					\
	libcosupport-math.la
libcosupport_allocators_la_LDFLAGS = $(AM_LDFLAGS)		\
	-Wl,--no-undefined					\
	$(filter-out %/libcosupport-allocators.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-allocators))

if COSUPPORT_ENABLE_SYSTEMC
libcosupport_tracing_la_SOURCES =				\
	src/cosupport-tracing/cpp/PajeTracer.cpp		\
	src/cosupport-tracing/cpp/PtpTracer.cpp			\
	src/cosupport-tracing/cpp/Tracer.cpp			\
	src/cosupport-tracing/cpp/TracingFactory.cpp
libcosupport_tracing_la_DEPENDENCIES =				\
	libcosupport-base.la					\
	libcosupport-string.la
libcosupport_tracing_la_LDFLAGS = $(AM_LDFLAGS)			\
	-Wl,--no-undefined					\
	$(filter-out %/libcosupport-tracing.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-tracing))
endif

libcosupport_path_la_SOURCES =					\
	src/cosupport-path/cpp/manipulation.cpp			\
	src/cosupport-path/cpp/resourcelocations.cpp
libcosupport_path_la_DEPENDENCIES =				\
	libcosupport-base.la
libcosupport_path_la_LDFLAGS = $(AM_LDFLAGS)			\
	-Wl,--no-undefined					\
	$(filter-out %/libcosupport-path.la, $(shell PKG_CONFIG_PATH="$(PKG_CONFIG_PATH)"; export PKG_CONFIG_PATH; $(PKG_CONFIG) --static --libs libcosupport-path))

CLEANFILES=$(nodist_pkgconfig_DATA)
