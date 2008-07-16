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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_LISTFACADE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_LISTFACADE_HPP

#include <boost/iterator/iterator_facade.hpp>

#include "../Type/transform.hpp"

namespace CoSupport { namespace DataTypes {

  namespace Detail {

    struct IterTemplateAccess;

    template <template <class> class M, class LISTFACADE>
    class IterTemplate
    : public boost::iterator_facade<
        IterTemplate<M, LISTFACADE>,
        typename M<typename LISTFACADE::value_type>::type,
        boost::bidirectional_traversal_tag,
        typename M<typename LISTFACADE::reference_type>::type> {
      friend class IterTemplate<Type::Mutable, LISTFACADE>;
      friend class IterTemplate<Type::Const, LISTFACADE>;
      friend class boost::iterator_core_access;
      friend class IterTemplateAccess;
    private:
      typedef typename LISTFACADE::IterImpl IterImpl;
    private:
      IterImpl iter;

      IterTemplate(const IterImpl &iter)
        : iter(iter) {}
    public:
      IterTemplate() {}

      IterTemplate(const IterTemplate<Type::Mutable, LISTFACADE> &iter)
        : iter(iter.iter) {}

      // overwrite operator -> from boost which thinks it knows best
      typename M<typename LISTFACADE::pointer_type>::type operator->() const {
        typename M<typename LISTFACADE::reference_type>::type ref = dereference();
        return &ref;
      }
    protected:
      template <template <class> class MM>
      bool equal(const IterTemplate<MM, LISTFACADE> &n) const
        { return iter.equal(n.iter); }
      void increment()
        { iter.next(); }
      void decrement()
        { iter.prev(); }
      typename LISTFACADE::reference_type dereference() const
        { return iter.deref(); }
    };

    struct IterTemplateAccess {
      template <class ITERTEMPLATE>
      static
      ITERTEMPLATE construct(const typename ITERTEMPLATE::IterImpl &iter)
        { return ITERTEMPLATE(iter); }
      template <class ITERTEMPLATE>
      static
      typename ITERTEMPLATE::IterImpl &retrieve(ITERTEMPLATE &iter)
        { return iter.iter; }
      template <class ITERTEMPLATE>
      static
      const typename ITERTEMPLATE::IterImpl &retrieve(const ITERTEMPLATE &iter)
        { return iter.iter; }
    };

  } // namespace Detail

  //
  // ListFacade - use as a public base class for defining new
  // [standard-conforming: not just yet] list containers.
  //
  template <
    class DERIVED,            // The derived list container being constructed
    class ITER_,
    class VALUE,
    class REFERENCE = VALUE &,
    class PTR_      = VALUE *
  >
  class ListFacade: protected Detail::IterTemplateAccess {
    typedef ListFacade<DERIVED, ITER_, VALUE, REFERENCE, PTR_>  this_type;
    typedef Detail::IterTemplateAccess                          base_type;

    friend class Detail::IterTemplate<Type::Mutable, this_type>;
    friend class Detail::IterTemplate<Type::Const,   this_type>;
  private:
    // This is not a standard container type definition => hide it!
    typedef PTR_   pointer_type;  // for usage by Detail::IterTemplate
    typedef ITER_  IterImpl;      // for usage by Detail::IterTemplate
    //
    // Curiously Recurring Template interface.
    //
    DERIVED &derived()
      { return *static_cast<DERIVED *>(this); }

    DERIVED const &derived() const
      { return *static_cast<DERIVED const *>(this); }
  public:
    typedef VALUE     value_type;
    typedef REFERENCE reference_type;

    typedef Detail::IterTemplate<Type::Mutable,this_type> iterator;
    typedef Detail::IterTemplate<Type::Const,this_type>   const_iterator;

    iterator begin()
      { return base_type::construct<iterator>(derived().first()); }
    const_iterator begin() const
      { return base_type::construct<const_iterator>(derived().first()); }

    iterator end()
      { return base_type::construct<iterator>(derived().last()); }
    const_iterator end() const
      { return base_type::construct<const_iterator>(derived().last()); }

    bool empty() const
      { return begin() == end(); }

    reference_type front()
      { return *begin(); }
    typename Type::Const<reference_type>::type front() const
      { return *begin(); }

    reference_type back()
      { return *--end(); }
    typename Type::Const<reference_type>::type back() const
      { return *--end(); }

    iterator erase(const iterator &iter) {
      return base_type::construct<iterator>
        (derived().del(base_type::retrieve(iter)));
    }
    iterator insert(const iterator &iter, const value_type &value) {
      return base_type::construct<iterator>
        (derived().add(base_type::retrieve(iter), value));
    }

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
/*
    template<class Predicate>
    const_iterator find_if(Predicate pred) const
      { return std::find_if(begin(), end(), pred); }
    
    template<class Predicate>
    iterator find_if(Predicate pred)
      { return std::find_if(begin(), end(), pred); }
    
    template<class Predicate>
    bool erase_if(Predicate pred) {
      iterator i = find_if(pred);
      if(i == end())
        return false;
      erase(i);
      return true;
    }
    
    template<class Predicate>
    typename Type::Const<pointer_type>::type lookup(Predicate pred) const {
      const_iterator i = find_if(pred);
      if(i == end())
        return NULL;
      typename Type::Const<reference_type>::type ref = *i;
      return &ref;
    }
    
    template<class Predicate>
    pointer_type lookup(Predicate pred) {
      iterator i = find_if(pred);
      if(i == end())
        return NULL;
      reference_type ref = *i;
      return &ref;
    }
 */
  };

} } // namespace CoSupport::DataTypes

namespace CoSupport { namespace Type {

  template <template <class> class M, class ListFacade>
  struct Const<DataTypes::Detail::IterTemplate<M, ListFacade> > {
    typedef DataTypes::Detail::IterTemplate<Type::Const, ListFacade> type;
  };

  template <template <class> class M, class ListFacade>
  struct Mutable<DataTypes::Detail::IterTemplate<M, ListFacade> > {
    typedef DataTypes::Detail::IterTemplate<Type::Mutable, ListFacade> type;
  };

  template <class ListFacade>
  struct ToggleConst<DataTypes::Detail::IterTemplate<Mutable, ListFacade> > {
    typedef DataTypes::Detail::IterTemplate<Const, ListFacade> type;
  };
  template <class ListFacade>
  struct ToggleConst<DataTypes::Detail::IterTemplate<Const, ListFacade> > {
    typedef DataTypes::Detail::IterTemplate<Mutable, ListFacade> type;
  };

} } // namespace CoSupport::Type

#endif // _INCLUDED_COSUPPORT_DATATYPES_LISTFACADE_HPP
