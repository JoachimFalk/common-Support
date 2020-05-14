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

#ifndef _INCLUDED_COSUPPORT_SYSTEMC_CHANNELMODIFICATIONLISTENER_HPP
#define _INCLUDED_COSUPPORT_SYSTEMC_CHANNELMODIFICATIONLISTENER_HPP

#include <cstdlib>
#include <string>

#include "export_config.h"

namespace CoSupport { namespace SystemC {

typedef size_t ChannelId;

class COSUPPORT_SYSTEMC_API
ChannelModificationListener {
public:
  ChannelModificationListener() : readCount(0) {}

  virtual void registerChannel(std::string sourceActor,
                               ChannelId id,
                               std::string name ) const = 0;
  virtual void modified(ChannelId id) const = 0 ;
  virtual ~ChannelModificationListener() {}      
private:
  size_t readCount;
};

} } // namespace CoSupport::SystemC

#endif // _INCLUDED_COSUPPORT_SYSTEMC_CHANNELMODIFICATIONLISTENER_HPP
