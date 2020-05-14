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

#ifndef _INCLUDED_COSUPPORT_XML_XERCES_HANDLER_HPP
#define _INCLUDED_COSUPPORT_XML_XERCES_HANDLER_HPP

#include <istream>
#include <ostream>
#include <map>

#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/sax/InputSource.hpp>

#include "common.hpp"

#include "export_config.h"

namespace CoSupport { namespace XML { namespace Xerces {

class COSUPPORT_XERCES_API
Handler: public XercesInitializer {
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

#ifdef _WIN32
  void load(std::wstring const &xmlFileName);
#endif //_WIN32
  void load(XStr const &xmlFileName);
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
