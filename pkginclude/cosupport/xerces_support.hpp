/*
 * Copyright (c) 2004-2006 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_XERCES_SUPPORT_HPP
#define _INCLUDED_XERCES_SUPPORT_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "sassert.h"
#include "string_convert.hpp"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>

namespace CoSupport { namespace Xerces {

  namespace XN {
    XERCES_CPP_NAMESPACE_USE 
  }

  class XercesInitializer {
  public:
    // Init Xerces C++
    XercesInitializer()
      { XN::XMLPlatformUtils::Initialize(); }
    // Done Xerces C++
    ~XercesInitializer()
      { XN::XMLPlatformUtils::Terminate(); }
  };

  class XStr: public std::basic_string<XMLCh> {
  public:
    XStr(const XMLCh *const str)
      : std::basic_string<XMLCh>(str) {}
    XStr(const char  *const str) {
      resize(strlen(str) + 1);
      sassert(XN::XMLString::transcode(str, &this->operator[](0), size() - 1));
      // Cut back to real size
      resize(XN::XMLString::stringLen(c_str()));
    }

    operator const XMLCh *() const
      { return c_str(); }
  };

  class NStr: public std::basic_string<char> {
  public:
    NStr(const char  *const str)
      :  std::basic_string<char>(str) {}
    NStr(const XMLCh *const str) {
      size_type len = XN::XMLString::stringLen(str);
      resize(len + (len >> 1) + 1);
      while (!XN::XMLString::transcode(str, &this->operator[](0), size() - 1))
        { resize(2 * size()); /* double size */ }
      // Cut back to real size
      resize(strlen(c_str()));
    }

    operator const char *() const
      { return c_str(); }
  };

  inline
  std::ostream &operator << (std::ostream &out, const XMLCh *const str)
    { out << NStr(str); return out; }

  inline
  XN::DOMNode *getAttrNode(const XN::DOMNode *node, const XMLCh *const attr) {
    XN::DOMNode               *retval = NULL;
    const XN::DOMNamedNodeMap *attrs  = node->getAttributes();
    
    if (attrs != NULL)
      retval = attrs->getNamedItem(attr);
    if (retval == NULL) {
      std::stringstream msg;
      
      msg << "Tag \"" << node->getNodeName() << "\""
              " has no attribute \"" << attr << "\" !";
      throw std::runtime_error(msg.str());
    }
    return retval;
  }

  inline
  XN::DOMNode *getAttrNode(const XN::DOMNode *node, const char *attr)
    { return getAttrNode(node, XStr(attr)); }

  /// Convert value in node to type T.
  /// This throws an exception if the conversion is invalid.
  template <typename T>
  T getNodeValueAs(const XN::DOMNode *node)
    { return strAs<T>(getNodeValueAs<std::string>(node)); }
  template <>
  inline
  const XMLCh *getNodeValueAs<const XMLCh *>(const XN::DOMNode *node)
    { return node->getNodeValue(); }
  template <>
  inline
  XStr getNodeValueAs<XStr>(const XN::DOMNode *node)
    { return node->getNodeValue(); }
  template <>
  inline
  std::string getNodeValueAs<std::string>(const XN::DOMNode *node)
    { return NStr(getNodeValueAs<const XMLCh *>(node)); }

  template <typename T>
  T getAttrValueAs(const XN::DOMNode *node, const XMLCh *const attr)
    { return getNodeValueAs<T>(getAttrNode(node, attr)); }
  template <typename T>
  T getAttrValueAs(const XN::DOMNode *node, const char *const attr)
    { return getNodeValueAs<T>(getAttrNode(node, attr)); }

} } // namespace CoSupport::Xerces

#endif // _INCLUDED_XERCES_SUPPORT_HPP
