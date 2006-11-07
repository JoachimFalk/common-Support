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

SUBDIRS=

pkgincludeprefix=cosupport/


AM_CPPFLAGS=				\
	@AM_CPPFLAGS@			\
	@BOOST_INCLUDE@			\
	@SYSTEMC_INCLUDE@		\
	-I./include			\
	-I$(srcdir)

AM_LDFLAGS=				\
	@AM_LDFLAGS@			\
	@SYSTEMC_LDFLAGS@		\
	@BOOST_LDFLAGS@


EXTRA_DIST	=

pkginclude_HEADERS =			\
	oneof.hpp			\
	stl_output_for_list.hpp		\
	stl_output_for_pair.hpp		\
	refcount.hpp			\
	refcount_object.hpp		\
	intrusive_refcount_ptr.hpp	\
	intrusive_copyonwrite_ptr.hpp	\
	shared_copyonwrite_ptr.hpp	\
	modulo_arith.hpp		\
	arrayfifo.hpp			\
	systemc_support.hpp		\
	functor.hpp                     \
	checked_vector.hpp              \
	string_convert.hpp              \
	iterator_dump.hpp		\
	bit_buffer.hpp			\
					\
	commondefs.h			\
	integertypes.h			\
	sassert.h			\
	snprintf.h			\
					\
	par_actor.hpp			\
	par_actor_factory.hpp		\
	par_manager.hpp			\
	par_port.hpp \
	smoc_ostream.hpp \
	smoc_debug_out.hpp


#noinst_HEADERS=	oneof.hpp

#noinst_SCRIPTS= test_pathstuff.sh

lib_LTLIBRARIES= libcosupport.la
noinst_PROGRAMS=			\
	test_arrayfifo			\
	test_event_and_list		\
	test_event_or_list

compileheader-libcosupport.stamp: $(refcount_CH_LTOBJS) $(refcount_object_CH_LTOBJS)
	@date > $@

libcosupport_la_LIBADD =  $(refcount_CH_LTOBJS) $(refcount_object_CH_LTOBJS)
libcosupport_la_DEPENDENCIES = compileheader-libcosupport.stamp
libcosupport_la_SOURCES =		\
	par_manager.cpp			\
	refcount_object.cpp \
	smoc_debug_out.cpp \
	smoc_ostream.cpp


test_arrayfifo_SOURCES      = test_arrayfifo.cpp
test_arrayfifo_DEPENDENCIES = include

test_event_and_list_DEPENDENCIES  = include
test_event_and_list_LDFLAGS	  = @SYSTEMC_LDFLAGS@ -lsystemc
test_event_and_list_CXXFLAGS	  = @SYSTEMC_INCLUDE@
test_event_and_list_SOURCES	  = test_event_and_list.cpp

test_event_or_list_DEPENDENCIES	  = include
test_event_or_list_LDFLAGS	  = @SYSTEMC_LDFLAGS@ -lsystemc
test_event_or_list_CXXFLAGS	  = @SYSTEMC_INCLUDE@
test_event_or_list_SOURCES	  = test_event_or_list.cpp

all: include
