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

#ifndef _INCLUDED_COSUPPORT_XML_XERCES_COMMON_HPP
#define _INCLUDED_COSUPPORT_XML_XERCES_COMMON_HPP

#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <memory>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#else
#include <alloca.h>
#endif

#include <CoSupport/compatibility-glue/nullptr.h>
#include <CoSupport/sassert.h>
#include <CoSupport/String/convert.hpp>
#include <CoSupport/Initializer/BasicInitializer.hpp>
#include <CoSupport/DataTypes/MaybeValue.hpp>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>

#include <boost/noncopyable.hpp>
#include <boost/static_assert.hpp>

#include "export_config.h"

namespace CoSupport { namespace Initializer {

  template <> struct COSUPPORT_XERCES_API
  BasicInitializerTraits<XERCES_CPP_NAMESPACE_QUALIFIER XMLPlatformUtils> {
    static void initialize();
    static void terminate();
  };

} } // namespace CoSupport::Initializer

namespace CoSupport { namespace XML { namespace Xerces {

  namespace XN {
    XERCES_CPP_NAMESPACE_USE
  } // namespace XN

  typedef Initializer::BasicInitializer<XN::XMLPlatformUtils> XercesInitializer;

  // this stuff is stolen and adapted from boost::scoped_ptr

  //  ScopedXMLPtr mimics a built-in pointer except that it guarantees deletion
  //  of the object pointed to, either on destruction of the ScopedXMLPtr or via
  //  an explicit reset(). ScopedXMLPtr is a simple solution for simple needs;

  template <class T>
  class ScopedXMLPtr: private boost::noncopyable {
    typedef ScopedXMLPtr<T> this_type;
  private:
    typedef T *this_type::*unspecified_bool_type;
  private:
    T *ptr;
  public:
    typedef T element_type;

    // never throws
    explicit ScopedXMLPtr(T *p = nullptr): ptr(p)
      {}

    ~ScopedXMLPtr() {
      // don't use delete use the release() method all xerces nodes have.
      if (ptr != nullptr)
        ptr->release();
    }

    // never throws
    void swap(this_type &b) {
      T *tmp = b.ptr;
      b.ptr  = ptr;
      ptr    = tmp;
    }

    // never throws
    void reset(T *p = nullptr) {
      assert(p == nullptr || p != ptr); // catch self-reset errors
      this_type(p).swap(*this);
    }

    // never throws
    T &operator *() const {
      assert(ptr != 0);
      return *ptr;
    }

    // never throws
    T *operator ->() const {
      assert(ptr != 0);
      return ptr;
    }

    // never throws
    T *get() const {
      return ptr;
    }

    // implicit conversion to "bool", never throws
    operator unspecified_bool_type() const {
      return ptr == nullptr ? nullptr: &this_type::ptr;
    }

    // never throws
    bool operator !() const  {
      return ptr == nullptr;
    }
  };

  // never throws
  template <class T>
  inline void swap(ScopedXMLPtr<T> &a, ScopedXMLPtr<T> &b) {
    a.swap(b);
  }

  // get_pointer(p) is a generic way to say p.get()
  template <class T>
  inline T *get_pointer(ScopedXMLPtr<T> const &p) {
    return p.get();
  }

#if defined _MSC_VER
# define XMLCH(x) static_cast<XMLCh const *>(L##x)
#elif defined __linux
# define XMLCH(x) reinterpret_cast<XMLCh const *>(static_cast<char16_t const *>(u##x))
#else
# error "No support for XMLCH macro"
#endif

  class COSUPPORT_XERCES_API
  XStr: public std::basic_string<XMLCh> {
  public:
    XStr()
      : std::basic_string<XMLCh>() {}
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

    inline
    operator std::string () const;

    operator const XMLCh *() const
      { return c_str(); }
  };

