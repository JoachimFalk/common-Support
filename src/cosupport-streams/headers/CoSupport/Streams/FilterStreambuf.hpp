// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_STREAMS_FILTERSTREAMBUF_HPP
#define _INCLUDED_COSUPPORT_STREAMS_FILTERSTREAMBUF_HPP

#include <streambuf>
#include <ostream>

#include "export_config.h"

namespace CoSupport { namespace Streams {

/// forward declarations 
class FilterOStream;

/**
 * the base class for all custom streambuffers
 */  
class COSUPPORT_STREAMS_API
FilterStreambuf
: public std::streambuf {
protected:
  /// pointer to the next streambuffer in the buffer chain
  std::streambuf *next;
  
  /// must be able to set next
  friend class FilterOStream;

  /// See std::streambuf
  virtual int sync();

public:  
  /// constructs a new object, optionally with a target
  /// streambuffer
  FilterStreambuf(std::streambuf *next = 0);

  /// Sets a new target for this buffer
  void setTarget(std::streambuf *os);

public:
  /// should be reimplemented in derived classes: return true
  /// if stream manipulators are available
  virtual bool hasManip() const;
  
  /// returns the index for the ostream extensible array
  /// (obtained with std::ostream::xalloc())
  virtual int getIndex() const;
  
  /// virtual destructor
  virtual ~FilterStreambuf();
};

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_FILTERSTREAMBUF_HPP
