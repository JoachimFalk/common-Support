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

#ifndef SMOC_OSTREAM_HPP
#define SMOC_OSTREAM_HPP

#include <iostream>
#include <ios>
#include <string>

class smoc_ostream : public std::basic_ostream<char, std::char_traits<char> > {
public:
  smoc_ostream (std::ostream &os); 

public:
  virtual smoc_ostream&  put ( char ch );

  virtual smoc_ostream&  write ( const char* str , std::streamsize n );  

public:
  //Manipulators
  friend smoc_ostream& endl(smoc_ostream& os);

  friend smoc_ostream& inc_level(smoc_ostream& os);
  friend smoc_ostream& dec_level(smoc_ostream& os);

	virtual std::streamsize width ( ) const;
	virtual std::streamsize width ( std::streamsize wide );

public:

  virtual smoc_ostream& operator<< (bool val );

  virtual smoc_ostream& operator<< (short val );
  virtual smoc_ostream& operator<< (unsigned short val );
  virtual smoc_ostream& operator<< (int val );
  virtual smoc_ostream& operator<< (unsigned int val );  
  virtual smoc_ostream& operator<< (long val);
  virtual smoc_ostream& operator<< (unsigned long val );

  virtual smoc_ostream& operator<< (float val );
  virtual smoc_ostream& operator<< (double val );
  virtual smoc_ostream& operator<< (long double val );

  virtual smoc_ostream& operator<< (void* val );

  //virtual smoc_ostream& operator<< (std::streambuf& sb );
  
  virtual smoc_ostream& operator<<(smoc_ostream& (*m)(smoc_ostream&));
  //without this method, the endl manipulator cannot be modified!!!
  

  virtual smoc_ostream& operator<< (std::ios& ( *pf )(std::ios&));
  virtual smoc_ostream& operator<< (std::ios_base& ( *pf )(std::ios_base&));

 
  friend smoc_ostream& operator<< (smoc_ostream& os, char ch );
  friend smoc_ostream& operator<< (smoc_ostream& os, signed char ch );
  friend smoc_ostream& operator<< (smoc_ostream& os, unsigned char ch );
 
  friend smoc_ostream& operator<< (smoc_ostream& os, const char* str );
  friend smoc_ostream& operator<< (smoc_ostream& os, const signed char* str );
  friend smoc_ostream& operator<< (smoc_ostream& os, const unsigned char* str );

  friend smoc_ostream& operator<< (smoc_ostream& os, const std::string& str );
  friend smoc_ostream& operator<< (smoc_ostream& os, std::string& str );

private:
  std::ostream &std_os;

  unsigned int indent_number;
  bool begin_of_line;

private:
  virtual void indent(void);

};


#endif