  // Fixup comparison operators.
  static inline
  bool operator ==(const XStr &lhs, const XStr &rhs) {
    return
      static_cast<const std::basic_string<XMLCh> &>(lhs) ==
      static_cast<const std::basic_string<XMLCh> &>(rhs);
  }
  static inline
  bool operator ==(const XMLCh *const lhs, const XStr &rhs) { 
    return lhs == static_cast<const std::basic_string<XMLCh> &>(rhs);
  }
  static inline
  bool operator ==(const XStr &lhs, const XMLCh *const rhs) { 
    return static_cast<const std::basic_string<XMLCh> &>(lhs) == rhs;
  }

  static inline
  bool operator !=(const XStr &lhs, const XStr &rhs) {
    return
      static_cast<const std::basic_string<XMLCh> &>(lhs) !=
      static_cast<const std::basic_string<XMLCh> &>(rhs);
  }
  static inline
  bool operator !=(const XMLCh *const lhs, const XStr &rhs) { 
    return lhs != static_cast<const std::basic_string<XMLCh> &>(rhs);
  }
  static inline
  bool operator !=(const XStr &lhs, const XMLCh *const rhs) { 
    return static_cast<const std::basic_string<XMLCh> &>(lhs) != rhs;
  }

  static inline
  bool operator < (const XStr &lhs, const XStr &rhs) {
    return
      static_cast<const std::basic_string<XMLCh> &>(lhs) <
      static_cast<const std::basic_string<XMLCh> &>(rhs);
  }
  static inline
  bool operator < (const XMLCh *const lhs, const XStr &rhs) { 
    return lhs <  static_cast<const std::basic_string<XMLCh> &>(rhs);
  }
  static inline
  bool operator < (const XStr &lhs, const XMLCh *const rhs) { 
    return static_cast<const std::basic_string<XMLCh> &>(lhs) <  rhs;
  }

  static inline
  bool operator <=(const XStr &lhs, const XStr &rhs) {
    return
      static_cast<const std::basic_string<XMLCh> &>(lhs) <=
      static_cast<const std::basic_string<XMLCh> &>(rhs);
  }
  static inline
  bool operator <=(const XMLCh *const lhs, const XStr &rhs) { 
    return lhs <= static_cast<const std::basic_string<XMLCh> &>(rhs);
  }
  static inline
  bool operator <=(const XStr &lhs, const XMLCh *const rhs) { 
    return static_cast<const std::basic_string<XMLCh> &>(lhs) <= rhs;
  }

  static inline
  bool operator > (const XStr &lhs, const XStr &rhs) {
    return
      static_cast<const std::basic_string<XMLCh> &>(lhs) >
      static_cast<const std::basic_string<XMLCh> &>(rhs);
  }
  static inline
  bool operator > (const XMLCh *const lhs, const XStr &rhs) { 
    return lhs >  static_cast<const std::basic_string<XMLCh> &>(rhs);
  }
  static inline
  bool operator > (const XStr &lhs, const XMLCh *const rhs) { 
    return static_cast<const std::basic_string<XMLCh> &>(lhs) >  rhs;
  }

  static inline
  bool operator >=(const XStr &lhs, const XStr &rhs) {
    return
      static_cast<const std::basic_string<XMLCh> &>(lhs) >=
      static_cast<const std::basic_string<XMLCh> &>(rhs);
  }
  static inline
  bool operator >=(const XMLCh *const lhs, const XStr &rhs) { 
    return lhs >= static_cast<const std::basic_string<XMLCh> &>(rhs);
  }
  static inline
  bool operator >=(const XStr &lhs, const XMLCh *const rhs) { 
    return static_cast<const std::basic_string<XMLCh> &>(lhs) >= rhs;
  }

