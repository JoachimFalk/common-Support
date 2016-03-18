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

#ifndef _INCLUDED_COSUPPORT_STREAMS_TRANSLATIONSTREAMBUF_HPP
#define _INCLUDED_COSUPPORT_STREAMS_TRANSLATIONSTREAMBUF_HPP

#include <stddef.h>
#include <map>

#include "FilterStreambuf.hpp"

#include "export_config.h"

namespace CoSupport { namespace Streams {

struct COSUPPORT_STREAMS_API
TranslationOp {
  char from;
  const char* to;
};

/**
 * stream manipulator for the TranslationStreambuf custom
 * streambuffer
 */
struct COSUPPORT_STREAMS_API
TranslationMap {
  /// @brief Translation map
  std::map<char,const char*> tm;

  /// @brief Default constructor (no translation)
  TranslationMap();
  
  /// @brief Constructor (custom map)
  TranslationMap(TranslationOp o[], size_t count);

  /// @brief Lookup translation string
  const char* get(char c) const;

  /// predefined translation maps
  /// We do not use static variables in order to avoid racing conditions
  /// when initializing global variables.
  static const TranslationMap& XMLAttr();
  static const TranslationMap& None();
};

/**
 * replaces characters according to translation map
 */
class COSUPPORT_STREAMS_API
TranslationStreambuf
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
  /// index obtained with std::ostream::xalloc
  static const int index;
 
  /// see TranslationMap
  bool hasManip() const;
  
  /// returns the (static) index
  int getIndex() const;
};

/// output operator for the TranslationMap manipulator
COSUPPORT_STREAMS_API
std::ostream &operator<<(std::ostream &os, const TranslationMap &t);

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_FILTER_OSTREAM_HPP
