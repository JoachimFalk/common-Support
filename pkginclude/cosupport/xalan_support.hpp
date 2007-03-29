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

#ifndef _INCLUDED_XALAN_SUPPORT_HPP
#define _INCLUDED_XALAN_SUPPORT_HPP

#include <iostream>
#include <string>

#include "sassert.h"

#include "xerces_support.hpp"

//#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>

namespace CoSupport { namespace Xerces {
  namespace XN {
    XALAN_CPP_NAMESPACE_USE
  } // namespace XN
} } // namespace CoSupport::Xerces

namespace CoSupport { namespace Xalan {
  using namespace Xerces;

  class XalanInitializer: public XercesInitializer {
  public:
    // Init Xalan C++
    XalanInitializer()
      { XN::XalanTransformer::initialize(); }
    // Done Xalan C++
    ~XalanInitializer() {
      XN::XalanTransformer::terminate();
      XN::XalanTransformer::ICUCleanUp();
    }
  };

  inline
  XN::XalanNode *getAttrNode(const XN::XalanNode *node, const XN::XalanDOMString &attr) {
    XN::XalanNode               *result = NULL;
    const XN::XalanNamedNodeMap *attrs  = node->getAttributes();
    
    if (attrs != NULL)
      result = attrs->getNamedItem(attr);
    if (result == NULL) {
      std::stringstream ss;
      ss << "ERROR: Tag '" << node->getNodeName()
         << "' missing attribute '" << attr << "' !";
      throw std::runtime_error(ss.str().c_str());
    }
    return result;
  }

  inline
  XN::XalanNode *getAttrNode(XN::XalanNode *node, const char *attr)
    { return getAttrNode(node, XN::XalanDOMString(attr)); }

  /// Convert value in node to type T.
  /// This throws an exception if the conversion is invalid.
  template <typename T>
  T getNodeValueAs(const XN::XalanNode *xalanNode);

  template <>
  XN::XalanDOMString getNodeValueAs<XN::XalanDOMString>(const XN::XalanNode *node) {
    return node->getNodeValue();
  }

  template <>
  std::string getNodeValueAs<std::string>(const XN::XalanNode *node) {
    return NStr(getNodeValueAs<XN::XalanDOMString>(node).c_str());
  }

} } // namespace CoSupport::Xalan

#endif // _INCLUDED_XALAN_SUPPORT_HPP
