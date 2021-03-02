// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
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

#ifndef _INCLUDED_COSUPPORT_RANDOM_RANDOMGENERATOR_HPP
#define _INCLUDED_COSUPPORT_RANDOM_RANDOMGENERATOR_HPP

#include <functional>

namespace CoSupport { namespace Random {

template <typename T>
struct RandomGenerator: public std::function<T (void)> {
  RandomGenerator() {}

  template <typename TT>
  RandomGenerator(TT const &f)
      : std::function<T (void)>(f) {}
};

/*
namespace boost {

  template <>
  RandomGenerator<size_t> lexical_cast<RandomGenerator<size_t>, std::string>(const std::string &arg) {
    char *endptr;
    long  val1, val2;
    if (val1 = strtol(arg.c_str(), &endptr, 10), *endptr == '\0') {
      return RandomConst<size_t>(val1); // constant value
    } else if (arg.substr(0,sizeof("uniform:")-1) == "uniform:" &&
        (val1 = strtol(arg.c_str()+sizeof("uniform:")-1, &endptr, 10), *endptr == '-') &&
        (val2 = strtol(endptr+1, &endptr, 10), *endptr == '\0')) {
      return RandomUniform<size_t>(val1, val2); // uniform distribution
    } else if (arg.substr(0,sizeof("normal:m")-1) == "normal:m" &&
        (val1 = strtol(arg.c_str()+sizeof("normal:m")-1, &endptr, 10), endptr[0] == ',' && endptr[1] == 's' ) &&
        (val2 = strtol(endptr+2, &endptr, 10), *endptr == '\0')) {
      return RandomNormal<size_t>(val1, val2);
    }
    throw boost::bad_lexical_cast();
  }

  template <>
  RandomGenerator<double> lexical_cast<RandomGenerator<double>, std::string>(const std::string &arg) {
    char *endptr;
    double  val1, val2;
    if (val1 = strtod(arg.c_str(), &endptr), *endptr == '\0') {
      return RandomConst<double>(val1); // constant value
    } else if (arg.substr(0,sizeof("uniform:")-1) == "uniform:" &&
        (val1 = strtod(arg.c_str()+sizeof("uniform:")-1, &endptr), *endptr == '-') &&
        (val2 = strtod(endptr+1, &endptr), *endptr == '\0')) {
      return RandomUniform<size_t>(val1, val2); // uniform distribution
    } else if (arg.substr(0,sizeof("normal:m")-1) == "normal:m" &&
        (val1 = strtod(arg.c_str()+sizeof("normal:m")-1, &endptr), endptr[0] == ',' && endptr[1] == 's' ) &&
        (val2 = strtod(endptr+2, &endptr), *endptr == '\0')) {
      return RandomNormal<double>(val1, val2);
    }
    throw boost::bad_lexical_cast();
  }

} // namespace boost
*/

} } // namespace CoSupport::Random

#endif //_INCLUDED_COSUPPORT_RANDOM_RANDOMGENERATOR_HPP
