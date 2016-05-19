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
#include <algorithm>

#include <map>

#include <CoSupport/compatibility-glue/nullptr.h>

#include <CoSupport/Streams/stl_output_for_set.hpp>
#include <CoSupport/Streams/stl_output_for_map.hpp>

#include <CoSupport/DataTypes/SetInterface.hpp>
#include <CoSupport/DataTypes/SetVirtual.hpp>
#include <CoSupport/DataTypes/SetFacade.hpp>
#include <CoSupport/DataTypes/Iter/BidirectionalTraversalExampleImpl.hpp>

#include <cstring>

#include <set>

/// This class implements the interface for a std::set containing values of type T.
template <
  typename T,
  typename R  = typename boost::add_reference<T>::type,
  typename CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
  typename P  = typename boost::add_pointer<T>::type,
  typename CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
>
class Set
: public CoSupport::DataTypes::SetInterface<
    Set<T,R,CR,P,CP>,
    CoSupport::DataTypes::Iter::BidirectionalTraversalExampleImpl,
    T,R,CR,P,CP>
{
  typedef Set<T,R,CR,P,CP>                                          this_type;
  typedef CoSupport::DataTypes::SetInterface<
    this_type,
    CoSupport::DataTypes::Iter::BidirectionalTraversalExampleImpl,
    T,R,CR,P,CP>                                                    base_type;

  friend class CoSupport::DataTypes::SetInterface<
    this_type,
    CoSupport::DataTypes::Iter::BidirectionalTraversalExampleImpl,
    T,R,CR,P,CP>;
  friend class CoSupport::DataTypes::Iter::ContainerAccessor<this_type>;
  friend class CoSupport::DataTypes::Iter::ContainerAccessor<this_type const>;

  typedef typename std::set<T>::iterator UnderlyingIterator;
public:
  Set()
    : set() {}
  Set(this_type const &val)
    : set(val.set) {}
  template <class DD>
  Set(std::set<DD> const &val)
    : set(val.begin(), val.end()) {}
  template <class DD, template<class> class II, class RR, class CRCR, class PP, class CPCP>
  Set(CoSupport::DataTypes::SetInterface<DD,II,T,RR,CRCR,PP,CPCP> const &val)
    : set(val.begin(), val.end()) {}
protected:
  std::set<T> set;

  typename this_type::iterator implBegin() const
    { return const_cast<this_type *>(this)->set.begin(); }
  typename this_type::iterator implEnd() const
    { return const_cast<this_type *>(this)->set.end(); }

  using base_type::implErase;

  void
  implErase(const typename this_type::iterator &iter)
    { set.erase(iter.iter); }
  std::pair<typename this_type::iterator, bool>
  implInsert(const typename this_type::value_type &value) {
    std::pair<typename std::set<T>::iterator, bool> retval(set.insert(value));
    return std::make_pair(
      typename this_type::iterator(retval.first),
      retval.second);
  }
};

typedef Set<int>               TIntSet;
typedef std::map<TIntSet, int> TIntSetInMap;

//typedef CoSupport::DataTypes::Set<std::string, CoSupport::DataTypes::SetVirtualInterface> VSetImpl;
//typedef std::map<VSetImpl, int>                                                           VSetImplInMap;

typedef Set<std::string>                              TSet;
typedef std::map<TSet, int>                           TSetInMap;

typedef CoSupport::DataTypes::SetVirtual<std::string> VSet;
typedef std::map<VSet, int>                           VSetInMap;

typedef CoSupport::DataTypes::SetFacade<std::string>  FSet;
typedef std::map<FSet, int>                           FSetInMap;

int main(int argc, char *argv[]) {
  {
    TIntSet set;
    set.insert(3);
    set.insert(1);
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 2);
    assert(*set.begin() == 1);
    assert(*++set.begin() == 3);
    assert(*--set.end() == 3);

    set.insert(2);
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 3);
    assert(*++set.begin() == 2);

    set.erase(1);
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 2);
    assert(*set.begin() == 2);
    assert(*++set.begin() == 3);

    set.erase(--set.end());
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 1);
    assert(*set.begin() == 2);

    TIntSetInMap  map;
    map.insert(std::make_pair(set, 13));
    for(TIntSet::iterator iter = set.begin();
        iter != set.end();
        ++iter) {
      if (iter != set.begin())
        std::cout << ", ";
      std::cout << *iter;
      int v = *iter;
      set.erase(iter);
      iter = set.insert(2*v).first;
      map.insert(std::make_pair(set, 13));
    }
    std::cout << std::endl;
    for(TIntSet::const_iterator iter = set.begin();
        iter != set.end();
        ++iter) {
      if (iter != set.begin())
        std::cout << ", ";
      std::cout << *iter;
    }
    std::cout << std::endl;
    set.erase(set.begin(), set.end());
    assert(set.empty());
    map.insert(std::make_pair(set, 33));
    {
      TIntSet::iterator iter(set.begin());
      iter = set.begin();
      TIntSet::const_iterator citer(iter);
      citer = iter;
    }
  }
