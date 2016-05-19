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

#include <CoSupport/Streams/stl_output_for_list.hpp>
#include <CoSupport/Streams/stl_output_for_map.hpp>

#include <CoSupport/DataTypes/ListInterface.hpp>
#include <CoSupport/DataTypes/ListVirtual.hpp>
#include <CoSupport/DataTypes/ListFacade.hpp>
#include <CoSupport/DataTypes/Iter/BidirectionalTraversalExampleImpl.hpp>

#include <cstring>

#include <list>

/// This class implements the interface for a std::list containing values of type T.
template <
  typename T,
  typename R  = typename boost::add_reference<T>::type,
  typename CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
  typename P  = typename boost::add_pointer<T>::type,
  typename CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
>
class List
: public CoSupport::DataTypes::ListInterface<
    List<T,R,CR,P,CP>,
    CoSupport::DataTypes::Iter::BidirectionalTraversalExampleImpl,
    T,R,CR,P,CP>
{
  typedef List<T,R,CR,P,CP>                                         this_type;
  typedef CoSupport::DataTypes::ListInterface<
    this_type,
    CoSupport::DataTypes::Iter::BidirectionalTraversalExampleImpl,
    T,R,CR,P,CP>                                                    base_type;

  friend class CoSupport::DataTypes::ListInterface<
    this_type,
    CoSupport::DataTypes::Iter::BidirectionalTraversalExampleImpl,
    T,R,CR,P,CP>;
  friend class CoSupport::DataTypes::Iter::ContainerAccessor<this_type>;
  friend class CoSupport::DataTypes::Iter::ContainerAccessor<this_type const>;

  typedef typename std::list<T>::iterator UnderlyingIterator;
public:
  List()
    : list() {}
  List(this_type const &val)
    : list(val.list) {}
  template <class DD>
  List(std::list<DD> const &val)
    : list(val.begin(), val.end()) {}
  template <class DD, template<class> class II, class RR, class CRCR, class PP, class CPCP>
  List(CoSupport::DataTypes::ListInterface<DD,II,T,RR,CRCR,PP,CPCP> const &val)
    : list(val.begin(), val.end()) {}
protected:
  std::list<T> list;

  typename this_type::iterator implBegin() const
    { return const_cast<this_type *>(this)->list.begin(); }
  typename this_type::iterator implEnd() const
    { return const_cast<this_type *>(this)->list.end(); }

  using base_type::implErase;

  typename this_type::iterator
  implErase(const typename this_type::iterator &iter)
    { return list.erase(iter.iter); }
  typename this_type::iterator
  implInsert(typename this_type::iterator iter, const typename this_type::value_type &value)
    { return list.insert(iter.iter, value); }
};

typedef List<int>                              TList;
typedef std::map<TList, int>                   TListInMap;

typedef CoSupport::DataTypes::ListVirtual<int> VList;
typedef std::map<VList, int>                   VListInMap;

typedef CoSupport::DataTypes::ListFacade<int>  FList;
typedef std::map<FList, int>                   FListInMap;

