// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2018 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <cassert>
#include <iostream>

#include <CoSupport/DataTypes/ArrayFifo.hpp>

using namespace CoSupport::DataTypes;

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
    assert( fifo.end() - fifo.begin() == static_cast<int>(fifo.size()) );
    for ( int j = 0; j < i; ++j )
      fifo.pop_front();
    for ( int j = 0; j < 23 - i; ++j )
      fifo.push_back('a'+i);
  }
  std::cout << fifo << std::endl;
  return 0;
}