  class COSUPPORT_XERCES_API
  NStr: public std::basic_string<char> {
  public:
    NStr(const char  *const str)
      :  std::basic_string<char>(str) {}
    NStr(const XMLCh *const str)
      { fromXMLCh(str); }

    operator const char *() const
      { return c_str(); }
  private:
    void fromXMLCh(const XMLCh *const str) {
      size_t i;
      size_t len = XN::XMLString::stringLen(str)+1;
      len += len >> 1;
      char  *buf = reinterpret_cast<char *>(alloca(len));
      // ASCII
      for (i = 0; str[i] != 0 && str[i] <= 127; ++i)
        buf[i] = str[i];
      if (str[i] == 0)
        buf[i] = 0;
      else
        // FALLBACK TRANSCODE
        while (!XN::XMLString::transcode(str, buf, len-1)) {
          len += len;
          buf = reinterpret_cast<char *>(alloca(len));
        }
      *this = buf;
    }
  };

  COSUPPORT_XERCES_API
  inline
  XStr::operator std::string () const
    { return NStr(c_str()); }

  COSUPPORT_XERCES_API
  inline
  std::ostream &operator << (std::ostream &out, const XMLCh *const str)
    { out << NStr(str); return out; }

  struct COSUPPORT_XERCES_API
  AttrNotFoundError : public std::runtime_error {
    AttrNotFoundError(std::string const &what) :
      std::runtime_error(what)
    {}
  };

  COSUPPORT_XERCES_API
  inline
  XN::DOMAttr *getMaybeAttrNode(XN::DOMNode const *const node, XMLCh const *const attr) {
    assert(node != nullptr || !"WTF?! Missing node!");
    assert(
        node->getNodeType() == XN::DOMNode::ELEMENT_NODE ||
        node->getNodeType() == XN::DOMNode::DOCUMENT_NODE);
    const XN::DOMNamedNodeMap *attrs  = node->getAttributes();
    return attrs == nullptr
      ? nullptr
      : static_cast<XN::DOMAttr *>(attrs->getNamedItem(attr));
  }

  COSUPPORT_XERCES_API
  inline
  XN::DOMAttr *getMaybeAttrNode(XN::DOMNode const *const node, char const *const attr)
    { return getMaybeAttrNode(node, XStr(attr).c_str()); }

  COSUPPORT_XERCES_API
  inline
  XN::DOMAttr *getAttrNode(XN::DOMNode const *const node, XMLCh const *const attr) {
    XN::DOMAttr *retval = getMaybeAttrNode(node, attr);
    if (retval == nullptr) {
      std::stringstream msg;

      msg << "Tag \"" <<  NStr(node->getNodeName()) << "\""
             " has no attribute \"" << NStr(attr) << "\"!";
      throw AttrNotFoundError(msg.str());
    }
    return retval;
  }

  COSUPPORT_XERCES_API
  inline
  XN::DOMAttr *getAttrNode(XN::DOMNode const *const node, char const *const attr)
    { return getAttrNode(node, XStr(attr).c_str()); }

  /// get XML attribute node attr of XML node node.
  /// Create the attribute node if it does not exist.
  COSUPPORT_XERCES_API
  inline
  XN::DOMAttr *createAttrNode(XN::DOMNode *const node, XMLCh const *const attr) {
    assert(node != nullptr || !"WTF?! Missing node!");
    assert(
        node->getNodeType() == XN::DOMNode::ELEMENT_NODE ||
        node->getNodeType() == XN::DOMNode::DOCUMENT_NODE);
    XN::DOMNamedNodeMap *attrs = node->getAttributes();
    assert(attrs != nullptr);
    XN::DOMAttr *retval = static_cast<XN::DOMAttr *>(attrs->getNamedItem(attr));
    if (retval == nullptr) {
      // Attribute missing => create it
      retval = node->getOwnerDocument()->createAttribute(attr);
      attrs->setNamedItem(retval);
    }
    return retval;
  }

  COSUPPORT_XERCES_API
  inline
  XN::DOMAttr *createAttrNode(XN::DOMNode *const node, char const *const attr)
    { return createAttrNode(node, XStr(attr).c_str()); }

