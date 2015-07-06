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

#ifndef _INCLUDED_COSUPPORT_ALLOCATORS_IDPOOL_HPP
#define _INCLUDED_COSUPPORT_ALLOCATORS_IDPOOL_HPP

#include <utility>
#include <stdlib.h>

#include <CoSupport/sassert.h>

#include "IdAllocRequest.hpp"

namespace CoSupport { namespace Allocators {

template <
  class DERIVED,
  class IDMAP,
  class IDRANGE = Detail::IdRange<typename IDMAP::key_type> >
class IdAllocator {
protected:
  typedef IDMAP                           IdMap;
  typedef IDRANGE                         IdRange;
private:
  typedef typename IdMap::key_type        key_type;
  typedef typename IdMap::mapped_type     mapped_type;
  typedef typename IdMap::value_type      value_type;
  typedef typename IdMap::iterator        iterator;
  typedef typename IdMap::const_iterator  const_iterator;
private:
  DERIVED       *getDerived()
    { return static_cast<DERIVED *>(this); }
  const DERIVED *getDerived() const
    { return static_cast<const DERIVED *>(this); }

  key_type nextId;
public:
  IdAllocator()
    : nextId(IdRange::min) {}

  iterator idAlloc(const mapped_type &obj) {
    return idAllocNext(nextId, obj);
  }

  iterator idAllocNext(key_type searchFrom, const mapped_type &obj) {
//  std::cerr << "IdAllocator<...>::idAllocNext" << std::endl;
//  std::cerr << "IdRange::min " << IdRange::min << std::endl;
//  std::cerr << "IdRange::max " << IdRange::min << std::endl;
//  std::cerr << "nextId:      " << nextId << std::endl;
    
    if (nextId > IdRange::max) {
      assert(!"UGH: No more IDS!!!");
      exit(-1);
    }
    assert(searchFrom >= IdRange::min && searchFrom <= IdRange::max);
    
    std::pair<iterator,bool>  allocated;
    bool                      setNextId;
    
    if (searchFrom <= nextId) {
      searchFrom = nextId;
      setNextId  = true;
    } else
      setNextId  = false;
    for (key_type id = searchFrom; true; ) {
      allocated = getDerived()->idMap.insert(std::make_pair(id, obj));
      if (allocated.second) {
        if (setNextId)
          nextId = id + 1;
        break;
      }
      if (++id > IdRange::max) {
        id        = nextId;
        setNextId = true;
      }
      if (id == searchFrom) {
        assert(!"UGH: No more IDS!!!");
        exit(-1);
      }
    }
    return allocated.first;
  }

  iterator idAllocThis(const key_type id, const mapped_type &obj) {
    std::pair<iterator,bool> allocated =
      getDerived()->idMap.insert(std::make_pair(id, obj));
    assert(allocated.second);
    return allocated.first;
  }

  void idFree(const key_type id) {
    sassert(getDerived()->idMap.erase(id) == 1);
    if (id < nextId && id >= IdRange::min)
      nextId = id;
  }
};

template <class IDMAP>
class IdPool: public IdAllocator<IdPool<IDMAP>, IDMAP> {
  typedef IdPool<IDMAP> this_type;

  friend class IdAllocator<this_type, IDMAP>;
protected:
  typename this_type::IdMap idMap;
};

} } // namespace CoSupport::Allocators

#endif // _INCLUDED_COSUPPORT_ALLOCATORS_IDPOOL_HPP
