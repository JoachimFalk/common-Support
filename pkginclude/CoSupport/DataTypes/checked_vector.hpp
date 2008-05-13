// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2004-2006 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_CHECKED_VECTOR_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_CHECKED_VECTOR_HPP

#include <memory>
#include <cassert>

namespace CoSupport { namespace DataTypes {

/**
 * the checked_vector class allows creating of arrays of objects that 
 * (can) have only non-default constructors and are not copy-constructable
 */
template<class T, class A = std::allocator<T> >
class checked_vector {
public:
  typedef A allocator_type;
  typedef typename A::size_type size_type;
  typedef typename A::pointer pointer;
  typedef typename A::reference reference;
  typedef typename A::const_reference const_reference;
private:
  size_type      count;
  allocator_type alloc;
  pointer        first;
public:

  // creates array by calling default-constructor
  checked_vector(size_type count, A alloc = A()) :
    count(count),
    alloc(alloc),
    first(alloc.allocate(count))
  {
    for(size_type i=0; i<count; ++i) {
      new (first + i) T();
    }
  }
  
  // creates array by calling copy-constructor
  checked_vector(size_type count, const_reference t, A alloc = A()) :
    count(count),
    alloc(alloc),
    first(alloc.allocate(count))
  {
    for(size_type i=0; i<count; ++i) {
      new (first + i) T(t);
    }
  }
  
  // creates array by calling the construct-method of a user defined
  // factory object (memory is already allocated, so placement new must be
  // used)
  template<class F>  
  checked_vector(size_type count, F factory = F(), A alloc = A()) :
    count(count),
    alloc(alloc),
    first(alloc.allocate(count))
  {
    for(size_type i=0; i<count; ++i) {
      factory.construct(first + i, i);
    }
  }
  
  ~checked_vector()
  {
    for(size_type i=0; i<count; ++i) {
      alloc.destroy(first + i);
    }
    alloc.deallocate(first, count);
  }
  
  reference operator[](size_type pos)
  {
    assert(pos < count);
    return first[pos];
  }
  
  const_reference operator[](size_type pos) const
  {
    assert(pos < count);
    return first[pos];
  }
  
  size_type size() const
  { return count; }
};

} } // namespace CoSupport::DataTypes

/*
#include <iostream>
#include "par_actor_factory.hpp"

using namespace CoSupport;

struct test
{
  int x;

  test() : x(0) { std::cout << "def. cons" << std::endl; }
  test(const std::string& name, int x) : x(x) { std::cout << "cons " << name << std::endl; }
  ~test() { std::cout << "des" << std::endl; }
  test(const test& t) : x(t.x) { std::cout << "cc" << std::endl; }
  
  struct factory : public par_actor_factory<test> {
    int x;
    
    factory(const std::string& prefix, int x) :
      par_actor_factory<test>(prefix), x(x)
    {}
    
    void construct_helper(test* p, const std::string& name)
    { new (p) test(name, x); }
  };
};

int main() {

  checked_vector<test> v1(2);
  std::cout << v1[1].x << std::endl;
  
  checked_vector<test> v2(3, test("blub", 99));
  std::cout << v2[2].x << std::endl;
  
  checked_vector<test> v3(4, test::factory("blub", 3));
  std::cout << v3[3].x << std::endl;

  return 0;
}
*/

#endif // _INCLUDED_COSUPPORT_DATATYPES_CHECKED_VECTOR_HPP