  /// destroy XML attribute node attr of XML node node.
  /// Delete the attribute node if it does exist.
  COSUPPORT_XERCES_API
  inline
  void destroyAttrNode(XN::DOMNode *const node, XMLCh const *const attr) {
    assert(node != nullptr || !"WTF?! Missing node!");
    assert(
        node->getNodeType() == XN::DOMNode::ELEMENT_NODE ||
        node->getNodeType() == XN::DOMNode::DOCUMENT_NODE);
    XN::DOMNamedNodeMap *attrs = node->getAttributes();
    assert(attrs != nullptr);
    XN::DOMAttr *retval = static_cast<XN::DOMAttr *>(attrs->getNamedItem(attr));
    if (retval != nullptr)
      // Delete attribute node
      attrs->removeNamedItem(attr)->release();
  }

  COSUPPORT_XERCES_API
  inline
  void destroyAttrNode(XN::DOMNode *const node, char const *const attr)
    { return destroyAttrNode(node, XStr(attr).c_str()); }

  namespace Detail {

    template <typename T>
    struct GetNodeValueAs {
      typedef T result_type;

      static
      result_type apply(XN::DOMNode const *const node) {
        assert(node != nullptr || !"WTF?! Missing node!");
        XMLCh const *const attrValueText = node->getNodeValue();
        assert(attrValueText != nullptr);
        return CoSupport::String::strAs<T>(NStr(attrValueText));
      }
    };

    template <typename T>
    struct GetNodeValueAs<DataTypes::MaybeValue<T> > {
      typedef DataTypes::MaybeValue<T> result_type;

      static
      result_type apply(XN::DOMNode const *const node) {
        if (node == nullptr)
          return boost::blank();
        else
          return GetNodeValueAs<T>::apply(node);
      }
    };

    template <>
    struct GetNodeValueAs<XMLCh *> {
      typedef XMLCh const *result_type;

      static
      result_type apply(XN::DOMNode const *const node) {
        assert(node != nullptr || !"WTF?! Missing node!");
        XMLCh const *const attrValueText = node->getNodeValue();
        assert(attrValueText != nullptr || !"WTF?! Missing node value!");
        return attrValueText;
      }
    };
    template <>
    struct GetNodeValueAs<XMLCh const *>
    : public GetNodeValueAs<XMLCh *> {};

    template <>
    struct GetNodeValueAs<XStr> {
      typedef XStr result_type;

      static
      result_type apply(XN::DOMNode const *const node) {
        assert(node != nullptr || !"WTF?! Missing node!");
        XMLCh const *const attrValueText = node->getNodeValue();
        assert(attrValueText != nullptr || !"WTF?! Missing node value!");
        return XStr(attrValueText);
      }
    };

    template <>
    struct GetNodeValueAs<std::string> {
      typedef std::string result_type;

      static
      result_type apply(XN::DOMNode const *const node) {
        assert(node != nullptr || !"WTF?! Missing node!");
        XMLCh const *const attrValueText = node->getNodeValue();
        assert(attrValueText != nullptr || !"WTF?! Missing node value!");
        return NStr(attrValueText);
      }
    };

    template <typename T>
    struct SetNodeValueFrom {
      typedef T const &value_type;

      static
      void apply(XN::DOMNode *node, value_type value) {
        assert(node != nullptr || !"WTF?! Missing node!");
        node->setNodeValue(XStr(String::asStr<T>(value)).c_str());
      }
    };

/*  template <typename T>
    struct SetNodeValueFrom<DataTypes::MaybeValue<T> > {
      // We don't support MaybeValus for SetNodeValueFrom functionality
      BOOST_STATIC_ASSERT(false);
    };
    template <>
    struct SetNodeValueFrom<boost::blank> {
      // We don't support boost::blank for SetNodeValueFrom functionality
      BOOST_STATIC_ASSERT(false);
    }; */

    template <>
    struct SetNodeValueFrom<XMLCh *> {
      typedef XMLCh const *value_type;

