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

#include <cstring>

#include <list>

/// This class implements the interface for a std::list containing values of type T.
template <
  typename T,
  typename R,
  typename CR,
  typename P,
  typename CP
>
class List;

namespace Detail {

  template <class CONTAINER>
  struct ListIterBaseAccessor {
    typedef typename CONTAINER::template IterBase<CONTAINER>::type type;
  };

  template <class CONTAINER>
  class ListIter: public ListIterBaseAccessor<CONTAINER>::type {
    typedef ListIter<CONTAINER> this_type;

    template <
      typename T,
      typename R,
      typename CR,
      typename P,
      typename CP
    >
    friend class List;
    friend class ListIter<typename boost::add_const<CONTAINER>::type>;
    friend class boost::iterator_core_access;
  public:
    ListIter() {}
    ListIter(ListIter<typename boost::remove_const<CONTAINER>::type> const &rhs)
      : iter(rhs.iter) {}
  private:
    typename std::list<typename CONTAINER::value_type>::iterator iter;

    ListIter(typename std::list<typename CONTAINER::value_type>::iterator const &iter): iter(iter) {}

    void increment() { ++iter; }
    void decrement() { --iter; }
    bool equal(const this_type &rhs) const { return iter == rhs.iter; }

    typename this_type::reference dereference() const { return *iter; }
  };

} // namespace Detail

/// This class implements the interface for a std::list containing values of type T.
template <
  typename T,
  typename R  = typename boost::add_reference<T>::type,
  typename CR = typename boost::add_reference<typename boost::add_const<T>::type>::type,
  typename P  = typename boost::add_pointer<T>::type,
  typename CP = typename boost::add_pointer<typename boost::add_const<T>::type>::type
>
class List
: public CoSupport::DataTypes::ListInterface<List<T,R,CR,P,CP>,Detail::ListIter,T,R,CR,P,CP>
{
  typedef List<T,R,CR,P,CP>                                                          this_type;
  typedef CoSupport::DataTypes::ListInterface<this_type,Detail::ListIter,T,R,CR,P,CP> base_type;

  friend class CoSupport::DataTypes::ListInterface<this_type,Detail::ListIter,T,R,CR,P,CP>;
  template <class CONTAINER, bool REVERSE> friend class Detail::ListIterBaseAccessor;
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
  //assert(list.size() == 2 && list[0] == 1 && list[1] == 2);
    list.insert(list.begin(), 0);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 3 && list[0] == 0 && list[1] == 1 && list[2] == 2);
    list.push_front(-1);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 4 && list[0] ==-1 && list[1] == 0 && list[2] == 1 && list[3] == 2);
    list.erase(--list.end());
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 3 && list[0] ==-1 && list[1] == 0 && list[2] == 1);
    list.erase(--(--list.end()));
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 2 && list[0] ==-1 && list[1] == 1);
    assert(list.front() == -1);
    assert(list.back() == 1);
    list.insert(++list.begin(), 2, 55);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 4 && list[0] ==-1 && list[1] == 55 && list[2] == 55 && list[3] == 1);

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
  //assert(list.size() == 2 && list[0] == 1 && list[1] == 2);
    list.insert(list.begin(), 0);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 3 && list[0] == 0 && list[1] == 1 && list[2] == 2);
    list.push_front(-1);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 4 && list[0] ==-1 && list[1] == 0 && list[2] == 1 && list[3] == 2);
    list.erase(--list.end());
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 3 && list[0] ==-1 && list[1] == 0 && list[2] == 1);
    list.erase(--(--list.end()));
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 2 && list[0] ==-1 && list[1] == 1);
    assert(list.front() == -1);
    assert(list.back() == 1);
    list.insert(++list.begin(), 2, 55);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 4 && list[0] ==-1 && list[1] == 55 && list[2] == 55 && list[3] == 1);

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
  //assert(list.size() == 2 && list[0] == 1 && list[1] == 2);
    list.insert(list.begin(), 0);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 3 && list[0] == 0 && list[1] == 1 && list[2] == 2);
    list.push_front(-1);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 4 && list[0] ==-1 && list[1] == 0 && list[2] == 1 && list[3] == 2);
    list.erase(--list.end());
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 3 && list[0] ==-1 && list[1] == 0 && list[2] == 1);
    list.erase(--(--list.end()));
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 2 && list[0] ==-1 && list[1] == 1);
    assert(list.front() == -1);
    assert(list.back() == 1);
    list.insert(++list.begin(), 2, 55);
    std::cout << "list: " << list << ", list.size(): " << list.size() << std::endl;
  //assert(list.size() == 4 && list[0] ==-1 && list[1] == 55 && list[2] == 55 && list[3] == 1);

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
    std::list<int> sList;
    TList          tList;
    VList          vList;
    FList          fList;
    {
      std::list<int> dst1(sList);
      std::list<int> dst2(tList);
      std::list<int> dst3(vList);
      std::list<int> dst4(fList);
    }
    {
      TList dst1(sList);
      TList dst2(tList);
      TList dst3(vList);
      TList dst4(fList);
    }
    {
      VList dst1(sList);
      VList dst2(tList);
      VList dst3(vList);
      VList dst4(fList);
    }
    {
      FList dst1(sList);
      FList dst2(tList);
      FList dst3(vList);
      FList dst4(fList);
    }
  }
  return 0;
}
