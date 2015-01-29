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

#ifndef _INCLUDED_COSUPPORT_XML_XERCES_STDISTREAMINPUTSTREAM_HPP
#define _INCLUDED_COSUPPORT_XML_XERCES_STDISTREAMINPUTSTREAM_HPP

#include "common.hpp"
#include <xercesc/util/BinInputStream.hpp>
#include <istream>
#include <boost/noncopyable.hpp>

#include <xercesc/util/XercesVersion.hpp>

namespace CoSupport { namespace XML { namespace Xerces {

  class StdIstreamInputStream
  : public XN::BinInputStream, private boost::noncopyable {
  private:
    std::istream &in;
  public:
    StdIstreamInputStream(std::istream &in)
      : in(in) {}

#if XERCES_VERSION_MAJOR == 2
    unsigned int curPos() const
      { return in.tellg(); }

    unsigned int readBytes(XMLByte *const toFill, unsigned int maxToRead) {
      in.read(reinterpret_cast<char *>(toFill), maxToRead);
      return in.gcount();
    }
#elif XERCES_VERSION_MAJOR >= 3
    XMLFilePos  curPos() const
      { return in.tellg(); }

    XMLSize_t   readBytes(XMLByte *const toFill, const XMLSize_t maxToRead) {
      in.read(reinterpret_cast<char *>(toFill), maxToRead);
      return in.gcount();
    }

    /*
     * Return the "out-of-band" content type for the data supplied by this input
     * stream in the form of the media-type production (mime type with optional
     * parameters such as encoding) as defined by the HTTP 1.1 specification.
     */
    const XMLCh *getContentType () const;
#endif // XERCES_VERSION_MAJOR >= 3
  };

} } } // namespace CoSupport::XML::Xerces

#endif // _INCLUDED_COSUPPORT_XML_XERCES_STDISTREAMINPUTSTREAM_HPP
