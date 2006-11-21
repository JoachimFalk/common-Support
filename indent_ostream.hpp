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

#ifndef _INCLUDED_COSUPPORT_INDENT_OSTREAM_HPP
#define _INCLUDED_COSUPPORT_INDENT_OSTREAM_HPP

#include <streambuf>
#include <ostream>

namespace CoSupport {

/**
 * custom streambuf implementation: prints spaces after
 * each newline
 */
class indent_buffer :
  public std::streambuf
{
private:
  /// the original streambuffer
  std::streambuf *buf;
  
  /// spaces to add/subtract when up/down is called
  size_t delta;
  
  /// current number of spaces to be printed after newline
  size_t indent;

  /// true, if spaces must be printed before next character
  bool fill;
  
public:
  /// constructs a new indent_buffer with the specified target
  /// buffer, delta and initial indentation
  indent_buffer(std::streambuf *buf, size_t delta, size_t indent);
  
  /// adds delta whitespaces to indent
  void up();
  
  /// subtracts delta whitespaces from indent
  void down();

private:
  /// is called when the buffer is full
  int overflow(int c);
};



/**
 * custom ostream implementation that provides indentation
 * functionality through the underlying custom indent_buffer
 */
class indent_ostream :
  public std::ostream
{
private:
  /// the used buffer
  indent_buffer buf;

public:
  /// constructs a new indent_ostream with the specified target
  /// ostream, delta and initial indentation
  indent_ostream(std::ostream &os, size_t delta = 1, size_t indent = 0);

private:
  /// disabled copy constructor
  indent_ostream(const indent_ostream &);

  /// disabled copy operator
  indent_ostream& operator=(const indent_ostream &);
};



/// dummy struct that can be used as stream manipulator to
/// raise the indentation level
struct indent {};
std::ostream& operator<<(std::ostream &os, const indent& t);



/// dummy struct that can be used as stream manipilator to
/// lower the indentation level
struct rindent {};
std::ostream& operator<<(std::ostream &os, const rindent& t);



} // namespace CoSupport

#endif // _INCLUDED_COSUPPORT_INDENT_OSTREAM_HPP
