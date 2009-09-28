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

#include <iostream>
#include <cassert>

#include <CoSupport/DataTypes/MaybeValue.hpp>

#define CHECK_OP(X,OP,V) do { \
    std::cout << #X " " #OP " " << #V << ": " << (X OP V) << std::endl; \
    assert(!X.isDefined() || (X OP V) == (X.get() OP V)); \
  } while (0)

using namespace CoSupport::DataTypes;

int main(int argc, char *argv[]) {
  MaybeValue<int> foo;
  
  std::cout << "foo(): " << foo << std::endl;
  
  assert(!foo.isDefined());
  
  foo = 13;
  std::cout << "foo = 13: " << foo << std::endl;
  
  foo += 1;
  std::cout << "foo += 1: " << foo << std::endl;
  std::cout << "++foo:    " << ++foo << std::endl;
  
  foo -= 1;
  std::cout << "foo -= 1: " << foo << std::endl;
  std::cout << "--foo:    " << --foo << std::endl;
  
  std::cout << "foo++:    " << foo++ << std::endl;
  std::cout << "foo:      " << foo << std::endl;
  
  std::cout << "foo--:    " << foo-- << std::endl;
  std::cout << "foo:      " << foo << std::endl;
  
  foo *= 3;
  std::cout << "foo *= 3: " << foo << std::endl;
  
  foo /= 4;
  std::cout << "foo /= 4: " << foo << std::endl;
  
  foo %= 5;
  std::cout << "foo %= 5: " << foo << std::endl;
  assert(foo.isDefined());
  assert(foo.get() == 4);
  
  foo = 4711;
  std::cout << "foo = 4711: " << foo << std::endl;
  CHECK_OP(foo,==,4710); CHECK_OP(foo,==,4711LL); CHECK_OP(foo,==,4712);
  CHECK_OP(foo,!=,4710); CHECK_OP(foo,!=,4711LL); CHECK_OP(foo,!=,4712);
  CHECK_OP(foo,< ,4710); CHECK_OP(foo,< ,4711LL); CHECK_OP(foo,< ,4712);
  CHECK_OP(foo,<=,4710); CHECK_OP(foo,<=,4711LL); CHECK_OP(foo,<=,4712);
  CHECK_OP(foo,> ,4710); CHECK_OP(foo,> ,4711LL); CHECK_OP(foo,> ,4712);
  CHECK_OP(foo,>=,4710); CHECK_OP(foo,>=,4711LL); CHECK_OP(foo,>=,4712);
  
  foo = boost::blank();
  std::cout << "foo = boost::blank(): " << foo << std::endl;
  assert(!foo.isDefined());
  
  MaybeValue<int> bar;
  
  std::cout << "bar: " << bar << std::endl;
  
  CHECK_OP(foo,==,13); CHECK_OP(foo,==,bar);
  CHECK_OP(foo,!=,13); CHECK_OP(foo,!=,bar);
  CHECK_OP(foo,< ,13); CHECK_OP(foo,< ,bar);
  CHECK_OP(foo,<=,13); CHECK_OP(foo,<=,bar);
  CHECK_OP(foo,> ,13); CHECK_OP(foo,> ,bar);
  CHECK_OP(foo,>=,13); CHECK_OP(foo,>=,bar);
  
  bar = 17U;
  
  std::cout << "bar = 17: " << bar << std::endl;
  
  assert(bar.isDefined());
  assert(bar.get() == 17);
  
  bar.set(foo);
  std::cout << "bar: " << bar << std::endl;
  
  assert(!bar.isDefined());
  
  bar.set(33);
  std::cout << "bar: " << bar << std::endl;
  
  MaybeValue<long> batz(bar);
  
  std::cout << "batz: " << batz << std::endl;
  
  assert(batz.isDefined());
  assert(batz.get() == 33);
  
  batz.undef();
  std::cout << "batz: " << batz << std::endl;
  
  foo.set(batz);
  std::cout << "foo: " << foo << std::endl;
  
  return 0;
}
