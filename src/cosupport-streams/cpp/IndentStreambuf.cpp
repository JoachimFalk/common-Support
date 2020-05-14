// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2020 FAU -- Joachim Falk <joachim.falk@fau.de>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) any
 * later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#include <CoSupport/Streams/IndentStreambuf.hpp>

#include <cassert>

namespace CoSupport { namespace Streams {

Indent::Indent(int delta_level)
  : delta_level(delta_level) {}
  
const Indent Indent::Up(1);
const Indent Indent::Down(-1);

std::ostream &operator << (std::ostream &os, const Indent &i) {
  IndentStreambuf *buf = static_cast<IndentStreambuf *>(os.pword(IndentStreambuf::index));
  if (buf) buf->setDeltaLevel(i.delta_level);
  return os;
}

ScopedIndent::ScopedIndent(std::ostream &out, const Indent &indent)
  : out(out << indent), delta(indent.delta_level) {}

ScopedIndent::~ScopedIndent()
  { out << Indent(-delta); }

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
  if (value < 0)
    assert(!"Negative indention not allowed!");
  indent = value;
}

void IndentStreambuf::setDeltaLevel(int value)
{
  if (value < 0 && indent < -(value * delta))
    assert(!"Too much negative delta indentation!");
  indent += value * delta;
}

int IndentStreambuf::overflow(int c)
{
  static const char ws[] = "                ";
  static const size_t WS = sizeof(ws) - 1;
  
  if (newline) {
    for(size_t i=0; i<indent/WS; ++i) {
      next->sputn(ws, WS);
    }
    next->sputn(ws, indent % WS);     
    newline = false;
  }
  if (!newline && c == '\n') {
    newline = true;
  } 
  return next->sputc(c);
}

bool IndentStreambuf::hasManip() const
{ return true; }

int IndentStreambuf::getIndex() const
{ return index; }

const int IndentStreambuf::index(std::ostream::xalloc());

} } // namespace CoSupport::Streams
