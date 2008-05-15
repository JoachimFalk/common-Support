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
root_srcdir   := $(shell cd $(top_srcdir)/.. && pwd)
root_builddir := $(shell cd $(top_builddir)/.. && pwd)

include $(auxdir)/jfrules.mk
include ../BuildSystem/doxygen.am

SUBDIRS=

# This is only needed if there is a configure.in in this directory
ACLOCAL_AMFLAGS=-I ../BuildSystem/m4

pkgincludedir=$(includedir)/cosupport

AM_CPPFLAGS=				\
	@AM_CPPFLAGS@			\
	@BOOST_INCLUDE@			\
	@SYSTEMC_INCLUDE@		\
	@XALAN_INCLUDE@			\
	@XERCES_INCLUDE@		\
	-I$(srcdir)/pkginclude		\
	-I$(srcdir)

AM_LDFLAGS=				\
	@AM_LDFLAGS@			\
	@BOOST_LDFLAGS@			\
	@SYSTEMC_LDFLAGS@		\
	@XALAN_LDFLAGS@			\
	@XERCES_LDFLAGS@

EXTRA_DIST	=

pkginclude_HEADERS = \
	pkginclude/CoSupport/Streams/HexOutput.hpp \
	pkginclude/CoSupport/Streams/AlternateStream.hpp \
	pkginclude/CoSupport/Streams/stl_output_for_list.hpp \
	pkginclude/CoSupport/Streams/DebugOStream.hpp \
	pkginclude/CoSupport/Streams/stl_output_for_pair.hpp \
	pkginclude/CoSupport/Streams/stl_output_for_vector.hpp \
	pkginclude/CoSupport/Streams/iterator_dump.hpp \
	pkginclude/CoSupport/Streams/FilterStreambuf.hpp \
	pkginclude/CoSupport/Streams/LineNumberStreambuf.hpp \
	pkginclude/CoSupport/Streams/HeaderFooterStreambuf.hpp \
	pkginclude/CoSupport/Streams/ColorStreambuf.hpp \
	pkginclude/CoSupport/Streams/IndentStreambuf.hpp \
	pkginclude/CoSupport/Streams/DebugStreambuf.hpp \
	pkginclude/CoSupport/Streams/NullStreambuf.hpp \
	pkginclude/CoSupport/Streams/FilterOStream.hpp \
	pkginclude/CoSupport/Initializer/BasicInitializer.hpp \
	pkginclude/CoSupport/String/convert.hpp \
	pkginclude/CoSupport/String/TypeName.hpp \
	pkginclude/CoSupport/String/Concat.hpp \
	pkginclude/CoSupport/XML/xalan_support.hpp \
	pkginclude/CoSupport/XML/xerces_support.hpp \
	pkginclude/CoSupport/SMXIdManager.hpp \
	pkginclude/CoSupport/SmartPtr/shared_copyonwrite_ptr.hpp \
	pkginclude/CoSupport/SmartPtr/intrusive_copyonwrite_ptr.hpp \
	pkginclude/CoSupport/SmartPtr/intrusive_refcount_ptr.hpp \
	pkginclude/CoSupport/SmartPtr/RefCount.hpp \
	pkginclude/CoSupport/SmartPtr/RefCountObject.hpp \
	pkginclude/CoSupport/SystemC/systemc_support.hpp \
	pkginclude/CoSupport/SystemC/par_actor_factory.hpp \
	pkginclude/CoSupport/SystemC/par_actor.hpp \
	pkginclude/CoSupport/SystemC/par_manager.hpp \
	pkginclude/CoSupport/SystemC/par_port.hpp \
	pkginclude/CoSupport/SystemC/PerformanceEvaluation.hpp \
	pkginclude/CoSupport/SystemC/ChannelModificationListener.hpp \
	pkginclude/CoSupport/DataTypes/ValidWrapper.hpp \
	pkginclude/CoSupport/DataTypes/checked_vector.hpp \
	pkginclude/CoSupport/DataTypes/bit_buffer.hpp \
	pkginclude/CoSupport/DataTypes/container_insert.hpp \
	pkginclude/CoSupport/DataTypes/ListFacade.hpp \
	pkginclude/CoSupport/DataTypes/oneof.hpp \
	pkginclude/CoSupport/DataTypes/ArrayFifo.hpp \
	pkginclude/CoSupport/DataTypes/Facade.hpp \
	pkginclude/CoSupport/Math/modulo_arith.hpp \
	pkginclude/CoSupport/Math/smoc_math.hpp \
	pkginclude/CoSupport/Math/string_hash.hpp \
	pkginclude/CoSupport/Type/transform.hpp \
	pkginclude/CoSupport/Lambda/functor.hpp \
	pkginclude/CoSupport/Lambda/function_objects.hpp

