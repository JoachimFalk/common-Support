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

#ifndef _INCLUDED_COSUPPORT_XML_XERCES_STDOSTREAMFORMATTARGET_HPP
#define _INCLUDED_COSUPPORT_XML_XERCES_STDOSTREAMFORMATTARGET_HPP

#include <ostream>
#include <boost/noncopyable.hpp>

#include <xercesc/util/XercesVersion.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

#include "common.hpp"
#include "export_config.h"

namespace CoSupport { namespace XML { namespace Xerces {

class COSUPPORT_XERCES_API
StdOstreamFormatTarget
: public XN::XMLFormatTarget, private boost::noncopyable {
private:
  std::ostream &out;
public:
  StdOstreamFormatTarget(std::ostream &out)
    : out(out) {}

#if XERCES_VERSION_MAJOR == 2
  void writeChars(
      const XMLByte *const toWrite,
      unsigned int count,
      XN::XMLFormatter *const)
    { out.write(reinterpret_cast<const char*>(toWrite), count); }
#elif XERCES_VERSION_MAJOR >= 3
  void writeChars(
      const XMLByte *const toWrite,
      const XMLSize_t count,
      XN::XMLFormatter *const)
    { out.write(reinterpret_cast<const char*>(toWrite), count); }
#endif // XERCES_VERSION_MAJOR >= 3

  void flush()
    { out.flush(); }
};

} } } // namespace CoSupport::XML::Xerces

#endif // _INCLUDED_STDOSTREAMFORMATTARGET_HPP
