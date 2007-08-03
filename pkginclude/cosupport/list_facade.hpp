/*
 * Copyright (c) 2004-2007 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_LIST_FACADE_HPP
#define _INCLUDED_COSUPPORT_LIST_FACADE_HPP

#include <boost/iterator/iterator_facade.hpp>

namespace CoSupport {

  //
  // ListFacade - use as a public base class for defining new
  // [standard-conforming: not just yet] list containers.
  //
  template <
    class Derived,            // The derived list container being constructed
    class Iter,
    class Value,
    class Reference = Value &,
    class Ptr       = Value *
  >
  class ListFacade {
  private:
    typedef ListFacade<Derived, Iter, Value, Reference, Ptr> this_type;

    template <template <class> class M>
    class IterTemplate
    : public boost::iterator_facade<
        IterTemplate<M>,
        typename M<Value>::type,
        boost::bidirectional_traversal_tag,
        typename M<Reference>::type> {
      friend class IterTemplate<CoSupport::Type::Mutable>;
      friend class IterTemplate<CoSupport::Type::Const>;
      friend class boost::iterator_core_access;
      friend class ListFacade<Derived, Iter, Value, Reference, Ptr>;
    private:
      Iter iter;
    public:
      IterTemplate() {}

      IterTemplate(const IterTemplate<CoSupport::Type::Mutable> &iter)
        : iter(iter.iter) {}

      // overwrite operator -> from boost which thinks he knows best
      typename M<Ptr>::type operator->() const {
        typename M<Reference>::type ref = dereference();
        return &ref;
      }
    protected:
      IterTemplate(const Iter &iter)
        : iter(iter) {}

      template <template <class> class MM>
      bool equal(const IterTemplate<MM> &n) const
        { return iter.equal(n.iter); }
      void increment()
        { iter.next(); }
      void decrement()
        { iter.prev(); }
      Reference dereference() const
        { return iter.deref(); }
    };

    //
    // Curiously Recurring Template interface.
    //
    Derived& derived()
      { return *static_cast<Derived*>(this); }

    Derived const& derived() const
      { return *static_cast<Derived const*>(this); }
  public:
    typedef Value                                  value_type;

    typedef IterTemplate<CoSupport::Type::Mutable> iterator;
    typedef IterTemplate<CoSupport::Type::Const>   const_iterator;

    iterator begin()
      { return iterator(derived().first()); }
    const_iterator begin() const
      { return const_iterator(derived().first()); }

    iterator end()
      { return iterator(derived().last()); }
    const_iterator end() const
      { return const_iterator(derived().last()); }

     bool empty() const
      { return begin() == end(); }

    Reference front()
      { return *begin(); }
    typename CoSupport::Type::Const<Reference>::type front() const
      { return *begin(); }

    Reference back()
      { return *--end(); }
    typename CoSupport::Type::Const<Reference>::type back() const
      { return *--end(); }

    iterator erase(const iterator &iter)
      { return iterator(derived().del(iter.iter)); }
    iterator insert(const iterator &iter, const value_type &value)
      { return iterator(derived().add(iter.iter, value)); }

    void push_back(const value_type &v)
      { insert(end(), v); }
    void push_front(const value_type &v) 
      { insert(begin(), v); }

    void pop_back()
      { erase(--end()); }
    void pop_front()
      { erase(begin()); }

    size_t size() const {
      size_t retval = 0;
      
      for (const_iterator iter = begin(); iter != end(); ++iter)
        ++retval;
      return retval;
    }
    
    template<class Predicate>
    const_iterator find_if(Predicate pred) const
      { return std::find_if(begin(), end(), pred); }
    
    template<class Predicate>
    iterator find_if(Predicate pred)
      { return std::find_if(begin(), end(), pred); }
    
    template<class Predicate>
    typename CoSupport::Type::Const<Ptr>::type lookup(Predicate pred) const {
      const_iterator i = find_if(pred);
      if(i == end())
        return NULL;
      typename CoSupport::Type::Const<Reference>::type ref = *i;
      return &ref;
    }
    
    template<class Predicate>
    Ptr lookup(Predicate pred) {
      iterator i = find_if(pred);
      if(i == end())
        return NULL;
      Reference ref = *i;
      return &ref;
    }
  };

} // namespace CoSupport

#endif // _INCLUDED_COSUPPORT_LIST_FACADE_HPP