#noinst_HEADERS=	oneof.hpp

#noinst_SCRIPTS= test_pathstuff.sh

lib_LTLIBRARIES=			\
	libcosupport.la			\
	libcosupport-initializer.la	\
	libcosupport-smartptr.la	\
	libcosupport-xerces.la		\
	libcosupport-systemc.la		\
	libcosupport-streams.la
noinst_PROGRAMS=			\
	test_arrayfifo			\
	test_event_and_list		\
	test_event_or_list		\
	test_nested_event_lists

libcosupport_la_SOURCES =
libcosupport_la_LIBADD =			\
	libcosupport-initializer.la		\
	libcosupport-smartptr.la		\
	libcosupport-xerces.la			\
	libcosupport-systemc.la			\
	libcosupport-streams.la

libcosupport_initializer_la_LDFLAGS =		\
	$(AM_LDFLAGS)				\
	-lboost_thread@BOOST_LIBMTPOSTFIX@
libcosupport_initializer_la_SOURCES =		\
	BasicInitializer.cpp

libcosupport_smartptr_la_LDFLAGS =		\
	$(AM_LDFLAGS)				\
	-lboost_thread@BOOST_LIBMTPOSTFIX@
libcosupport_smartptr_la_LIBADD =		\
	$(refcount_CH_LTOBJS)			\
	$(refcount_object_CH_LTOBJS)
libcosupport_smartptr_la_SOURCES =		\
	RefCountObject.cpp

libcosupport_xerces_la_LDFLAGS =		\
	$(AM_LDFLAGS)				\
	-lboost_thread@BOOST_LIBMTPOSTFIX@
libcosupport_xerces_la_LIBADD =			\
	libcosupport-initializer.la
libcosupport_xerces_la_SOURCES =		\
	SMXIdManager.cpp

libcosupport_systemc_la_LDFLAGS =		\
	$(AM_LDFLAGS)				\
	-lboost_thread@BOOST_LIBMTPOSTFIX@	\
	-lsystemc
libcosupport_systemc_la_SOURCES =		\
	par_manager.cpp				\
	PerformanceEvaluation.cpp

libcosupport_streams_la_LDFLAGS =		\
	$(AM_LDFLAGS)				\
	-lboost_thread@BOOST_LIBMTPOSTFIX@
libcosupport_streams_la_SOURCES =		\
	DebugOStream.cpp			\
	FilterOStream.cpp

test_arrayfifo_SOURCES		  = test_arrayfifo.cpp

test_event_and_list_LDADD	  = libcosupport-systemc.la
test_event_and_list_SOURCES	  = test_event_and_list.cpp

test_event_or_list_LDADD	  = libcosupport-systemc.la
test_event_or_list_SOURCES	  = test_event_or_list.cpp

test_nested_event_lists_LDADD	  = libcosupport-systemc.la libcosupport-streams.la
test_nested_event_lists_SOURCES	  = test_nested_event_lists.cpp

# autotest script is build by some source file. #! line is generated by
#  automake to guarantee proper path. Other scripting languages are easy
#  possible, too (e.g., Perl).
noinst_SCRIPTS=autotest.sh

CLEANFILES=$(noinst_SCRIPTS)

autotest.sh: test_event_and_list $(srcdir)/autotest-sh
	rm -f $@
	echo "#!" $(BASH) > $@
	cat autovars.sh $(srcdir)/autotest-sh >> $@
	chmod +x $@

# to check we just call the 'autotest.sh' script
check: autotest.sh
	./autotest.sh
