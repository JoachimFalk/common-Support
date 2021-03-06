// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_CHECKED_VECTOR_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_CHECKED_VECTOR_HPP

#include <memory>
#include <cassert>

namespace CoSupport { namespace DataTypes {

/**
 * the CheckedVector class allows creating of arrays of objects that 
 * (can) have only non-default constructors and are not copy-constructable
 */
template<class T, class A = std::allocator<T> >
class CheckedVector {
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
  CheckedVector(size_type count, A alloc = A()) :
    count(count),
    alloc(alloc),
    first(alloc.allocate(count))
  {
    for(size_type i=0; i<count; ++i) {
      new (first + i) T();
    }
  }
  
  // creates array by calling copy-constructor
  CheckedVector(size_type count, const_reference t, A alloc = A()) :
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
  CheckedVector(size_type count, F factory = F(), A alloc = A()) :
    count(count),
    alloc(alloc),
    first(alloc.allocate(count))
  {
    for(size_type i=0; i<count; ++i) {
      factory.construct(first + i, i);
    }
  }
  
  ~CheckedVector()
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

using namespace CoSupport;

struct test
{
  int x;

  test() : x(0) { std::cout << "def. cons" << std::endl; }
  test(const std::string& name, int x) : x(x) { std::cout << "cons " << name << std::endl; }
  ~test() { std::cout << "des" << std::endl; }
  test(const test& t) : x(t.x) { std::cout << "cc" << std::endl; }
  
  struct factory {
    std::string prefix;
    int x;
    
    factory(const std::string& prefix, int x) :
      prefix(prefix), x(x)
    {}
    
    void construct(test* p, size_t instance)
    { new (p) test(Concat(prefix)(".")(instance), x); }
  };
};

int main() {

  CheckedVector<test> v1(2);
  std::cout << v1[1].x << std::endl;
  
  CheckedVector<test> v2(3, test("blub", 99));
  std::cout << v2[2].x << std::endl;
  
  CheckedVector<test> v3(4, test::factory("blub", 3));
  std::cout << v3[3].x << std::endl;

  return 0;
}
*/

#endif // _INCLUDED_COSUPPORT_DATATYPES_CHECKED_VECTOR_HPP
