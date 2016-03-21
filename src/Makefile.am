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
	cosupport-allocators/headers/CoSupport/Allocators/IdAllocRequest.hpp \
	cosupport-allocators/headers/CoSupport/Allocators/IdPool.hpp \
	cosupport-base/headers/CoSupport/boost/graph/graph_traits.hpp \
	cosupport-base/headers/CoSupport/boost/graph/properties.hpp \
	cosupport-base/headers/CoSupport/boost/graph/undirect_graph.hpp \
	cosupport-base/headers/CoSupport/boost/init_namespace.hpp \
	cosupport-base/headers/CoSupport/boost/pending/property.hpp \
	cosupport-base/headers/CoSupport/boost/program_options/init_namespace.hpp \
	cosupport-base/headers/CoSupport/boost/program_options/value_semantic.hpp \
	cosupport-base/headers/CoSupport/boost/property_map/compose_property_map.hpp \
	cosupport-base/headers/CoSupport/boost/python/init_namespace.hpp \
	cosupport-base/headers/CoSupport/boost/python/register_listfacade.hpp \
	cosupport-base/headers/CoSupport/boost/python/register_setfacade.hpp \
	cosupport-base/headers/CoSupport/boost/python/register_tofrom_python_converter_facade.hpp \
	cosupport-base/headers/CoSupport/boost/python/register_tofrom_python_converter_maybevalue.hpp \
	cosupport-base/headers/CoSupport/boost/python/register_tofrom_python_converter_stdlist.hpp \
	cosupport-base/headers/CoSupport/commondefs.h \
	cosupport-base/headers/CoSupport/compatibility-glue/integertypes.h \
	cosupport-base/headers/CoSupport/compatibility-glue/nullptr.h \
	cosupport-base/headers/CoSupport/cosupport_config.h \
	cosupport-base/headers/CoSupport/DataTypes/ArrayFifo.hpp \
	cosupport-base/headers/CoSupport/DataTypes/bit_buffer.hpp \
	cosupport-base/headers/CoSupport/DataTypes/CheckedVector.hpp \
	cosupport-base/headers/CoSupport/DataTypes/container_insert.hpp \
	cosupport-base/headers/CoSupport/DataTypes/container_retrieve.hpp \
	cosupport-base/headers/CoSupport/DataTypes/Detail/RandomAccessTraversalIterImpl.hpp \
	cosupport-base/headers/CoSupport/DataTypes/Detail/RandomAccessTraversalIterTemplate.hpp \
	cosupport-base/headers/CoSupport/DataTypes/EMPTY.hpp \
	cosupport-base/headers/CoSupport/DataTypes/Facade.hpp \
	cosupport-base/headers/CoSupport/DataTypes/Iter/BidirectionalTraversalExampleImpl.hpp \
	cosupport-base/headers/CoSupport/DataTypes/Iter/BidirectionalTraversalVIf.hpp \
	cosupport-base/headers/CoSupport/DataTypes/Iter/Detail/BidirectionalTraversalBase.hpp \
	cosupport-base/headers/CoSupport/DataTypes/Iter/Detail/BidirectionalTraversalVFImpl.hpp \
	cosupport-base/headers/CoSupport/DataTypes/Iter/Detail/BidirectionalTraversalVImpl.hpp \
	cosupport-base/headers/CoSupport/DataTypes/ListFacade.hpp \
	cosupport-base/headers/CoSupport/DataTypes/ListInterface.hpp \
	cosupport-base/headers/CoSupport/DataTypes/ListVirtual.hpp \
	cosupport-base/headers/CoSupport/DataTypes/MapFacade.hpp \
	cosupport-base/headers/CoSupport/DataTypes/MapInterface.hpp \
	cosupport-base/headers/CoSupport/DataTypes/MapVirtual.hpp \
	cosupport-base/headers/CoSupport/DataTypes/MaybeValueFacade.hpp \
	cosupport-base/headers/CoSupport/DataTypes/MaybeValue.hpp \
	cosupport-base/headers/CoSupport/DataTypes/MaybeValueInterface.hpp \
	cosupport-base/headers/CoSupport/DataTypes/MaybeValueVirtual.hpp \
	cosupport-base/headers/CoSupport/DataTypes/NILTYPE.hpp \
	cosupport-base/headers/CoSupport/DataTypes/oneof.hpp \
	cosupport-base/headers/CoSupport/DataTypes/Projection.hpp \
	cosupport-base/headers/CoSupport/DataTypes/SerializeHelper.hpp \
	cosupport-base/headers/CoSupport/DataTypes/SetFacade.hpp \
	cosupport-base/headers/CoSupport/DataTypes/SetInterface.hpp \
	cosupport-base/headers/CoSupport/DataTypes/SetVirtual.hpp \
	cosupport-base/headers/CoSupport/DataTypes/ValidWrapper.hpp \
	cosupport-base/headers/CoSupport/DataTypes/ValueFacade.hpp \
	cosupport-base/headers/CoSupport/DataTypes/ValueInterface.hpp \
	cosupport-base/headers/CoSupport/DataTypes/ValueVirtual.hpp \
	cosupport-base/headers/CoSupport/DataTypes/Vector.hpp \
	cosupport-base/headers/CoSupport/DataTypes/VectorInterface.hpp \
	cosupport-base/headers/CoSupport/DataTypes/VectorProjection.hpp \
	cosupport-base/headers/CoSupport/endianess.h \
	cosupport-base/headers/CoSupport/Lambda/functor.hpp \
	cosupport-base/headers/CoSupport/limits.h \
	cosupport-base/headers/CoSupport/sassert.h \
	cosupport-base/headers/CoSupport/Type/constpropagate.hpp \
	cosupport-base/headers/CoSupport/Type/STLIteratorSelector.hpp \
	cosupport-base/headers/CoSupport/Type/STLPointerSelector.hpp \
	cosupport-base/headers/CoSupport/Type/STLReferenceSelector.hpp \
	cosupport-base/headers/CoSupport/Type/STLReverseIteratorSelector.hpp \
	cosupport-initializer/headers/CoSupport/Initializer/BasicInitializer.hpp \
	cosupport-initializer/headers/CoSupport/Initializer/export_config.h \
	cosupport-math/headers/CoSupport/Math/export_config.h \
	cosupport-math/headers/CoSupport/Math/flog2.hpp \
	cosupport-math/headers/CoSupport/Math/Interval.hpp \
	cosupport-math/headers/CoSupport/Math/ModuloGroup.hpp \
	cosupport-math/headers/CoSupport/Math/modulo.hpp \
	cosupport-math/headers/CoSupport/Math/string_hash.hpp \
	cosupport-math/headers/CoSupport/Math/Tuple/BinOp.hpp \
	cosupport-math/headers/CoSupport/Math/Tuple/PO.hpp \
	cosupport-math/headers/CoSupport/Math/Tuple/POVector.hpp \
	cosupport-math/headers/CoSupport/Math/Tuple/ScalarConstant.hpp \
	cosupport-math/headers/CoSupport/Math/Tuple/UnOp.hpp \
	cosupport-path/headers/CoSupport/Path/export_config.h \
	cosupport-path/headers/CoSupport/Path/manipulation.hpp \
	cosupport-path/headers/CoSupport/Path/resourcelocations.hpp \
	cosupport-random/headers/CoSupport/Random/export_config.h \
	cosupport-random/headers/CoSupport/Random.hpp \
	cosupport-random/headers/CoSupport/Random/RandomConst.hpp \
	cosupport-random/headers/CoSupport/Random/RandomGenerator.hpp \
	cosupport-random/headers/CoSupport/Random/RandomGenerators.hpp \
	cosupport-random/headers/CoSupport/Random/RandomNormal.hpp \
	cosupport-random/headers/CoSupport/Random/randomSource.hpp \
	cosupport-random/headers/CoSupport/Random/RandomUniform.hpp \
	cosupport-random/headers/CoSupport/Random/RandomUrn.hpp \
	cosupport-smartptr/headers/CoSupport/SmartPtr/export_config.h \
	cosupport-smartptr/headers/CoSupport/SmartPtr/intrusive_copyonwrite_ptr.hpp \
	cosupport-smartptr/headers/CoSupport/SmartPtr/intrusive_refcount_ptr.hpp \
	cosupport-smartptr/headers/CoSupport/SmartPtr/RefCount.hpp \
	cosupport-smartptr/headers/CoSupport/SmartPtr/RefCountObject.hpp \
	cosupport-smartptr/headers/CoSupport/SmartPtr/shared_copyonwrite_ptr.hpp \
	cosupport-streams/headers/CoSupport/Streams/AlternateStream.hpp \
	cosupport-streams/headers/CoSupport/Streams/ColorStreambuf.hpp \
	cosupport-streams/headers/CoSupport/Streams/DebugOStream.hpp \
	cosupport-streams/headers/CoSupport/Streams/DebugStreambuf.hpp \
	cosupport-streams/headers/CoSupport/Streams/export_config.h \
	cosupport-streams/headers/CoSupport/Streams/FilterOStream.hpp \
	cosupport-streams/headers/CoSupport/Streams/FilterStreambuf.hpp \
	cosupport-streams/headers/CoSupport/Streams/HeaderFooterStreambuf.hpp \
	cosupport-streams/headers/CoSupport/Streams/HexOutput.hpp \
	cosupport-streams/headers/CoSupport/Streams/IndentStreambuf.hpp \
	cosupport-streams/headers/CoSupport/Streams/iterator_dump.hpp \
	cosupport-streams/headers/CoSupport/Streams/LineNumberStreambuf.hpp \
	cosupport-streams/headers/CoSupport/Streams/NullStreambuf.hpp \
	cosupport-streams/headers/CoSupport/Streams/SocketTCPClient.hpp \
	cosupport-streams/headers/CoSupport/Streams/stl_output_for_list.hpp \
	cosupport-streams/headers/CoSupport/Streams/stl_output_for_map.hpp \
	cosupport-streams/headers/CoSupport/Streams/stl_output_for_pair.hpp \
	cosupport-streams/headers/CoSupport/Streams/stl_output_for_set.hpp \
	cosupport-streams/headers/CoSupport/Streams/stl_output_for_vector.hpp \
	cosupport-streams/headers/CoSupport/Streams/TranslationStreambuf.hpp \
	cosupport-string/headers/CoSupport/String/Concat.hpp \
	cosupport-string/headers/CoSupport/String/convert.hpp \
	cosupport-string/headers/CoSupport/String/DoubleQuotedString.hpp \
	cosupport-string/headers/CoSupport/String/export_config.h \
	cosupport-string/headers/CoSupport/String/identifiermanipulation.hpp \
	cosupport-string/headers/CoSupport/String/QuotedString.hpp \
	cosupport-string/headers/CoSupport/String/SingleQuotedString.hpp \
	cosupport-string/headers/CoSupport/String/TemplateProcessor.hpp \
	cosupport-string/headers/CoSupport/String/TypeName.hpp \
	cosupport-string/headers/CoSupport/String/UniqueCIdentifierPool.hpp \
	cosupport-string/headers/CoSupport/String/UniquePool.hpp \
	cosupport-string/headers/CoSupport/String/UnQuotedString.hpp \
	cosupport-tracing/headers/CoSupport/Tracing/export_config.h \
	cosupport-tracing/headers/CoSupport/Tracing/PtpTracer.hpp \
	cosupport-tracing/headers/CoSupport/Tracing/TaskTracer.hpp \
	cosupport-tracing/headers/CoSupport/Tracing/Tracer.hpp \
	cosupport-tracing/headers/CoSupport/Tracing/TracingFactory.hpp

