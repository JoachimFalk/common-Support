//  -*- tab-width:8; intent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 expandtab:
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

#ifndef VALIDWRAPPER_HPP
#define VALIDWRAPPER_HPP

#include <cassert>
#include <new>
#include <cstdlib> 

namespace ValidWrapper {

    /// test code
  static double testingLikelihood;




  template<class T>
  class VW {
  public:
    typedef VW<T> type;

    VW()
      :valid(true),
       allocated(false) {
      //cerr << "VW()" << endl;
      randError(testingLikelihood);
    }

    VW( const T& t )
      :valid(true),
       allocated(true) {
      //cerr << "VW(const T& t)" << endl;
      new(data) T(t);
      randError(testingLikelihood);
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
  
    ~VW() {
      //cerr << "~VW(): destroyed" << endl;
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
}

#endif // VALIDWRAPPER_HPP
