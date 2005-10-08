/* vim: set sw=2 ts=8: */

#include <assert.h>
#include <iostream>

#include "arrayfifo.hpp"

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


