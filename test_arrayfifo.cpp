/* vim: set sw=2 ts=8: */
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

#include <assert.h>
#include <iostream>

#include <cosupport/arrayfifo.hpp>

using namespace CoSupport;

template <typename T, size_t N>
std::ostream &operator << (std::ostream &out, const ArrayFifo<T,N> &fifo) {
  out << "fifo size: " << fifo.size()
      << " space: " << fifo.space()
      << " capacity: " << fifo.capacity();
  return out;
}

typedef ArrayFifo<char, 144> Fifo;

int main( int argc, char *argv[] ) {
  Fifo fifo;
  
  for ( int i = 0; i <= 23; ++i ) {
    std::cout << fifo << std::endl;
    for ( Fifo::iterator iter = fifo.begin();
          iter != fifo.end();
          ++iter )
      std::cout << *iter;
    std::cout << std::endl;
    for ( Fifo::const_iterator iter = fifo.end();
          iter != const_cast<const Fifo &>(fifo).begin();
          --iter )
      std::cout << *(iter-1);
    std::cout << std::endl;
    if ( fifo.empty() )
      std::cout << "fifo empty !" << std::endl;
    assert( fifo.size() + fifo.space() == fifo.capacity() );
    assert( fifo.end() - fifo.begin() == fifo.size() );
    for ( int j = 0; j < i; ++j )
      fifo.pop_front();
    for ( int j = 0; j < 23 - i; ++j )
      fifo.push_back('a'+i);
  }
  std::cout << fifo << std::endl;
  return 0;
}


