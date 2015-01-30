// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2015-2015 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_XML_XERCES_HANDLER_HPP
#define _INCLUDED_COSUPPORT_XML_XERCES_HANDLER_HPP

#include <istream>
#include <ostream>
#include <map>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>

#include "common.hpp"

namespace CoSupport { namespace XML { namespace Xerces {

  /**
   * Simple error handler to be installed in a parser
   */
  class DOMErrorHandler
  : public XN::DOMErrorHandler,
    private boost::noncopyable {
  public:
    DOMErrorHandler(std::ostream &outDbg);

    /**
     * Implementation of the DOM ErrorHandler interface
     */
    bool handleError(const XN::DOMError &domError);

    /**
     * Check if an error occurred.
     */
    bool parseFailed()
      { return failed; }
  private:
    std::ostream &outDbg;
    bool          failed;
  };

  class Handler: public XercesInitializer {
  private:
    typedef Handler this_type;

    XN::DOMImplementation         *domImpl; // we don't own this pointer
    ScopedXMLPtr<XN::DOMDocument>  domDocument;
  public:
    Handler();

    void setTopElementName(XStr const &topElementName_)
      { topElementName = topElementName_; }
    XStr getTopElementName() const
      { return topElementName; }

    void setDTDUrl(XStr const &dtdUrl_)
      { dtdUrl = dtdUrl_; }
    XStr getDTDUrl() const
      { return dtdUrl; }
    void setDTD(std::string const &dtdStr_);
    void setDTD(char const *dtdBuf_);
    void setDTD(char const *dtdBuf_, size_t dtdSize_);

    void setXSDUrl(XStr const &xsdUrl_)
      { xsdUrl = xsdUrl_; }
    XStr getXSDUrl() const
      { return xsdUrl; }
    void setXSD(std::string const &xsdStr_);
    void setXSD(char const *xsdBuf_);
    void setXSD(char const *xsdBuf_, size_t xsdSize_);

    void load(std::string const &xmlFileName);
    void load(std::istream &xmlInputStream);
    void createEmpty();

    void save(std::string const &xmlFileName) const;
    void save(std::ostream &xmlOutputStream) const;

    const XN::DOMDocument *getDocument() const;
    XN::DOMDocument *getDocument();

    ~Handler();
  protected:
    XStr topElementName;

    XStr         dtdUrl,   xsdUrl;
    std::string  dtdStr,   xsdStr;
    char const  *dtdBuf,  *xsdBuf;
    size_t       dtdSize,  xsdSize;

    void load(XN::InputSource &in);
    void save(XN::XMLFormatTarget *myFormTarget) const;
  };
  
} } } // namespace CoSupport::XML::Xerces

#endif // _INCLUDED_COSUPPORT_XML_XERCES_HANDLER_HPP
