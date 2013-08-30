/*
 * Copyright (c) 2013-2013 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_RANDOM_RANDOMGENERATORS_HPP
#define _INCLUDED_COSUPPORT_RANDOM_RANDOMGENERATORS_HPP

#include "RandomConst.hpp"
#include "RandomNormal.hpp"
#include "RandomUniform.hpp"
#include "RandomUrn.hpp"

namespace CoSupport { namespace Random {

template <typename T>
std::istream &operator >> (std::istream &in, RandomGenerator<T> &v) {
  switch (in.peek()) {
    case 'u': {
      // either uniform: or urn:
      char distName[std::max(
          sizeof("uniform:"),
          sizeof("urn:"))-1];
      in.getline(distName, sizeof(distName), ':');
      if (in.fail() || in.eof()) {
        in.setstate(std::ios::badbit);
        return in;
      }
      if (strcmp(distName, "uniform") == 0)
        return in >> static_cast<RandomUniform<T> &>(v);
      if (strcmp(distName, "urn") == 0)
        return in >> static_cast<RandomUrn<T> &>(v);
      in.setstate(std::ios::badbit);
      return in;
    }
    case 'n': {
      // must be normal:
      char distName[sizeof("normal:")-1];
      in.read(distName, sizeof(distName));
      if (in.fail() || in.eof()) {
        in.setstate(std::ios::badbit);
        return in;
      }
      if (strncmp(distName, "normal:", sizeof(distName)) != 0) {
        in.setstate(std::ios::badbit);
        return in;
      }
      return in >> static_cast<RandomNormal<T> &>(v);
    }
    case 'c': {
      // must be const:
      char distName[sizeof("const:")-1];
      in.read(distName, sizeof(distName));
      if (in.fail() || in.eof()) {
        in.setstate(std::ios::badbit);
        return in;
      }
      if (strncmp(distName, "const:", sizeof(distName)) != 0) {
        in.setstate(std::ios::badbit);
        return in;
      }
      return in >> static_cast<RandomConst<T> &>(v);
    }
    default:
      return in >> static_cast<RandomConst<T> &>(v);
  }
}

} } // namespace CoSupport::Random

#endif //_INCLUDED_COSUPPORT_RANDOM_RANDOMGENERATORS_HPP
