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

#include "smoc_ostream.hpp"

smoc_ostream::smoc_ostream (std::ostream &os)
  : std::basic_ostream<char, std::char_traits<char> >(os.rdbuf()),
  std_os(os), indent_number(0), begin_of_line(true)
{
}

void smoc_ostream::indent(void){
  if (begin_of_line){
    for (unsigned int i = 0; i < indent_number; i++)
      std_os.put(' ');
    begin_of_line = false;
  }
}

smoc_ostream&  smoc_ostream::put ( char ch ) {
  if (ch == '\n'){
    begin_of_line = true;
  }else{
    indent();
  }
  std_os.put(ch);
  return *this;
}

smoc_ostream&  smoc_ostream::write ( const char* str , std::streamsize n ) {
  for (std::streamsize i = 0; i < n; i++)
    std_os.put(str[i]);
  return *this;
}

smoc_ostream& smoc_ostream::operator<<(smoc_ostream& (*m)(smoc_ostream&)){
  //without this method, the endl manipulator cannot be modified!!!
  return (*m)(*this);
}

std::streamsize smoc_ostream::width ( ) const{
	return std_os.width();
}
std::streamsize smoc_ostream::width ( std::streamsize wide ){
	return std_os.width(wide);
}

smoc_ostream& endl(smoc_ostream& os){
  os << '\n';
  os.flush();
  return os;
}

smoc_ostream& inc_level(smoc_ostream& os){ 
  os.indent_number++;
  return os;
}

smoc_ostream& dec_level(smoc_ostream& os){ 
  if (os.indent_number > 0)
    os.indent_number--;
  return os;
}


smoc_ostream& smoc_ostream::operator<< (bool val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (short val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (unsigned short val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (int val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (unsigned int val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (long val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (unsigned long val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (float val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (double val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (long double val ){
  indent();
  std_os << val;
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (void* val ){
  indent();
  std_os << val;
  return *this;
}

/*
smoc_ostream& smoc_ostream::operator<< (std::streambuf& sb ){
  //do not indent
  std_os << sb;
  return *this;
}
*/

smoc_ostream& smoc_ostream::operator<< (std::ios& ( *pf )(std::ios&)){
  //do not indent
  std_os << (pf);
  return *this;
}

smoc_ostream& smoc_ostream::operator<< (std::ios_base& ( *pf )(std::ios_base&)){
  //do not indent
  std_os << (pf);
  return *this;
}


smoc_ostream& operator<< (smoc_ostream& os, char ch ){
  return os.put(ch);
}

smoc_ostream& operator<< (smoc_ostream& os, signed char ch ){
  return os.put(ch);
}

smoc_ostream& operator<< (smoc_ostream& os, unsigned char ch ){
  return os.put(ch);
}
 
smoc_ostream& operator<< (smoc_ostream& os, const char* str ){
  int i = 0;
  while (str[i] != 0){
    os<<str[i];
    i++;
  };
  return os;
}

smoc_ostream& operator<< (smoc_ostream& os, const signed char* str ){
  int i = 0;
  while (str[i] != 0){
    os<<str[i];
    i++;
  };
  return os;
}

smoc_ostream& operator<< (smoc_ostream& os, const unsigned char* str ){
  int i = 0;
  while (str[i] != 0){
    os<<str[i];
    i++;
  };
  return os;
}

smoc_ostream& operator<< (smoc_ostream& os, const std::string& str ){
  for(unsigned int i = 0; i < str.length(); i++){
    os << str[i];
  }
  return os;
}

smoc_ostream& operator<< (smoc_ostream& os, std::string& str ){
  for(unsigned int i = 0; i < str.length(); i++){
    os << str[i];
  }
  return os;
}

