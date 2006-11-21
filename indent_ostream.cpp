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

#include "indent_ostream.hpp"

namespace CoSupport {

indent_buffer::indent_buffer(std::streambuf *buf, size_t delta, size_t indent) :
  buf(buf),
  delta(delta),
  indent(indent),
  fill(true)
{}


void indent_buffer::up()
{ indent += delta; }


void indent_buffer::down()
{ if(indent >= delta) indent -= delta; }


int indent_buffer::overflow(int c)
{
  static const char ws[] = "                ";
  static const size_t WS = sizeof(ws) - 1;

  if(fill) {
    for(size_t i=0; i<indent/WS; ++i) {
      buf->sputn(ws, WS);
    }
    buf->sputn(ws, indent % WS);
    fill = false;
  }
  
  if(c == '\n')
    fill = true;
  
  return buf->sputc(c);
}


indent_ostream::indent_ostream(std::ostream &os, size_t delta, size_t indent) :
  std::ostream(&buf),
  buf(os.rdbuf(), delta, indent)
{}


std::ostream& operator<<(std::ostream &os, const indent& t)
{
  indent_buffer *buf = dynamic_cast<indent_buffer *>(os.rdbuf());
  if(buf) buf->up();
  return os;  
}


std::ostream& operator<<(std::ostream &os, const rindent& t)
{
  indent_buffer *buf = dynamic_cast<indent_buffer *>(os.rdbuf());
  if(buf) buf->down();
  return os;  
}

}

