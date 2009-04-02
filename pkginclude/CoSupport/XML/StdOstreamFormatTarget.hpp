// vim: set sw=2 sts=2 ts=8 et syn=cpp:
/*
 * Copyright (c) 2004-2008 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
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

#include "xerces_support.hpp"
#include <xercesc/framework/XMLFormatter.hpp>
#include <ostream>

namespace CoSupport { namespace XML { namespace Xerces {

  class StdOstreamFormatTarget : public XN::XMLFormatTarget {
  private:
    std::ostream& out;
  public:
    StdOstreamFormatTarget(std::ostream& out);
    virtual void writeChars(
        const XMLByte* const toWrite,
        const unsigned int count,
        XN::XMLFormatter* const);
    virtual void flush();
  private:
    StdOstreamFormatTarget(const StdOstreamFormatTarget&);
    StdOstreamFormatTarget& operator=(const StdOstreamFormatTarget&);
  };

}}} // namespace CoSupport::XML::Xerces

#endif // _INCLUDED_STDOSTREAMFORMATTARGET_HPP