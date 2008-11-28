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
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/add_pointer.hpp>

#include "../Type/STLReferenceSelector.hpp"
#include "../Type/STLPointerSelector.hpp"

namespace CoSupport { namespace DataTypes {

  namespace Detail {

    struct IterTemplateAccess;

    template <typename LISTFACADE>
    class IterTemplate
    : public boost::iterator_facade<
        IterTemplate<LISTFACADE>,
        typename LISTFACADE::value_type,
        boost::bidirectional_traversal_tag,
        typename Type::STLReferenceSelector<LISTFACADE>::type,
        typename LISTFACADE::difference_type> {
      typedef IterTemplate this_type;

      friend class IterTemplate<typename boost::remove_const<LISTFACADE>::type>;
      friend class IterTemplate<typename boost::add_const<LISTFACADE>::type>;
      friend class boost::iterator_core_access;
      friend struct IterTemplateAccess;
    private:
      typedef typename LISTFACADE::IterImpl IterImpl;
    public:
      // overwrite pointer type from boost which thinks it knows best
      typedef typename Type::STLPointerSelector<LISTFACADE>::type pointer;
    private:
      IterImpl iter;

      IterTemplate(const IterImpl &iter)
        : iter(iter) {}
    public:
      IterTemplate() {}

      IterTemplate(const IterTemplate<typename boost::remove_const<LISTFACADE>::type> &iter)
        : iter(iter.iter) {}

      // overwrite operator -> from boost which thinks it knows best
      pointer operator->() const {
        typename this_type::reference ref = dereference();
        return &ref;
      }
    protected:
      bool equal(const IterTemplate<typename boost::remove_const<LISTFACADE>::type> &n) const
        { return iter.equal(n.iter); }
      bool equal(const IterTemplate<typename boost::add_const<LISTFACADE>::type> &n) const
        { return iter.equal(n.iter); }
      void increment()
        { iter.next(); }
      void decrement()
        { iter.prev(); }
      typename this_type::reference dereference() const
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
    class DERIVED, // The derived list container being constructed
    class ITER_,
    class VALUE,
    class REFERENCE = typename boost::add_reference<VALUE>::type,
    class CONSTREFERENCE = typename boost::add_reference<typename boost::add_const<VALUE>::type>::type,
    class PTR_ = typename boost::add_pointer<VALUE>::type,
    class CONSTPTR_ = typename boost::add_pointer<typename boost::add_const<VALUE>::type>::type
  >
  class ListFacade: protected Detail::IterTemplateAccess {
    typedef ListFacade                  this_type;
    typedef Detail::IterTemplateAccess  base_type;

    friend class Detail::IterTemplate<this_type>;
    friend class Detail::IterTemplate<const this_type>;
  private:
    // This is not a standard container type definition => hide it!
    typedef ITER_  IterImpl;      // for usage by Detail::IterTemplate
    //
    // Curiously Recurring Template interface.
    //
    DERIVED &derived()
      { return *static_cast<DERIVED *>(this); }

    DERIVED const &derived() const
      { return *static_cast<DERIVED const *>(this); }
  public:

    typedef VALUE           value_type;
    typedef REFERENCE       reference;
    typedef CONSTREFERENCE  const_reference;
    typedef PTR_            pointer;
    typedef CONSTPTR_       const_pointer;

    typedef Detail::IterTemplate<this_type>       iterator;
    typedef Detail::IterTemplate<const this_type> const_iterator;

    typedef std::ptrdiff_t                                        difference_type;
    typedef typename boost::make_unsigned<difference_type>::type  size_type;

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

    reference front()
      { return *begin(); }
    const_reference front() const
      { return *begin(); }

    reference back()
      { return *--end(); }
    const_reference back() const
      { return *--end(); }

    iterator erase(const iterator &iter) {
      return base_type::construct<iterator>
        (derived().del(base_type::retrieve(iter)));
    }
    // const value_type &v is correct here this is also used by std::list
    iterator insert(const iterator &iter, const value_type &v) {
      return base_type::construct<iterator>
        (derived().add(base_type::retrieve(iter), v));
    }

    // const value_type &v is correct here this is also used by std::list
    void push_back(const value_type &v)
      { insert(end(), v); }
    // const value_type &v is correct here this is also used by std::list
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

#endif // _INCLUDED_COSUPPORT_DATATYPES_LISTFACADE_HPP
