// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2020 FAU -- Joachim Falk <joachim.falk@fau.de>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) any
 * later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#ifndef _INCLUDED_COSUPPORT_STREAMS_FILTEROSTREAM_HPP
#define _INCLUDED_COSUPPORT_STREAMS_FILTEROSTREAM_HPP

#include "FilterStreambuf.hpp"

#include "export_config.h"

namespace CoSupport { namespace Streams {

/**
 * convenience class for managing a custom streambuffer
 * chain
 */
class COSUPPORT_STREAMS_API
FilterOStream
: public std::ostream {
public:
  /// construct a new object which uses the streambuffer
  /// of the specified stream as initial target
  FilterOStream(std::ostream &os);
  
  /// inserts a custom streambuffer (as first buffer in
  /// the chain)
  void insert(FilterStreambuf &head);
};

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_FILTER_OSTREAM_HPP
