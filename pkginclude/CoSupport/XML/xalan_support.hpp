// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2004-2007 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_XML_XALAN_SUPPORT_HPP
#define _INCLUDED_COSUPPORT_XML_XALAN_SUPPORT_HPP

#ifdef _INCLUDED_COSUPPORT_XML_XERCES_SUPPORT_HPP
# error "Include xalan_support.hpp before xerces_support.hpp !!!"
#endif // _INCLUDED_COSUPPORT_XML_XERCES_SUPPORT_HPP

#include <iostream>
#include <string>

#include "sassert.h"
#include "string_convert.hpp"
#include "../Initializer/BasicInitializer.hpp"

//#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>

namespace CoSupport {

namespace XML { namespace Xerces {

  namespace XN {
    XALAN_CPP_NAMESPACE_USE
  } // namespace XN

} } // namespace XML::Xerces

namespace Initializer {

  template <>
  struct BasicInitializerTraits<Xerces::XN::XalanTransformer> {
    static void initialize()
      { Xerces::XN::XalanTransformer::initialize(); }
    static void terminate() {
      Xerces::XN::XalanTransformer::terminate();
      Xerces::XN::XalanTransformer::ICUCleanUp();
    }
  };

} // namespace Initializer

} // namespace CoSupport

#include "xerces_support.hpp"

namespace CoSupport { namespace XML { namespace Xalan {

  using namespace Xerces;

  class XalanInitializer : public XercesInitializer {
    BasicInitializer<Xalan::XN::XalanTransformer> m_initializer;
  };

  inline
  XN::XalanNode *getAttrNode(const XN::XalanNode *node, const XN::XalanDOMString &attr) {
    XN::XalanNode               *retval = NULL;
    const XN::XalanNamedNodeMap *attrs  = node->getAttributes();
    
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

  /// Convert value in node to type T.
  /// This throws an exception if the conversion is invalid.
  template <typename T>
  T getNodeValueAs(const XN::XalanNode *node)
    { return strAs<T>(getNodeValueAs<std::string>(node)); }
  template <>
  inline
  XN::XalanDOMString getNodeValueAs<XN::XalanDOMString>(const XN::XalanNode *node)
    { return node->getNodeValue(); }
  template <>
  inline
  XStr getNodeValueAs<XStr>(const XN::XalanNode *node)
    { return node->getNodeValue().c_str(); }
  template <>
  inline
  std::string getNodeValueAs<std::string>(const XN::XalanNode *node)
    { return NStr(getNodeValueAs<XN::XalanDOMString>(node).c_str()); }

  template <typename T>
  T getAttrValueAs(const XN::XalanNode *node, const XN::XalanDOMString &attr)
    { return getNodeValueAs<T>(getAttrNode(node, attr)); }
  template <typename T>
  T getAttrValueAs(const XN::XalanNode *node, const char *const attr)
    { return getNodeValueAs<T>(getAttrNode(node, XN::XalanDOMString(attr))); }

} } } // namespace CoSupport::XML::Xalan

#endif // _INCLUDED_COSUPPORT_XML_XALAN_SUPPORT_HPP
