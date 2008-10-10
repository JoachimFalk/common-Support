// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2004-2008 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_STREAMS_TRANSLATIONSTREAMBUF_HPP
#define _INCLUDED_COSUPPORT_STREAMS_TRANSLATIONSTREAMBUF_HPP

#include <stddef.h>
#include <map>

#include "FilterStreambuf.hpp"

namespace CoSupport { namespace Streams {

struct TranslationOp {
  char from;
  const char* to;
};

/**
 * stream manipulator for the TranslationStreambuf custom
 * streambuffer
 */
struct TranslationMap {
  /// @brief Translation map
  std::map<char,const char*> tm;

  /// @brief Default constructor (no translation)
  TranslationMap() {}
  
  /// @brief Constructor (custom map)
  TranslationMap(TranslationOp o[], size_t count) {
    for(size_t i = 0; i < count; ++i) {
      tm[o[i].from] = o[i].to;
    }
  }

  /// @brief Lookup translation string
  const char* get(char c) const {
    std::map<char,const char*>::const_iterator i = tm.find(c);
    if(i == tm.end()) return 0;
    return i->second;
  }

  /// predefined translation maps
  /// We do not use static variables in order to avoid racing conditions
  /// when initializing global variables.
  static const TranslationMap& XMLAttr() {
    static TranslationOp o[] =
      { {'&'  , "&amp;" },
        {'<'  , "&lt;"  },
        {'>'  , "&gt;"  },
        {'\"' , "&quot;"},
        {'\'' , "&apos;"} };
    static TranslationMap t(o, sizeof(o));
    return t;
  }
  static const TranslationMap& None() {
    static TranslationMap t;
    return t;
  }
};

/**
 * replaces characters according to translation map
 */
class TranslationStreambuf
: public FilterStreambuf {
private:
  /// current translation map
  TranslationMap tm;

public:
  /// @brief Constructor
  TranslationStreambuf(
      const TranslationMap& tm = TranslationMap::None());

  /// @brief Set new translation map
  void setTranslationMap(const TranslationMap& value);

protected:
  int overflow(int c);
  
public:
#ifndef KASCPAR_PARSING
  /// index obtained with std::ostream::xalloc
  static const int index;
#endif
 
  /// see TranslationMap
  bool hasManip() const;
  
  /// returns the (static) index
  int getIndex() const;
};

/// output operator for the TranslationMap manipulator
std::ostream &operator<<(std::ostream &os, const TranslationMap &t);

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_FILTER_OSTREAM_HPP
