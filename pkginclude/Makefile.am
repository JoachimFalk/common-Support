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

#all: lndir.stamp
#
#clean-local:
#	@$(RM) lndir.stamp
#	@find . -type l -delete
#	@find . -type d | sort -r | xargs rmdir 2>/dev/null || :
#
#lndir.stamp:
#	lndir -silent $(srcdir) .
#	touch "$@"

nobase_include_HEADERS = \
	CoSupport/Allocators/IdAllocRequest.hpp \
	CoSupport/Allocators/IdPool.hpp \
	CoSupport/boost/graph/graph_traits.hpp \
	CoSupport/boost/graph/undirect_graph.hpp \
	CoSupport/boost/init_namespace.hpp \
	CoSupport/boost/program_options/init_namespace.hpp \
	CoSupport/boost/program_options/value_semantic.hpp \
	CoSupport/boost/python/init_namespace.hpp \
	CoSupport/boost/python/register_listfacade.hpp \
	CoSupport/boost/python/register_setfacade.hpp \
	CoSupport/boost/python/register_tofrom_python_converter_facade.hpp \
	CoSupport/boost/python/register_tofrom_python_converter_maybevalue.hpp \
	CoSupport/commondefs.h \
	CoSupport/compatibility-glue/integertypes.h \
	CoSupport/cosupport_config.h \
	CoSupport/DataTypes/ArrayFifo.hpp \
	CoSupport/DataTypes/bit_buffer.hpp \
	CoSupport/DataTypes/CheckedVector.hpp \
	CoSupport/DataTypes/container_insert.hpp \
	CoSupport/DataTypes/container_retrieve.hpp \
	CoSupport/DataTypes/Iter/BidirectionalTraversalVIf.hpp \
	CoSupport/DataTypes/Iter/Detail/BidirectionalTraversalBase.hpp \
	CoSupport/DataTypes/Iter/Detail/BidirectionalTraversalVFImpl.hpp \
	CoSupport/DataTypes/Iter/Detail/BidirectionalTraversalVImpl.hpp \
	CoSupport/DataTypes/Detail/RandomAccessTraversalIterImpl.hpp \
	CoSupport/DataTypes/Detail/RandomAccessTraversalIterTemplate.hpp \
	CoSupport/DataTypes/EMPTY.hpp \
	CoSupport/DataTypes/Facade.hpp \
	CoSupport/DataTypes/ListInterface.hpp \
	CoSupport/DataTypes/ListVirtual.hpp \
	CoSupport/DataTypes/ListFacade.hpp \
	CoSupport/DataTypes/MaybeValue.hpp \
	CoSupport/DataTypes/MaybeValueInterface.hpp \
	CoSupport/DataTypes/NILTYPE.hpp \
	CoSupport/DataTypes/oneof.hpp \
	CoSupport/DataTypes/Projection.hpp \
	CoSupport/DataTypes/SerializeHelper.hpp \
	CoSupport/DataTypes/SetInterface.hpp \
	CoSupport/DataTypes/ValidWrapper.hpp \
	CoSupport/DataTypes/ValueInterface.hpp \
	CoSupport/DataTypes/Vector.hpp \
	CoSupport/DataTypes/VectorInterface.hpp \
	CoSupport/DataTypes/VectorProjection.hpp \
	CoSupport/endianess.h \
	CoSupport/Initializer/BasicInitializer.hpp \
	CoSupport/Lambda/function_objects.hpp \
	CoSupport/Lambda/functor.hpp \
	CoSupport/Math/flog2.hpp \
	CoSupport/Math/Interval.hpp \
	CoSupport/Math/ModuloGroup.hpp \
	CoSupport/Math/smoc_math.hpp \
	CoSupport/Math/string_hash.hpp \
	CoSupport/Math/Tuple/BinOp.hpp \
	CoSupport/Math/Tuple/exceptions.hpp \
	CoSupport/Math/Tuple/PO.hpp \
	CoSupport/Math/Tuple/POVector.hpp \
	CoSupport/Math/Tuple/ScalarConstant.hpp \
	CoSupport/Math/Tuple/UnOp.hpp \
	CoSupport/Random.hpp \
	CoSupport/Random/randomSource.hpp \
	CoSupport/Random/RandomGenerator.hpp \
	CoSupport/Random/RandomConst.hpp \
	CoSupport/Random/RandomNormal.hpp \
	CoSupport/Random/RandomUniform.hpp \
	CoSupport/Random/RandomUrn.hpp \
	CoSupport/Random/RandomGenerators.hpp \
	CoSupport/sassert.h \
	CoSupport/SmartPtr/intrusive_copyonwrite_ptr.hpp \
	CoSupport/SmartPtr/intrusive_refcount_ptr.hpp \
	CoSupport/SmartPtr/RefCount.hpp \
	CoSupport/SmartPtr/RefCountObject.hpp \
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
	CoSupport/Streams/iterator_dump.hpp \
	CoSupport/Streams/LineNumberStreambuf.hpp \
	CoSupport/Streams/NullStreambuf.hpp \
	CoSupport/Streams/SocketTCPClient.hpp \
	CoSupport/Streams/stl_output_for_list.hpp \
	CoSupport/Streams/stl_output_for_map.hpp \
	CoSupport/Streams/stl_output_for_pair.hpp \
	CoSupport/Streams/stl_output_for_set.hpp \
	CoSupport/Streams/stl_output_for_vector.hpp \
	CoSupport/Streams/TranslationStreambuf.hpp \
	CoSupport/String/Concat.hpp \
	CoSupport/String/convert.hpp \
	CoSupport/String/DoubleQuotedString.hpp \
	CoSupport/String/QuotedString.hpp \
	CoSupport/String/SingleQuotedString.hpp \
	CoSupport/String/TemplateProcessor.hpp \
	CoSupport/String/TypeName.hpp \
	CoSupport/String/UniqueCIdentifierPool.hpp \
	CoSupport/String/UniquePool.hpp \
	CoSupport/String/UnQuotedString.hpp \
	CoSupport/Tracing/PtpTracer.hpp \
	CoSupport/Tracing/TaskTracer.hpp \
	CoSupport/Tracing/Tracer.hpp \
	CoSupport/Tracing/TracingFactory.hpp \
	CoSupport/Type/constpropagate.hpp \
	CoSupport/Type/STLIteratorSelector.hpp \
	CoSupport/Type/STLPointerSelector.hpp \
	CoSupport/Type/STLReferenceSelector.hpp \
	CoSupport/Type/STLReverseIteratorSelector.hpp \
	CoSupport/Type/transform.hpp

if COSUPPORT_ENABLE_XERCES
nobase_include_HEADERS += \
	CoSupport/XML/StdIstreamInputSource.hpp \
	CoSupport/XML/StdIstreamInputStream.hpp \
	CoSupport/XML/StdOstreamFormatTarget.hpp \
	CoSupport/XML/xerces_support.hpp
endif

if COSUPPORT_ENABLE_SYSTEMC
nobase_include_HEADERS += \
	CoSupport/SystemC/algorithm.hpp \
	CoSupport/SystemC/ChannelModificationListener.hpp \
	CoSupport/SystemC/par_manager.hpp \
	CoSupport/SystemC/PerformanceEvaluation.hpp \
	CoSupport/SystemC/systemc_support.hpp \
	CoSupport/SystemC/systemc_time.hpp \
	CoSupport/SystemC/VcdFile.hpp
endif