if COSUPPORT_ENABLE_XERCES
nobase_include_HEADERS += \
	cosupport-xerces/headers/CoSupport/XML/Xerces/common.hpp \
	cosupport-xerces/headers/CoSupport/XML/Xerces/export_config.h \
	cosupport-xerces/headers/CoSupport/XML/Xerces/Handler.hpp \
	cosupport-xerces/headers/CoSupport/XML/Xerces/StdIstreamInputSource.hpp \
	cosupport-xerces/headers/CoSupport/XML/Xerces/StdIstreamInputStream.hpp \
	cosupport-xerces/headers/CoSupport/XML/Xerces/StdOstreamFormatTarget.hpp \
	cosupport-xerces/headers/CoSupport/XML/xerces_support.hpp
endif

if COSUPPORT_ENABLE_SYSTEMC
nobase_include_HEADERS += \
	cosupport-systemc/headers/CoSupport/SystemC/algorithm.hpp \
	cosupport-systemc/headers/CoSupport/SystemC/ChannelModificationListener.hpp \
	cosupport-systemc/headers/CoSupport/SystemC/export_config.h \
	cosupport-systemc/headers/CoSupport/SystemC/par_manager.hpp \
	cosupport-systemc/headers/CoSupport/SystemC/PerformanceEvaluation.hpp \
	cosupport-systemc/headers/CoSupport/SystemC/systemc_support.hpp \
	cosupport-systemc/headers/CoSupport/SystemC/systemc_time.hpp
endif
