// vim: set sw=2 ts=8:
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

#ifndef _INCLUDED_COSUPPORT_XML_XERCES_SUPPORT_HPP
#define _INCLUDED_COSUPPORT_XML_XERCES_SUPPORT_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "../sassert.h"
#include "../String/convert.hpp"
#include "../Initializer/BasicInitializer.hpp"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>

namespace CoSupport {

namespace XML { namespace Xerces {

  namespace XN {
    XERCES_CPP_NAMESPACE_USE
  } // namespace XN

} } // namespace XML::Xerces

namespace Initializer {

  template <>
  struct BasicInitializerTraits<XML::Xerces::XN::XMLPlatformUtils> {
    static void initialize()
      { XML::Xerces::XN::XMLPlatformUtils::Initialize(); }
    static void terminate()
      { XML::Xerces::XN::XMLPlatformUtils::Terminate(); }
  };

} // namespace Initializer

} // namespace CoSupport

namespace CoSupport { namespace XML { namespace Xerces {

  typedef Initializer::BasicInitializer<XN::XMLPlatformUtils> XercesInitializer;

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
    XStr(const std::string &str) {
      resize(str.length() + 1);
      sassert(XN::XMLString::transcode(str.c_str(), &this->operator[](0), size() - 1));
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
    NStr(const XMLCh *const str)
      { fromXMLCh(str); }
#ifdef _INCLUDED_XALAN_SUPPORT_HPP
    NStr(const XN::XalanDOMString &str)
      { fromXMLCh(str.c_str()); }
#endif

    operator const char *() const
      { return c_str(); }
  private:
    void fromXMLCh(const XMLCh *const str) {
      size_type len = XN::XMLString::stringLen(str);
      resize(len + (len >> 1) + 1);
      while (!XN::XMLString::transcode(str, &this->operator[](0), size() - 1))
        { resize(2 * size()); /* double size */ }
      // Cut back to real size
      resize(strlen(c_str()));
    }
  };

  inline
  std::ostream &operator << (std::ostream &out, const XMLCh *const str)
    { out << NStr(str); return out; }

  struct AttrNotFoundError : public std::runtime_error {
    AttrNotFoundError(const std::string& what) :
      std::runtime_error(what)
    {}
  };

  inline
  XN::DOMAttr *getAttrNode(const XN::DOMNode *node, const XMLCh *const attr) {
    XN::DOMAttr               *retval = NULL;
    const XN::DOMNamedNodeMap *attrs  = node->getAttributes();
    
    if (attrs != NULL)
      retval = static_cast<XN::DOMAttr *>(attrs->getNamedItem(attr));
    if (retval == NULL) {
      std::stringstream msg;
      
      msg << "Tag \"" <<  NStr(node->getNodeName()) << "\""
             " has no attribute \"" << NStr(attr) << "\" !";
      throw AttrNotFoundError(msg.str());
    }
    return retval;
  }
  inline
  XN::DOMAttr *getAttrNode(const XN::DOMNode *node, const char *attr)
    { return getAttrNode(node, XStr(attr)); }

  /// get XML attribute node attr of XML node node.
  /// Create the attribute node if it does not exist.
  inline
  XN::DOMAttr *createAttrNode(const XN::DOMNode *node, const XMLCh *const attr) {
    XN::DOMNamedNodeMap *attrs = node->getAttributes();
    assert(attrs != NULL);
    XN::DOMAttr *retval = static_cast<XN::DOMAttr *>(attrs->getNamedItem(attr));
    if (retval == NULL) {
      // Attribute missing => create it
      retval = node->getOwnerDocument()->createAttribute(attr);
      attrs->setNamedItem(retval);
    }
    return retval;
  }
  inline
  XN::DOMAttr *createAttrNode(const XN::DOMNode *node, const char *attr)
    { return createAttrNode(node, XStr(attr)); }

  /// Convert value in node to type T.
  /// This throws an exception if the conversion is invalid.
  template <typename T>
  T getNodeValueAs(const XN::DOMNode *node)
    { return String::strAs<T>(getNodeValueAs<std::string>(node)); }
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

  /// Convert set value in node from type T.
  /// This throws an exception if the conversion is invalid.
  template <typename T>
  void setNodeValueFrom(XN::DOMNode *node, const T &v)
    { setNodeValueFrom<std::string>(node, String::asStr<T>(v)); }
  template <>
  inline
  void setNodeValueFrom<const XMLCh *>(XN::DOMNode *node, const XMLCh *const &v)
    { node->setNodeValue(v); }
  template <>
  inline
  void setNodeValueFrom<XStr>(XN::DOMNode *node, const XStr &v)
    { node->setNodeValue(v); }
  template <>
  inline
  void setNodeValueFrom<std::string>(XN::DOMNode *node, const std::string &v)
    { setNodeValueFrom<XStr>(node, v); }

  template <typename T>
  T getAttrValueAs(const XN::DOMNode *node, const XMLCh *const attr)
    { return getNodeValueAs<T>(getAttrNode(node, attr)); }
  template <typename T>
  T getAttrValueAs(const XN::DOMNode *node, const char *const attr)
    { return getNodeValueAs<T>(getAttrNode(node, attr)); }

  template <typename T>
  void setAttrValueFrom(XN::DOMNode *node, const XMLCh *const attr, const T &v)
    { setNodeValueFrom<T>(createAttrNode(node, attr), v); }
  template <typename T>
  void setAttrValueFrom(XN::DOMNode *node, const char *const attr, const T &v)
    { setNodeValueFrom<T>(createAttrNode(node, attr), v); }

} } } // namespace CoSupport::XML::Xerces

#endif // _INCLUDED_COSUPPORT_XML_XERCES_SUPPORT_HPP
