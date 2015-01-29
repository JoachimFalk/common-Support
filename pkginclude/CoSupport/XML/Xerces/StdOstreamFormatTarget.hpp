// vim: set sw=2 sts=2 ts=8 et syn=cpp:
/*
 * Copyright (c) 2004-2009 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
 * 
 *   This library is free software; you can redistribute it and/or modify it under
 *   the terms of the GNU Lesser General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option) any
 *   later version.
 * 
 *   This library is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 *   details.
 * 
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software Foundation, Inc.,
 *   59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 * 
 * --- This software and any associated documentation is provided "as is" 
 * 
 * IN NO EVENT SHALL HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG
 * BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN
 * NUREMBERG HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG, SPECIFICALLY
 * DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
 * HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF
 * ERLANGEN NUREMBERG HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 */

#ifndef _INCLUDED_COSUPPORT_XML_XERCES_STDOSTREAMFORMATTARGET_HPP
#define _INCLUDED_COSUPPORT_XML_XERCES_STDOSTREAMFORMATTARGET_HPP

#include "common.hpp"
#include <ostream>
#include <boost/noncopyable.hpp>

#include <xercesc/util/XercesVersion.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

namespace CoSupport { namespace XML { namespace Xerces {

  class StdOstreamFormatTarget
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
