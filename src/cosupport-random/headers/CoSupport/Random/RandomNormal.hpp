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

#ifndef _INCLUDED_COSUPPORT_RANDOM_RANDOMNORMAL_HPP
#define _INCLUDED_COSUPPORT_RANDOM_RANDOMNORMAL_HPP

#include "randomSource.hpp"
#include "RandomGenerator.hpp"

#include <boost/random/normal_distribution.hpp>

#include <boost/bind.hpp>

#include <istream>

namespace CoSupport { namespace Random {

template <typename T>
struct RandomNormal: public RandomGenerator<T> {
  RandomNormal(T const &mean, T const &sigma)
      : RandomGenerator<T>(boost::bind(
          reinterpret_cast<double (boost::random::normal_distribution<>::*)(boost::random::mt19937 &) const>(
            static_cast<double (boost::random::normal_distribution<>::*)(boost::random::mt19937 &)>(
              &boost::random::normal_distribution<>::operator())),
          boost::random::normal_distribution<>(mean, sigma),
          randomSource)) {}
};


template <typename T>
std::istream &operator >> (std::istream &in, RandomNormal<T> &v) {
  char ch;
  T mean, sigma;
  if ((in.get(ch), ch) != 'm') {
    in.setstate(std::ios::badbit);
    return in;
  }
  in >> mean;
  if (in.fail())
    return in;
  if ((in.get(ch), ch) != ',') {
    in.setstate(std::ios::badbit);
    return in;
  }
  if ((in.get(ch), ch) != 's') {
    in.setstate(std::ios::badbit);
    return in;
  }
  in >> sigma;
  if (in.fail())
    return in;
  v = RandomNormal<T>(mean,sigma);
  return in;
}

} } // namespace CoSupport::Random

#endif //_INCLUDED_COSUPPORT_RANDOM_RANDOMNORMAL_HPP
