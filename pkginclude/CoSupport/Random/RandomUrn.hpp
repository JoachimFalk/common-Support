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

#ifndef _INCLUDED_COSUPPORT_RANDOM_RANDOMURN_HPP
#define _INCLUDED_COSUPPORT_RANDOM_RANDOMURN_HPP

#include "randomSource.hpp"
#include "RandomGenerator.hpp"

#include <boost/random/uniform_int_distribution.hpp>

#include <boost/bind.hpp>

#include <istream>

namespace CoSupport { namespace Random {

template <typename T>
struct RandomUrn: public RandomGenerator<T> {
  RandomUrn(std::vector<std::pair<size_t, T> > const &urn)
      : RandomGenerator<T>(create(urn)) {}
private:
  static boost::function<T (void)> create(std::vector<std::pair<size_t, T> > const &urn) {
    std::map<size_t, T> urnMap;
    size_t              urnEndWeight;
    for (typename std::vector<std::pair<size_t, T> >::const_iterator iter = urn.begin();
         iter != urn.end();
         ++iter) {
      urnEndWeight += iter->first;
      urnMap[urnEndWeight-1] = iter->second;
    }
    boost::random::uniform_int_distribution<> urnSelection(0, urnEndWeight-1);
    return boost::bind(&pull, urnSelection, urnMap);
  }

  static T pull(boost::random::uniform_int_distribution<> const &urnSelection, std::map<size_t, T> const &urnMap) {
    return urnMap.lower_bound(urnSelection(randomSource))->second;
  }
};

template <typename T>
std::istream &operator >> (std::istream &in, RandomUrn<T> &v) {
  assert(!"YUP!!!");
  return in;
}

} } // namespace CoSupport::Random

#endif //_INCLUDED_COSUPPORT_RANDOM_RANDOMURN_HPP
