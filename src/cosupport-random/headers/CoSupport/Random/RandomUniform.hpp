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

#ifndef _INCLUDED_COSUPPORT_RANDOM_RANDOMUNIFORM_HPP
#define _INCLUDED_COSUPPORT_RANDOM_RANDOMUNIFORM_HPP

#include <CoSupport/compatibility-glue/nullptr.h>

#include "randomSource.hpp"
#include "RandomGenerator.hpp"

#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>

#include <boost/bind.hpp>

#include <istream>

namespace CoSupport { namespace Random {

template <typename T>
struct RandomUniform: public RandomGenerator<T> {
  RandomUniform(T const &min, T const &max)
      : RandomGenerator<T>(create(min, max, (typename boost::is_integral<T>::type *) nullptr)) {}
private:
  static boost::function<T (void)> create(T const &min, T const &max, boost::true_type *) {
    boost::random::uniform_int_distribution<> dist(min, max);
    int (boost::random::uniform_int_distribution<>::* fun)(boost::random::mt19937 &) const =
        &boost::random::uniform_int_distribution<>::operator();
    return boost::bind(fun, dist, randomSource);
  }
  static boost::function<T (void)> create(T const &min, T const &max, boost::false_type *) {
    boost::random::uniform_real_distribution<> dist(min, max);
    double (boost::random::uniform_real_distribution<>::* fun)(boost::random::mt19937 &) const =
        &boost::random::uniform_real_distribution<>::operator();
    return boost::bind(fun, dist, randomSource);
  }
};

template <typename T>
std::istream &operator >> (std::istream &in, RandomUniform<T> &v) {
  T min, max;
  in >> min;
  if (in.fail())
    return in;
  char ch; in.get(ch);
  if (ch != '-') {
    in.setstate(std::ios::badbit);
    return in;
  }
  in >> max;
  if (in.fail())
    return in;
  v = RandomUniform<T>(min,max);
  return in;
}

} } // namespace CoSupport::Random

#endif //_INCLUDED_COSUPPORT_RANDOM_RANDOMUNIFORM_HPP
