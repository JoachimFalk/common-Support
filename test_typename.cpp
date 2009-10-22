/*
 * Copyright (c) 2004-2009 Hardware-Software-CoDesign, University of
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

#include <cassert>

#include <iostream>

#include <CoSupport/String/TypeName.hpp>

using namespace CoSupport::String;

class A;

COSUPPORT_REGISTER_TYPENAME(A);

template <typename T>
void check_typename_for(const std::string &expected) {
  std::string retval = TypeName<T>::name();
  std::cout << "TypeName<" << expected << ">::name(): \"" << retval << "\"" << std::endl;
  assert(retval == expected);
}

#define CHECK_TYPENAME_FOR(TYPE) \
  check_typename_for<TYPE>(#TYPE)

int main(int argc, char *argv[]) {
  CHECK_TYPENAME_FOR(char);
  CHECK_TYPENAME_FOR(const char);
  CHECK_TYPENAME_FOR(char *);
  CHECK_TYPENAME_FOR(const char *);
  CHECK_TYPENAME_FOR(char *const);
  CHECK_TYPENAME_FOR(const char *const);
  CHECK_TYPENAME_FOR(char **);
  CHECK_TYPENAME_FOR(const char **);
  CHECK_TYPENAME_FOR(char *const *);
  CHECK_TYPENAME_FOR(const char *const *);
  CHECK_TYPENAME_FOR(char **const);
  CHECK_TYPENAME_FOR(const char **const);
  CHECK_TYPENAME_FOR(char *const *const);
  CHECK_TYPENAME_FOR(const char *const *const);
  CHECK_TYPENAME_FOR(char &);
  CHECK_TYPENAME_FOR(const char &);
  CHECK_TYPENAME_FOR(char * &);
  CHECK_TYPENAME_FOR(const char * &);
  CHECK_TYPENAME_FOR(char *const &);
  CHECK_TYPENAME_FOR(const char *const &);
  CHECK_TYPENAME_FOR(char ** &);
  CHECK_TYPENAME_FOR(const char ** &);
  CHECK_TYPENAME_FOR(char *const * &);
  CHECK_TYPENAME_FOR(const char *const * &);
  CHECK_TYPENAME_FOR(char **const &);
  CHECK_TYPENAME_FOR(const char **const &);
  CHECK_TYPENAME_FOR(char *const *const &);
  CHECK_TYPENAME_FOR(const char *const *const &);
  
  CHECK_TYPENAME_FOR(A);
  CHECK_TYPENAME_FOR(const A);
  CHECK_TYPENAME_FOR(A *);
  CHECK_TYPENAME_FOR(const A *);
  CHECK_TYPENAME_FOR(A *const);
  CHECK_TYPENAME_FOR(const A *const);
  CHECK_TYPENAME_FOR(A **);
  CHECK_TYPENAME_FOR(const A **);
  CHECK_TYPENAME_FOR(A *const *);
  CHECK_TYPENAME_FOR(const A *const *);
  CHECK_TYPENAME_FOR(A **const);
  CHECK_TYPENAME_FOR(const A **const);
  CHECK_TYPENAME_FOR(A *const *const);
  CHECK_TYPENAME_FOR(const A *const *const);
  CHECK_TYPENAME_FOR(A &);
  CHECK_TYPENAME_FOR(const A &);
  CHECK_TYPENAME_FOR(A * &);
  CHECK_TYPENAME_FOR(const A * &);
  CHECK_TYPENAME_FOR(A *const &);
  CHECK_TYPENAME_FOR(const A *const &);
  CHECK_TYPENAME_FOR(A ** &);
  CHECK_TYPENAME_FOR(const A ** &);
  CHECK_TYPENAME_FOR(A *const * &);
  CHECK_TYPENAME_FOR(const A *const * &);
  CHECK_TYPENAME_FOR(A **const &);
  CHECK_TYPENAME_FOR(const A **const &);
  CHECK_TYPENAME_FOR(A *const *const &);
  CHECK_TYPENAME_FOR(const A *const *const &);
  return 0;
}
