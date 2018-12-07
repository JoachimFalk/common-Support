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

#include <CoSupport/Streams/ColorStreambuf.hpp>

#include <CoSupport/String/convert.hpp>

namespace CoSupport { namespace Streams {

using String::asStr;
using String::strAs;
  
Color::Color(size_t color, size_t attr) :
  escape("\027[" + asStr(attr) + ";" + asStr(color) + "m")
{
  // std::cerr << "Enter Color::Color(size_t color, size_t attr)" << std::endl;
  // std::cerr << "this = " << this << std::endl;
  // std::cerr << escape << std::endl;
  // std::cerr << "Leave Color::Color(size_t color, size_t attr)" << std::endl;
  // std::cerr.flush();
}

Color::Color(size_t attr) :
  escape("\027[" + asStr(attr) + "m")
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
