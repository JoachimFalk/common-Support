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

#ifndef _INCLUDED_COSUPPORT_RANDOM_RANDOMGENERATOR_HPP
#define _INCLUDED_COSUPPORT_RANDOM_RANDOMGENERATOR_HPP

#include <boost/function.hpp>

namespace CoSupport { namespace Random {

template <typename T>
struct RandomGenerator: public boost::function<T (void)> {
  RandomGenerator() {}

  template <typename TT>
  RandomGenerator(TT const &f)
      : boost::function<T (void)>(f) {}
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
