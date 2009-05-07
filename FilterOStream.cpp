// vim: set sw=2 ts=8:
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

#include <CoSupport/String/convert.hpp>

#include <CoSupport/Streams/DebugStreambuf.hpp>
#include <CoSupport/Streams/ColorStreambuf.hpp>
#include <CoSupport/Streams/FilterOStream.hpp>
#include <CoSupport/Streams/FilterStreambuf.hpp>
#include <CoSupport/Streams/HeaderFooterStreambuf.hpp>
#include <CoSupport/Streams/IndentStreambuf.hpp>
#include <CoSupport/Streams/LineNumberStreambuf.hpp>
#include <CoSupport/Streams/NullStreambuf.hpp>
#include <CoSupport/Streams/TranslationStreambuf.hpp>

#include <cassert>
#include <iostream>
#include <cstring>

namespace CoSupport { namespace Streams {

using String::asStr;
using String::strAs;
  
FilterStreambuf::FilterStreambuf(std::streambuf *next) :
  next(next)
{
  //std::cerr << "Enter FilterStreambuf::FilterStreambuf" << std::endl;
  //std::cerr << "Leave FilterStreambuf::FilterStreambuf" << std::endl;
}

bool FilterStreambuf::hasManip() const
{ return false; }

int FilterStreambuf::getIndex() const
{ return -1; }

FilterStreambuf::~FilterStreambuf()
{}

  
LineNumberStreambuf::LineNumberStreambuf(std::streambuf *next) :
  FilterStreambuf(next),
  line(1),
  newline(true)
{}

int LineNumberStreambuf::overflow(int c)
{
  if(newline) {
    std::string tmp = asStr(line) + ": ";
    next->sputn(tmp.c_str(), tmp.size());
    newline = false;
  }
  if(!newline && c == '\n') {
    line++;
    newline = true;
  }
  return next->sputc(c);
}


Header::Header(const std::string &value, bool add) :
  value(value),
  add(add)
{}

Header::Header(const char *value, bool add) :
  value(value),
  add(add)
{}

const Header Header::Start("", true);
const Header Header::End("", false);

Footer::Footer(const std::string &value, bool add) :
  value(value),
  add(add)
{}

Footer::Footer(const char *value, bool add) :
  value(value),
  add(add)
{}

const Footer Footer::Start("", true);
const Footer Footer::End("", false);

HeaderFooterStreambuf::HeaderFooterStreambuf(
    const std::string &header,
    const std::string &footer,
    bool add_header,
    bool add_footer,
    std::streambuf *next) :
  FilterStreambuf(next),
  header(header),
  footer(footer),
  add_header(add_header),
  add_footer(add_footer),
  newline(true)
{
  //std::cerr << "Enter HeaderFooterStreambuf::HeaderFooterStreambuf" << std::endl;
  //std::cerr << "Leave HeaderFooterStreambuf::HeaderFooterStreambuf" << std::endl;
}

const std::string& HeaderFooterStreambuf::getHeader() const
{ return header; }

void HeaderFooterStreambuf::setHeader(const std::string &value)
{
  if(add_header)
    header += value;
  else
    header = value;
}

const std::string& HeaderFooterStreambuf::getFooter() const
{ return footer; }

void HeaderFooterStreambuf::setFooter(const std::string &value)
{
  if(add_footer)
    footer += value;
  else
    footer = value;
}

void HeaderFooterStreambuf::setAddHeader(bool value)
{ add_header = value; }

void HeaderFooterStreambuf::setAddFooter(bool value)
{ add_footer = value; }

int HeaderFooterStreambuf::overflow(int c)
{
  if(add_header || add_footer) {
    if(add_header)
      header += c;
    if(add_footer)
      footer += c;
    return 1;
  }
  
  if(newline) {
    next->sputn(header.c_str(), header.size());
    newline = false;
  }
  if(!newline && c == '\n') {
    next->sputn(footer.c_str(), footer.size());
    newline = true;
  } 
  return next->sputc(c);
}

bool HeaderFooterStreambuf::hasManip() const
{ return true; }

int HeaderFooterStreambuf::getIndex() const
{ return index; }

const int HeaderFooterStreambuf::index(std::ostream::xalloc());

std::ostream& operator<<(std::ostream &os, const Header &p)
{
  HeaderFooterStreambuf *buf =
    static_cast<HeaderFooterStreambuf *>(os.pword(HeaderFooterStreambuf::index));
  if(buf) {
    buf->setHeader(p.value);
    buf->setAddHeader(p.add);
  }
  return os;
}

std::ostream& operator<<(std::ostream &os, const Footer &p)
{
  HeaderFooterStreambuf *buf =
    static_cast<HeaderFooterStreambuf *>(os.pword(HeaderFooterStreambuf::index));
  if(buf) {
    buf->setFooter(p.value);
    buf->setAddFooter(p.add);
  }
  return os;
}

Color::Color(size_t color, size_t attr) :
  escape("\e[" + asStr(attr) + ";" + asStr(color) + "m")
{
  // std::cerr << "Enter Color::Color(size_t color, size_t attr)" << std::endl;
  // std::cerr << "this = " << this << std::endl;
  // std::cerr << escape << std::endl;
  // std::cerr << "Leave Color::Color(size_t color, size_t attr)" << std::endl;
  // std::cerr.flush();
}

Color::Color(size_t attr) :
  escape("\e[" + asStr(attr) + "m")
{  
  // std::cerr << "Enter Color::Color(size_t attr)" << std::endl;
  // std::cerr << "this = " << this << std::endl;
  // std::cerr << escape << std::endl;
  // std::cerr << "Leave Color::Color(size_t attr)" << std::endl;
  // std::cerr.flush();
}

ColorStreambuf::ColorStreambuf(
    const Color &color,
    const Color &reset,
    std::streambuf *next) :
  FilterStreambuf(next),
  color(color),
  reset(reset),
  newline(true)
{
  //std::cerr << "Enter ColorStreambuf::ColorStreambuf" << std::endl;
  //std::cerr << "Leave ColorStreambuf::ColorStreambuf" << std::endl;
}

void ColorStreambuf::setColor(const Color &c)
{ color = c; }

void ColorStreambuf::setReset(const Color &c)
{ reset = c; }

int ColorStreambuf::overflow(int c)
{
  if(newline) {
    next->sputn(color.escape.c_str(), color.escape.size());
    newline = false;
  }
  if(!newline && c == '\n') {
    next->sputn(reset.escape.c_str(), reset.escape.size());
    newline = true;
  } 
  return next->sputc(c);
}

bool ColorStreambuf::hasManip() const
{ return true; }

int ColorStreambuf::getIndex() const
{ return index; }

const int ColorStreambuf::index(std::ostream::xalloc());

std::ostream& operator<<(std::ostream &os, const Color &c)
{
  ColorStreambuf *buf = static_cast<ColorStreambuf *>(os.pword(ColorStreambuf::index));
  if(buf) buf->setColor(c);
  return os;
}


Indent::Indent(int delta_level) :
  delta_level(delta_level)
{}
  
const Indent Indent::Up(1);
const Indent Indent::Down(-1);


IndentStreambuf::IndentStreambuf(
    size_t delta,
    size_t indent,
    std::streambuf *next) :
  FilterStreambuf(next),
  delta(delta),
  indent(indent),
  newline(true)
{
  // std::cerr << "Enter IndentStreambuf::IndentStreambuf" << std::endl;
  // std::cerr << "Leave IndentStreambuf::IndentStreambuf" << std::endl;
}

void IndentStreambuf::setIndentation(int value)
{
  if(value < 0)
    assert(!"Negative indention not allowed!");
  indent = value;
}

void IndentStreambuf::setDeltaLevel(int value)
{
  if(value < 0 && indent < -(value * delta))
    assert(!"Too much negative delta indentation!");
  indent += value * delta;
}

int IndentStreambuf::overflow(int c)
{
  static const char ws[] = "                ";
  static const size_t WS = sizeof(ws) - 1;
  
  if(newline) {
    for(size_t i=0; i<indent/WS; ++i) {
      next->sputn(ws, WS);
    }
    next->sputn(ws, indent % WS);     
    newline = false;
  }
  if(!newline && c == '\n') {
    newline = true;
  } 
  return next->sputc(c);
}

bool IndentStreambuf::hasManip() const
{ return true; }

int IndentStreambuf::getIndex() const
{ return index; }

const int IndentStreambuf::index(std::ostream::xalloc());

std::ostream& operator<<(std::ostream &os, const Indent &i)
{
  IndentStreambuf *buf = static_cast<IndentStreambuf *>(os.pword(IndentStreambuf::index));
  if(buf) buf->setDeltaLevel(i.delta_level);
  return os;
}

ScopedIndent::ScopedIndent(std::ostream& out, const Indent& indent) :
  out(out << indent), delta(indent.delta_level)
{}
  
ScopedIndent::~ScopedIndent()
{ out << Indent(-delta); }

Debug::Debug(size_t level) :
  level(level)
{}
  
const Debug Debug::Low(0);
const Debug Debug::Medium(1);
const Debug Debug::High(2);
const Debug Debug::None(3);

DebugStreambuf::DebugStreambuf(
    const Debug &dbg,
    bool visible,
    std::streambuf *next) :
  FilterStreambuf(next),
  level(dbg.level),
  visible(visible)
{}

void DebugStreambuf::setLevel(const Debug &dbg)
{ level = dbg.level; }

void DebugStreambuf::setVisibility(const Debug &dbg)
{ visible = dbg.level >= level; }

int DebugStreambuf::overflow(int c)
{
  if(visible)
    return next->sputc(c);
  else
    return 1;
}

bool DebugStreambuf::hasManip() const
{ return true; }

int DebugStreambuf::getIndex() const
{ return index; }

const int DebugStreambuf::index(std::ostream::xalloc());

std::ostream& operator<<(std::ostream &os, const Debug &d)
{
  DebugStreambuf *buf = static_cast<DebugStreambuf *>(os.pword(DebugStreambuf::index));
  if(buf) buf->setVisibility(d);
  return os;
}

  
NullStreambuf::NullStreambuf(std::streambuf *next) :
  FilterStreambuf(next)
{}

int NullStreambuf::overflow(int c)
{ return 1; }


FilterOStream::FilterOStream(std::ostream &os) :
  std::ostream(os.rdbuf())
{}
  
void FilterOStream::insert(FilterStreambuf &head)
{
  head.next = rdbuf();
  rdbuf(&head);
  if(head.hasManip())
    pword(head.getIndex()) = &head;
}

TranslationStreambuf::TranslationStreambuf(const TranslationMap& tm)
  : tm(tm) {}

void TranslationStreambuf::setTranslationMap(const TranslationMap& value)
  { tm = value; }

int TranslationStreambuf::overflow(int c) {
  if(const char* t = tm.get(c)) {
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
  if(buf) buf->setTranslationMap(t);
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
  if(i == tm.end()) return 0;
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