int main(int argc, char *argv[]) {
  {
    TList list;
    list.push_back(1);
    list.push_back(2);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 2);
    assert(*list.begin() == 1);
    assert(*++list.begin() == 2);
    list.insert(list.begin(), 0);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 3);
    assert(*list.begin() == 0);
    assert(*++list.begin() == 1);
    assert(*--list.end() == 2);
    list.push_front(-1);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 4);
    assert(*list.begin() == -1);
    assert(*++list.begin() == 0);
    assert(*++(++list.begin()) == 1);
    assert(*--list.end() == 2);
    list.erase(--list.end());
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 3);
    assert(*list.begin() == -1);
    assert(*--(--list.end()) == 0);
    assert(*--list.end() == 1);
    list.erase(--(--list.end()));
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 2);
    assert(*list.begin() == -1);
    assert(*--list.end() == 1);
    assert(list.front() == -1);
    assert(list.back() == 1);
    list.insert(++list.begin(), 2, 55);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 4);
    assert(*list.begin() == -1);
    assert(*++list.begin() == 55);
    assert(*++(++list.begin()) == 55);
    assert(*--list.end() == 1);

    TListInMap  map;
    map.insert(std::make_pair(list, 13));
    std::cout << "list: {";
    for(TList::iterator iter = list.begin();
        iter != list.end();
        ++iter) {
      if (iter != list.begin())
        std::cout << ", ";
      std::cout << *iter;
      int v = *iter;
      iter = list.insert(list.erase(iter), 2*v);
      map.insert(std::make_pair(list, 13));
    }
    std::cout << "}" << std::endl;
    std::cout << "list: {";
    for(TList::const_iterator iter = list.begin();
        iter != list.end();
        ++iter) {
      if (iter != list.begin())
        std::cout << ", ";
      std::cout << *iter;
    }
    std::cout << "}" << std::endl;
    list.erase(list.begin(), list.end());
    assert(list.empty());
    map.insert(std::make_pair(list, 33));
    std::cout << map << std::endl;
    {
      TList::iterator iter(list.begin());
      iter = list.begin();
      TList::const_iterator citer(iter);
      citer = iter;
    }
  }
  {
    VList list;
    list.push_back(1);
    list.push_back(2);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 2);
    assert(*list.begin() == 1);
    assert(*++list.begin() == 2);
    list.insert(list.begin(), 0);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 3);
    assert(*list.begin() == 0);
    assert(*++list.begin() == 1);
    assert(*--list.end() == 2);
    list.push_front(-1);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 4);
    assert(*list.begin() == -1);
    assert(*++list.begin() == 0);
    assert(*++(++list.begin()) == 1);
    assert(*--list.end() == 2);
    list.erase(--list.end());
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 3);
    assert(*list.begin() == -1);
    assert(*--(--list.end()) == 0);
    assert(*--list.end() == 1);
    list.erase(--(--list.end()));
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 2);
    assert(*list.begin() == -1);
    assert(*--list.end() == 1);
    assert(list.front() == -1);
    assert(list.back() == 1);
    list.insert(++list.begin(), 2, 55);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 4);
    assert(*list.begin() == -1);
    assert(*++list.begin() == 55);
    assert(*++(++list.begin()) == 55);
    assert(*--list.end() == 1);

    VListInMap  map;
    map.insert(std::make_pair(list, 13));
    std::cout << "list: {";
    for(VList::iterator iter = list.begin();
        iter != list.end();
        ++iter) {
      if (iter != list.begin())
        std::cout << ", ";
      std::cout << *iter;
      int v = *iter;
      iter = list.insert(list.erase(iter), 2*v);
      map.insert(std::make_pair(list, 13));
    }
    std::cout << "}" << std::endl;
    std::cout << "list: {";
    for(VList::const_iterator iter = list.begin();
        iter != list.end();
        ++iter) {
      if (iter != list.begin())
        std::cout << ", ";
      std::cout << *iter;
    }
    std::cout << "}" << std::endl;
    list.erase(list.begin(), list.end());
    assert(list.empty());
    map.insert(std::make_pair(list, 33));
    std::cout << map << std::endl;
    {
      VList::iterator iter(list.begin());
      iter = list.begin();
      VList::const_iterator citer(iter);
      citer = iter;
    }
  }
  {
    FList list;
    list.push_back(1);
    list.push_back(2);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 2);
    assert(*list.begin() == 1);
    assert(*++list.begin() == 2);
    list.insert(list.begin(), 0);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 3);
    assert(*list.begin() == 0);
    assert(*++list.begin() == 1);
    assert(*--list.end() == 2);
    list.push_front(-1);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 4);
    assert(*list.begin() == -1);
    assert(*++list.begin() == 0);
    assert(*++(++list.begin()) == 1);
    assert(*--list.end() == 2);
    list.erase(--list.end());
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 3);
    assert(*list.begin() == -1);
    assert(*--(--list.end()) == 0);
    assert(*--list.end() == 1);
    list.erase(--(--list.end()));
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 2);
    assert(*list.begin() == -1);
    assert(*--list.end() == 1);
    assert(list.front() == -1);
    assert(list.back() == 1);
    list.insert(++list.begin(), 2, 55);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    assert(list.size() == 4);
    assert(*list.begin() == -1);
    assert(*++list.begin() == 55);
    assert(*++(++list.begin()) == 55);
    assert(*--list.end() == 1);

    FListInMap  map;
    map.insert(std::make_pair(list, 13));
    std::cout << "list: {";
    for(FList::iterator iter = list.begin();
        iter != list.end();
        ++iter) {
      if (iter != list.begin())
        std::cout << ", ";
      std::cout << *iter;
      int v = *iter;
      iter = list.insert(list.erase(iter), 2*v);
      map.insert(std::make_pair(list, 13));
    }
    std::cout << "}" << std::endl;
    std::cout << "list: {";
    for(FList::const_iterator iter = list.begin();
        iter != list.end();
        ++iter) {
      if (iter != list.begin())
        std::cout << ", ";
      std::cout << *iter;
    }
    std::cout << "}" << std::endl;
    list.erase(list.begin(), list.end());
    assert(list.empty());
    map.insert(std::make_pair(list, 33));
    std::cout << map << std::endl;
    {
      FList::iterator iter(list.begin());
      iter = list.begin();
      FList::const_iterator citer(iter);
      citer = iter;
    }
  }
  {
    // Check various copy operations of lists
    std::list<int>       sList;
    TList                tList;
    VList                vList;
    FList                fList;
    std::list<int> const csList;
    TList          const ctList;
    VList          const cvList;
    FList          const cfList;
    {
      std::list<int> dst1(sList);
      std::list<int> dst2(tList);
      std::list<int> dst3(vList);
      std::list<int> dst4(fList);
    }
    {
      std::list<int> dst1(csList);
      std::list<int> dst2(ctList);
      std::list<int> dst3(cvList);
      std::list<int> dst4(cfList);
    }
    {
      TList dst1(sList);
      TList dst2(tList);
      TList dst3(vList);
      TList dst4(fList);
    }
    {
      TList dst1(csList);
      TList dst2(ctList);
      TList dst3(cvList);
      TList dst4(cfList);
    }
    {
      VList dst1(sList);
      VList dst2(tList);
      VList dst3(vList);
      VList dst4(fList);
    }
    {
      VList dst1(csList);
      VList dst2(ctList);
      VList dst3(cvList);
      VList dst4(cfList);
    }
    {
      FList dst1(sList);
      FList dst2(tList);
      FList dst3(vList);
      FList dst4(fList);
    }
    {
      FList dst1(csList);
      FList dst2(ctList);
      FList dst3(cvList);
      FList dst4(cfList);
    }
  }
  return 0;
}
