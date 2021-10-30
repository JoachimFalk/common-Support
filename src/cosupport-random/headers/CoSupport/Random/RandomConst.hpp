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

#ifndef _INCLUDED_COSUPPORT_RANDOM_RANDOMCONST_HPP
#define _INCLUDED_COSUPPORT_RANDOM_RANDOMCONST_HPP

#include "RandomGenerator.hpp"

#include <istream>

namespace CoSupport { namespace Random {

template <typename T>
struct RandomConst: public RandomGenerator<T> {
  RandomConst(T const &val)
    : RandomGenerator<T>([val] (boost::random::mt19937 &rng) -> T {
        return val;
      }) {}
};

template <typename T>
std::istream &operator >> (std::istream &in, RandomConst<T> &v) {
  T vv;
  in >> vv;
  if (!in.fail())
    v = RandomConst<T>(vv);
  return in;
}

} } // namespace CoSupport::Random

#endif //_INCLUDED_COSUPPORT_RANDOM_RANDOMCONST_HPP
