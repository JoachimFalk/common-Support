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

#include <map>

#include <CoSupport/Streams/stl_output_for_set.hpp>
#include <CoSupport/Streams/stl_output_for_map.hpp>

#include <CoSupport/DataTypes/Set.hpp>
#include <CoSupport/DataTypes/SetVirtual.hpp>
#include <CoSupport/DataTypes/SetFacade.hpp>

#include <cstring>

typedef CoSupport::DataTypes::Set<int> TSet;
typedef std::map<TSet, int>            TSetInMap;

//typedef CoSupport::DataTypes::Set<std::string, CoSupport::DataTypes::SetVirtualInterface> VSetImpl;
//typedef std::map<VSetImpl, int>                                                           VSetImplInMap;

typedef CoSupport::DataTypes::SetVirtual<std::string> VSet;
typedef std::map<VSet, int>                           VSetInMap;

typedef CoSupport::DataTypes::SetFacade<std::string> FSet;
typedef std::map<FSet, int>                          FSetInMap;

int main(int argc, char *argv[]) {
  {
    TSet set;
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

    TSetInMap  map;
    map.insert(std::make_pair(set, 13));
    for(TSet::iterator iter = set.begin();
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
    for(TSet::const_iterator iter = set.begin();
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
      pa = NULL;
      assert(ra.size() == 2);
      pa = &ra;
    }
    assert(pa->size() == 2);
    {
      FSet::iterator iter = pa->begin();
      assert(*iter == "bar");
      assert(*--pa->end() == "foo");
      pa = NULL;
      assert(*iter == "bar");
    }
  }

/*    FSet::Ref       ra(a);
      FSet::ConstRef  cra1(a);
      FSet::ConstRef  cra2(ra);
      FSet::Ptr       pa(a.toPtr());
      FSet::ConstPtr  cpa1(pa);
      FSet::ConstPtr  cpa2(a.toPtr()); */
  return 0;
}