      static
      void apply(XN::DOMNode *node, value_type value) {
        assert(node != nullptr || !"WTF?! Missing node!");
        node->setNodeValue(value);
      }
    };
    template <>
    struct SetNodeValueFrom<XMLCh const *>
    : public SetNodeValueFrom<XMLCh *> {};

    template <>
    struct SetNodeValueFrom<XStr> {
      typedef XStr const &value_type;

      static
      void apply(XN::DOMNode *node, value_type value) {
        assert(node != nullptr || !"WTF?! Missing node!");
        node->setNodeValue(value.c_str());
      }
    };

    template <>
    struct SetNodeValueFrom<std::string> {
      typedef std::string const &value_type;

      static
      void apply(XN::DOMNode *node, value_type value) {
        assert(node != nullptr || !"WTF?! Missing node!");
        node->setNodeValue(XStr(value).c_str());
      }
    };

    template <typename T>
    struct GetAttrValueAs {
      typedef T result_type;

      static
      result_type apply(XN::DOMNode const *const node, const XMLCh *const attr)
        { return GetNodeValueAs<result_type>::apply(getAttrNode(node, attr)); }
    };

    template <typename T>
    struct GetAttrValueAs<DataTypes::MaybeValue<T> > {
      typedef DataTypes::MaybeValue<T> result_type;

      static
      result_type apply(XN::DOMNode const *const node, const XMLCh *const attr)
        { return GetNodeValueAs<result_type>::apply(getMaybeAttrNode(node, attr)); }
    };

    template <typename T>
    struct SetAttrValueFrom {
      typedef T const &value_type;

      static
      void apply(XN::DOMNode *node, const XMLCh *const attr, value_type value)
        { SetNodeValueFrom<T>::apply(createAttrNode(node, attr), value); }
    };

    template <typename T>
    struct SetAttrValueFrom<DataTypes::MaybeValue<T> > {
      typedef DataTypes::MaybeValue<T> const &value_type;

      static
      void apply(XN::DOMNode *node, const XMLCh *const attr, value_type value) {
        if (value.isDefined)
          SetNodeValueFrom<T>::apply(createAttrNode(node, attr), value.get());
        else
          destroyAttrNode(node, attr);
      }
    };

    template <>
    struct SetAttrValueFrom<boost::blank> {
      typedef boost::blank const &value_type;

      static
      void apply(XN::DOMNode *node, const XMLCh *const attr, value_type value)
        { destroyAttrNode(node, attr); }
    };

  };

  /// Convert value in node to type T.
  /// This throws an exception if the conversion is invalid.
  template <typename T>
  T getNodeValueAs(XN::DOMNode const *const node)
    { return Detail::GetNodeValueAs<T>::apply(node); }
  template <typename T>
  T getAttrValueAs(XN::DOMNode const *const node, XMLCh const *const attr)
    { return Detail::GetAttrValueAs<T>::apply(node, attr); }
  template <typename T>
  T getAttrValueAs(XN::DOMNode const *const node, char const *const attr)
    { return Detail::GetAttrValueAs<T>::apply(node, XStr(attr).c_str()); }

  /// Convert set value in node from type T.
  /// This throws an exception if the conversion is invalid.
  template <typename T>
  void setNodeValueFrom(XN::DOMNode *const node, const T &value)
    { Detail::SetNodeValueFrom<T>::apply(node, value); }
  template <typename T>
  void setAttrValueFrom(XN::DOMNode *const node, XMLCh const *const attr, const T &value)
    { Detail::SetAttrValueFrom<T>::apply(node, attr, value); }
  template <typename T>
  void setAttrValueFrom(XN::DOMNode *const node, char const *const attr, const T &value)
    { Detail::SetAttrValueFrom<T>::apply(node, XStr(attr).c_str(), value); }

} } } // namespace CoSupport::XML::Xerces

#endif // _INCLUDED_COSUPPORT_XML_XERCES_COMMON_HPP
