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

SUBDIRS=

nobase_include_HEADERS = \
	CoSupport/Allocators/IdAllocRequest.hpp \
	CoSupport/Allocators/IdPool.hpp \
	CoSupport/DataTypes/ArrayFifo.hpp \
	CoSupport/DataTypes/CheckedVector.hpp \
	CoSupport/DataTypes/EMPTY.hpp \
	CoSupport/DataTypes/Facade.hpp \
	CoSupport/DataTypes/ListFacade.hpp \
	CoSupport/DataTypes/MaybeValue.hpp \
	CoSupport/DataTypes/NILTYPE.hpp \
	CoSupport/DataTypes/SerializeHelper.hpp \
	CoSupport/DataTypes/ValidWrapper.hpp \
	CoSupport/DataTypes/Value.hpp \
	CoSupport/DataTypes/bit_buffer.hpp \
	CoSupport/DataTypes/container_insert.hpp \
	CoSupport/DataTypes/oneof.hpp \
	CoSupport/Initializer/BasicInitializer.hpp \
	CoSupport/Lambda/function_objects.hpp \
	CoSupport/Lambda/functor.hpp \
	CoSupport/Math/ModuloGroup.hpp \
	CoSupport/Math/flog2.hpp \
	CoSupport/Math/modulo_arith.hpp \
	CoSupport/Math/smoc_math.hpp \
	CoSupport/Math/string_hash.hpp \
	CoSupport/SmartPtr/RefCount.hpp \
	CoSupport/SmartPtr/RefCountObject.hpp \
	CoSupport/SmartPtr/intrusive_copyonwrite_ptr.hpp \
	CoSupport/SmartPtr/intrusive_refcount_ptr.hpp \
	CoSupport/SmartPtr/shared_copyonwrite_ptr.hpp \
	CoSupport/Streams/AlternateStream.hpp \
	CoSupport/Streams/ColorStreambuf.hpp \
	CoSupport/Streams/DebugOStream.hpp \
	CoSupport/Streams/DebugStreambuf.hpp \
	CoSupport/Streams/FilterOStream.hpp \
	CoSupport/Streams/FilterStreambuf.hpp \
	CoSupport/Streams/HeaderFooterStreambuf.hpp \
	CoSupport/Streams/HexOutput.hpp \
	CoSupport/Streams/IndentStreambuf.hpp \
	CoSupport/Streams/LineNumberStreambuf.hpp \
	CoSupport/Streams/NullStreambuf.hpp \
	CoSupport/Streams/SocketTCPClient.hpp \
	CoSupport/Streams/TranslationStreambuf.hpp \
	CoSupport/Streams/iterator_dump.hpp \
	CoSupport/Streams/stl_output_for_list.hpp \
	CoSupport/Streams/stl_output_for_pair.hpp \
	CoSupport/Streams/stl_output_for_vector.hpp \
	CoSupport/String/Concat.hpp \
	CoSupport/String/DoubleQuotedString.hpp \
	CoSupport/String/QuotedString.hpp \
	CoSupport/String/SingleQuotedString.hpp \
	CoSupport/String/TemplateProcessor.hpp \
	CoSupport/String/TypeName.hpp \
	CoSupport/String/UnQuotedString.hpp \
	CoSupport/String/UniquePool.hpp \
	CoSupport/String/convert.hpp \
	CoSupport/Type/STLIteratorSelector.hpp \
	CoSupport/Type/STLPointerSelector.hpp \
	CoSupport/Type/STLReferenceSelector.hpp \
	CoSupport/Type/STLReverseIteratorSelector.hpp \
	CoSupport/Type/constpropagate.hpp \
	CoSupport/Type/transform.hpp \
	CoSupport/commondefs.h \
	CoSupport/compatibility-glue/integertypes.h \
	CoSupport/compatibility-glue/snprintf.h \
	CoSupport/endianess.h \
	CoSupport/sassert.h

if COSUPPORT_ENABLE_XERCES
nobase_include_HEADERS += \
	CoSupport/XML/StdIstreamInputSource.hpp \
	CoSupport/XML/StdIstreamInputStream.hpp \
	CoSupport/XML/StdOstreamFormatTarget.hpp \
	CoSupport/XML/xerces_support.hpp
if COSUPPORT_ENABLE_XALAN
nobase_include_HEADERS += \
	CoSupport/XML/xalan_support.hpp
endif
endif
if COSUPPORT_ENABLE_SYSTEMC
nobase_include_HEADERS += \
	CoSupport/SystemC/ChannelModificationListener.hpp \
	CoSupport/SystemC/PerformanceEvaluation.hpp \
	CoSupport/SystemC/algorithm.hpp \
	CoSupport/SystemC/par_manager.hpp \
	CoSupport/SystemC/systemc_support.hpp \
	CoSupport/SystemC/systemc_time.hpp
if COSUPPORT_ENABLE_XERCES
nobase_include_HEADERS += \
	CoSupport/SMXIdManager.hpp
endif
endif
