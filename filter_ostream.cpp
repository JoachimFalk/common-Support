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

#include "filter_ostream.hpp"
#include "string_convert.hpp"

namespace CoSupport {
  
FilterStreambuf::FilterStreambuf(std::streambuf *next) :
  next(next)
{}

bool FilterStreambuf::hasManip() const
{ return false; }

int FilterStreambuf::getIndex() const
{ return -1; }

  
LineNumberStreambuf::LineNumberStreambuf(std::streambuf *next) :
  FilterStreambuf(next),
  line(1),
  newline(true)
{}

int LineNumberStreambuf::overflow(int c)
{
  if(newline) {
    std::string tmp = string_cast(line) + ": ";
    next->sputn(tmp.c_str(), tmp.size());
    newline = false;
  }
  if(!newline && c == '\n') {
    line++;
    newline = true;
  }
  return next->sputc(c);
}


Header::Header(const char *value) :
  value(value)
{}

Footer::Footer(const char *value) :
  value(value)
{}

HeaderFooterStreambuf::HeaderFooterStreambuf(
    const std::string &header,
    const std::string &footer,
    std::streambuf *next) :
  FilterStreambuf(next),
  header(header),
  footer(footer),
  newline(true)
{}

void HeaderFooterStreambuf::setHeader(const std::string &value)
{ header = value; }

void HeaderFooterStreambuf::setFooter(const std::string &value)
{ footer = value; }

int HeaderFooterStreambuf::overflow(int c)
{
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
  if(buf) buf->setHeader(p.value);
  return os;
}

std::ostream& operator<<(std::ostream &os, const Footer &p)
{
  HeaderFooterStreambuf *buf =
    static_cast<HeaderFooterStreambuf *>(os.pword(HeaderFooterStreambuf::index));
  if(buf) buf->setFooter(p.value);
  return os;
}


Color::Color(const char *escape) :
  escape(escape)
{}
  
const Color Color::Auto("\e[0m");
const Color Color::Black("\e[0;30m");
const Color Color::Blue("\e[0;34m");
const Color Color::Brown("\e[0;33m");
const Color Color::Cyan("\e[0;36m");
const Color Color::Gray("\e[0;37m");
const Color Color::Green("\e[0;32m");
const Color Color::Purple("\e[0;35m");
const Color Color::Red("\e[0;31m");
const Color Color::White("\e[1;37m");
const Color Color::Yellow("\e[1;33m");

ColorStreambuf::ColorStreambuf(
    const Color &color,
    const Color &reset,
    std::streambuf *next) :
  FilterStreambuf(next),
  color(color),
  reset(reset),
  newline(true)
{}

void ColorStreambuf::setColor(const Color &c)
{ color = c; }

void ColorStreambuf::setReset(const Color &c)
{ reset = c; }

int ColorStreambuf::overflow(int c)
{
  if(newline) {
    next->sputn(color.escape, std::strlen(color.escape));
    newline = false;
  }
  if(!newline && c == '\n') {
    next->sputn(reset.escape, std::strlen(reset.escape));
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
{}

void IndentStreambuf::setIndentation(int value)
{ indent = value; }

void IndentStreambuf::setDeltaLevel(int value)
{ indent += value * delta; }

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


Debug::Debug(size_t level) :
  level(level)
{}
  
const Debug Debug::Low(0);
const Debug Debug::Medium(1);
const Debug Debug::High(2);
const Debug Debug::None(3);

DebugStreambuf::DebugStreambuf(
    const Debug &dbg,
    std::streambuf *next) :
  FilterStreambuf(next),
  level(dbg.level),
  visible(false)
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


FilterOstream::FilterOstream(std::ostream &os) :
  std::ostream(os.rdbuf())
{}
  
void FilterOstream::insert(FilterStreambuf &head)
{
  head.next = rdbuf();
  rdbuf(&head);
  if(head.hasManip())
    pword(head.getIndex()) = &head;
}

} // namespace CoSupport

