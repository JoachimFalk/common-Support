// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2018 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2019 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <CoSupport/Streams/ColorStreambuf.hpp>

#include <CoSupport/String/convert.hpp>

namespace CoSupport { namespace Streams {

using String::asStr;
using String::strAs;
  
Color::Color(size_t color, size_t attr) :
  escape("\033[" + asStr(attr) + ";" + asStr(color) + "m")
{
  // std::cerr << "Enter Color::Color(size_t color, size_t attr)" << std::endl;
  // std::cerr << "this = " << this << std::endl;
  // std::cerr << escape << std::endl;
  // std::cerr << "Leave Color::Color(size_t color, size_t attr)" << std::endl;
  // std::cerr.flush();
}

Color::Color(size_t attr) :
  escape("\033[" + asStr(attr) + "m")
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

int ColorStreambuf::overflow(int c) {
  if (newline) {
    next->sputn(color.escape.c_str(), color.escape.size());
    newline = false;
  }
  if (!newline && c == '\n') {
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

std::ostream& operator << (std::ostream &os, const Color &c) {
  ColorStreambuf *buf = static_cast<ColorStreambuf *>(os.pword(ColorStreambuf::index));
  if (buf) buf->setColor(c);
  return os;
}

} } // namespace CoSupport::Streams
