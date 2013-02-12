// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2004-2011 Hardware-Software-CoDesign, University of
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

#include <CoSupport/Streams/TranslationStreambuf.hpp>

#include <cstring>

namespace CoSupport { namespace Streams {

TranslationStreambuf::TranslationStreambuf(const TranslationMap& tm)
  : tm(tm) {}

void TranslationStreambuf::setTranslationMap(const TranslationMap& value)
  { tm = value; }

int TranslationStreambuf::overflow(int c) {
  if (const char* t = tm.get(c)) {
    next->sputn(t, std::strlen(t));
    return 1;
  }
  else {
    return next->sputc(c);
  }
}

bool TranslationStreambuf::hasManip() const
{ return true; }
  
int TranslationStreambuf::getIndex() const
{ return index; }

const int TranslationStreambuf::index(std::ostream::xalloc());

std::ostream &operator<<(std::ostream &os, const TranslationMap &t) {
  TranslationStreambuf *buf = static_cast<TranslationStreambuf *>(os.pword(TranslationStreambuf::index));
  if (buf) buf->setTranslationMap(t);
  return os;
}

TranslationMap::TranslationMap() {}
  
TranslationMap::TranslationMap(TranslationOp o[], size_t count) {
  for(size_t i = 0; i < count; ++i) {
    tm[o[i].from] = o[i].to;
  }
}

const char* TranslationMap::get(char c) const {
  std::map<char,const char*>::const_iterator i = tm.find(c);
  if (i == tm.end()) return 0;
  return i->second;
}

const TranslationMap& TranslationMap::XMLAttr() {
  static TranslationOp o[] =
    { {'&'  , "&amp;" },
      {'<'  , "&lt;"  },
      {'>'  , "&gt;"  },
      {'\"' , "&quot;"},
      {'\'' , "&apos;"} };
  static TranslationMap t(o, sizeof(o) / sizeof(TranslationOp));
  return t;
}

const TranslationMap& TranslationMap::None() {
  static TranslationMap t;
  return t;
}

} } // namespace CoSupport::Streams
