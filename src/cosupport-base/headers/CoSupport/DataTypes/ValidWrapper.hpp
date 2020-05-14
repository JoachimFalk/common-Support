// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_VALIDWRAPPER_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_VALIDWRAPPER_HPP

#include <cassert>
#include <new>
#include <cstdlib> 

namespace CoSupport { namespace DataTypes {

/// test code
static double testingLikelihood;

template<class T>
class ValidWrapper {
public:
  typedef ValidWrapper<T> type;

  ValidWrapper()
    :valid(true),
     allocated(false) {
    //cerr << "ValidWrapper()" << endl;
    randError(Detail::testingLikelihood);
  }

  ValidWrapper( const T& t )
    :valid(true),
     allocated(true) {
    //cerr << "ValidWrapper(const T& t)" << endl;
    new(data) T(t);
    randError(Detail::testingLikelihood);
  }

  operator const T&() const {
    //cerr << "T&()" << endl;
    assert(allocated);
    return *getPointer();
  }

  void operator=( const T& t ) {
    //cerr << "op=()" << endl;
    if(allocated) {
      *getPointer() = t;
    } else {
      new(data) T(t);
      allocated = true;
    }
  }
  const bool isValid() const {
    //cerr << "isValid()" << endl;
    return valid;
  }

  // for debugging
  void randError(double likelihood) const {
    double rnd = ( ((double)rand()) /RAND_MAX);
    //cerr << "random value: " << rnd  << " likelihood=" << likelihood << endl;
    if( likelihood < rnd ){
      valid = false;
    }
  }

  ~ValidWrapper() {
    //cerr << "~ValidWrapper(): destroyed" << endl;
    if( allocated ) {
      getPointer()->~T();
      allocated = false;
    }
  }

private:
  char data[ sizeof( T ) ];

  // token (data) is either valid or faulty
  mutable bool valid;

  // memory allocation flag (data location)
  bool allocated;

  T* getPointer() {
    return reinterpret_cast<T*>(data);
  }

  const T* getPointer() const {
    return reinterpret_cast<const T*>(data);
  }
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_VALIDWRAPPER_HPP
