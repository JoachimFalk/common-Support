// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
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
  /// The urn vector contains pairs. Each pair consists of
  /// a value and an associated weight, where pair.first is
  /// the weight of the value pair.second.
  RandomUrn(std::vector<std::pair<size_t, T> > const &urn)
      : RandomGenerator<T>(create(urn)) {}
private:
  static boost::function<T (void)> create(std::vector<std::pair<size_t, T> > const &urn) {
    std::map<size_t, T> urnMap;
    size_t              urnEndWeight = 0;
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
  std::vector<std::pair<size_t, T> > urn;

  do {
    char   ch;
    T      value;
    size_t weight = 1;

    in >> value;
    if (in.fail())
      return in;
    if (in.peek() == 'x') {
      in.get(ch); // skip 'x'
      in >> weight;
      if (in.fail())
        return in;
    }
    urn.push_back(std::make_pair(weight, value));
    if (in.peek() == ',') {
      in.get(ch); // skip ','
    } else {
      if (in.eof() && !in.bad())
        in.clear();
      break;
    }
  } while (true);
  v = RandomUrn<T>(urn);
  return in;
}

} } // namespace CoSupport::Random

#endif //_INCLUDED_COSUPPORT_RANDOM_RANDOMURN_HPP