/*{
    VSetImpl set;
    set.insert("hax");
    set.insert("bar");
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 2);
    assert(*set.begin() == "bar");
    assert(*++set.begin() == "hax");
    assert(*--set.end() == "hax");

    set.insert("foo");
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 3);
    assert(*set.begin() == "bar");
    assert(*++set.begin() == "foo");
    assert(*++(++set.begin()) == "hax");
    assert(*--set.end() == "hax");
    assert(*--(--set.end()) == "foo");
    assert(*--(--(--set.end())) == "bar");

    set.erase("bar");
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 2);
    assert(*set.begin() == "foo");
    assert(*++set.begin() == "hax");

    set.erase(--set.end());
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 1);
    assert(*set.begin() == "foo");
    assert(*--set.end() == "foo");

    set.erase(set.begin(), set.end());
    assert(set.empty());
    set.insert("hko");

    VSetImplInMap  map;
    map.insert(std::make_pair(set, 4711));
    for (int i = 4712; i < 4718; ++i) {
      VSetImpl::iterator iter, niter;
      for(iter = set.begin();
          iter != set.end();
          iter = niter) {
        if (iter != set.begin())
          std::cout << ", ";
        std::cout << *iter;
        std::string v = *iter;
        ++(niter = iter);
        set.erase(iter);
        for (size_t j = 0; j < v.length(); ++j) {
          --v[j];
          iter = set.insert(v).first;
          assert(*iter == v);
          map.insert(std::make_pair(set, i));
        }
      }
      std::cout << std::endl;
    }
    map.insert(std::make_pair(set, 33));
    std::cout << map << std::endl;
  }*/
  {
    VSet set;

    set.insert("hax");
    set.insert("bar");
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 2);
    assert(*set.begin() == "bar");
    assert(*++set.begin() == "hax");
    assert(*--set.end() == "hax");

    set.insert("foo");
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 3);
    assert(*set.begin() == "bar");
    assert(*++set.begin() == "foo");
    assert(*++(++set.begin()) == "hax");
    assert(*--set.end() == "hax");
    assert(*--(--set.end()) == "foo");
    assert(*--(--(--set.end())) == "bar");

    set.erase("bar");
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 2);
    assert(*set.begin() == "foo");
    assert(*++set.begin() == "hax");

    set.erase(--set.end());
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 1);
    assert(*set.begin() == "foo");
    assert(*--set.end() == "foo");

    set.erase(set.begin(), set.end());
    assert(set.empty());
    set.insert("hko");

    VSetInMap  map;
    map.insert(std::make_pair(set, 4711));
    for (int i = 4712; i < 4718; ++i) {
      VSet::iterator iter, niter;
      for(iter = set.begin();
          iter != set.end();
          iter = niter) {
        if (iter != set.begin())
          std::cout << ", ";
        std::cout << *iter;
        std::string v = *iter;
        ++(niter = iter);
        set.erase(iter);
        for (size_t j = 0; j < v.length(); ++j) {
          --v[j];
          iter = set.insert(v).first;
          assert(*iter == v);
          map.insert(std::make_pair(set, i));
        }
      }
      std::cout << std::endl;
    }
    map.insert(std::make_pair(set, 33));
    std::cout << map << std::endl;
    for(VSet::iterator iter = set.begin();
        iter != set.end();
        ++iter) {
      if (iter != set.begin())
        std::cout << ", ";
      std::cout << *iter;
      std::string v = *iter;
      set.erase(iter);
      std::reverse(v.begin(), v.end());
      iter = set.insert(v).first;
      map.insert(std::make_pair(set, 13));
    }
    std::cout << std::endl;
    for(VSet::const_iterator iter = set.begin();
        iter != set.end();
        ++iter) {
      if (iter != set.begin())
        std::cout << ", ";
      std::cout << *iter;
    }
    std::cout << std::endl;
    set.erase(set.begin(), set.end());
    assert(set.empty());
    {
      VSet::iterator iter(set.begin());
      iter = set.begin();
      VSet::const_iterator citer(iter);
      citer = iter;
    }
  }
  {
    FSet set;

    set.insert("hax");
    set.insert("bar");
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 2);
    assert(*set.begin() == "bar");
    assert(*++set.begin() == "hax");
    assert(*--set.end() == "hax");

    set.insert("foo");
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 3);
    assert(*set.begin() == "bar");
    assert(*++set.begin() == "foo");
    assert(*++(++set.begin()) == "hax");
    assert(*--set.end() == "hax");
    assert(*--(--set.end()) == "foo");
    assert(*--(--(--set.end())) == "bar");

    set.erase("bar");
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 2);
    assert(*set.begin() == "foo");
    assert(*++set.begin() == "hax");

    set.erase(--set.end());
    std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
    assert(set.size() == 1);
    assert(*set.begin() == "foo");
    assert(*--set.end() == "foo");

    set.erase(set.begin(), set.end());
    assert(set.empty());
    set.insert("hko");

    {
      FSet::iterator iter = ++set.begin();
      FSet::const_iterator citer(iter);
      citer = iter;
    }
    FSetInMap  map;
    map.insert(std::make_pair(set, 4711));
    for (int i = 4712; i < 4718; ++i) {
      FSet::iterator iter, niter;
      for(iter = set.begin();
          iter != set.end();
          iter = niter) {
        if (iter != set.begin())
          std::cout << ", ";
        std::cout << *iter;
        std::string v = *iter;
        ++(niter = iter);
        set.erase(iter);
        for (size_t j = 0; j < v.length(); ++j) {
          --v[j];
          iter = set.insert(v).first;
          assert(*iter == v);
          map.insert(std::make_pair(set, i));
        }
      }
      std::cout << std::endl;
    }
    map.insert(std::make_pair(set, 33));
    std::cout << map << std::endl;
    {
      FSet::iterator iter(set.begin());
      iter = set.begin();
      FSet::const_iterator citer(iter);
      citer = iter;
    }
  }
  {
    FSet::Ptr pa;
    {
      FSet a;
      a.insert("foo");
      pa = &a;
      FSet::ConstPtr  cpa1(pa);
      FSet::ConstPtr  cpa2(a.toPtr());
    }
    assert(pa->size() == 1);
    {
      FSet            b(*pa);
      FSet::Ref       rb(b);
      FSet::ConstRef  crb1(b);
      FSet::ConstRef  crb2(rb);
      
      assert(b.size() == 1);
      rb.insert("aaa");
      assert(*b.begin() == "aaa");
      assert(b.size() == 2);
      assert(pa->size() == 1);
      
      FSet::Ref ra(*pa);
      ra.insert("bar");
      assert(pa->size() == 2);
      pa = nullptr;
      assert(ra.size() == 2);
      pa = &ra;
    }
    assert(pa->size() == 2);
    {
      FSet::iterator iter = pa->begin();
      assert(*iter == "bar");
      assert(*--pa->end() == "foo");
      pa = nullptr;
      assert(*iter == "bar");
    }
  }
  {
    // Check various copy operations of sets
    std::set<std::string>       sSet;
    TSet                        tSet;
    VSet                        vSet;
    FSet                        fSet;
    std::set<std::string> const csSet;
    TSet                  const ctSet;
    VSet                  const cvSet;
    FSet                  const cfSet;
    {
      std::set<std::string> dst1(sSet);
      std::set<std::string> dst2(tSet);
      std::set<std::string> dst3(vSet);
      std::set<std::string> dst4(fSet);
    }
    {
      std::set<std::string> dst1(csSet);
      std::set<std::string> dst2(ctSet);
      std::set<std::string> dst3(cvSet);
      std::set<std::string> dst4(cfSet);
    }
    {
      TSet dst1(sSet);
      TSet dst2(tSet);
      TSet dst3(vSet);
      TSet dst4(fSet);
    }
    {
      TSet dst1(csSet);
      TSet dst2(ctSet);
      TSet dst3(cvSet);
      TSet dst4(cfSet);
    }
    {
      VSet dst1(sSet);
      VSet dst2(tSet);
      VSet dst3(vSet);
      VSet dst4(fSet);
    }
    {
      VSet dst1(csSet);
      VSet dst2(ctSet);
      VSet dst3(cvSet);
      VSet dst4(cfSet);
    }
    {
      FSet dst1(sSet);
      FSet dst2(tSet);
      FSet dst3(vSet);
      FSet dst4(fSet);
    }
    {
      FSet dst1(csSet);
      FSet dst2(ctSet);
      FSet dst3(cvSet);
      FSet dst4(cfSet);
    }
  }
  return 0;
}
