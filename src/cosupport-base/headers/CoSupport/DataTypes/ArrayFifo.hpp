// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
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

/*
 * $Log: fifo.h,v $
 * Revision 1.2  2001/06/26 18:27:53  joachim
 * Convert files to unix file format !
 *
 * Revision 1.1  2001/06/26 18:25:35  joachim
 * Initial revision
 *
 */

#ifndef _INCLUDED_COSUPPORT_DATATYPES_ARRAYFIFO_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_ARRAYFIFO_HPP

#include "../compatibility-glue/nullptr.h"
#include "../sassert.h"
#include <CoSupport/Math/ModuloGroup.hpp>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/type_traits/remove_const.hpp>

namespace CoSupport { namespace DataTypes {

  /*
   * Fixed Array FIFO buffer
   */
  template <typename T, size_t N = 1>
  class ArrayFifo {
  public:
    typedef ArrayFifo<T,N>  this_type;
    typedef T               value_type;
  protected:
    typedef this_type       fifo_type;
    
    template <typename F, typename V>
    class IterTemplate
    : public boost::iterator_facade<
        IterTemplate<F,V>,
        V,
        boost::random_access_traversal_tag> {
    private:
      template <typename FF, typename VV>
      friend class IterTemplate;
      friend class boost::iterator_core_access;
      friend class ArrayFifo<T,N>;
      
      F                                                *f;
      Math::ModuloGroup<Math::CTModulus<size_t, N+1> >  i;
    public:
      IterTemplate()
        : f(nullptr) {}
      
      // Copy Constructor from const_iterator
      IterTemplate(
        IterTemplate<
          typename boost::remove_const<F>::type,
          typename boost::remove_const<V>::type> const &iter)
        : f(iter.f), i(iter.i) {}
    protected:
      IterTemplate(F *f, Math::ModuloGroup<Math::CTModulus<size_t, N+1> > i)
        : f(f), i(i) {}
      
      template <typename FF, typename VV>
      int distance_to(IterTemplate<FF,VV> const &iter) const {
        return (iter.i - f->rIndex).getValue() -
               (     i - f->rIndex).getValue();
      }
      template <typename FF, typename VV>
      bool equal(IterTemplate<FF,VV> const &iter) const
        { return i == iter.i; }
      void advance( int n )
        { i += n; }
      void increment()
        { i++; }
      void decrement()
        { i--; }
      V &dereference() const
        { return f->buffer[i.getValue()]; }
    };
    
    value_type                                       buffer[N+1];
    Math::ModuloGroup<Math::CTModulus<size_t, N+1> > rIndex;
    Math::ModuloGroup<Math::CTModulus<size_t, N+1> > wIndex;
  public:
    typedef IterTemplate<this_type      , value_type      > iterator;
    typedef IterTemplate<this_type const, value_type const> const_iterator;
    
    ArrayFifo() {}
    
    iterator begin()
      { return iterator(this,rIndex); }
    const_iterator begin() const
      { return const_iterator(this,rIndex); }
    iterator end()
      { return iterator(this,wIndex); }
    const_iterator end() const
      { return const_iterator(this,wIndex); }
   
    /// front element of fifo (at rptr)
    value_type &front()
      { return buffer[rIndex.getValue()]; }
    /// front element of fifo (at rptr)
    const value_type &front() const
      { return buffer[rIndex.getValue()]; }
    /// back element of fifo (element one previous of wptr)
    value_type &back()
      { return buffer[(wIndex-1).getValue()]; }
    /// back element of fifo (element one previous of wptr)
    const value_type &back() const
      { return buffer[(wIndex-1).getValue()]; }
    
    /// increase fifo read ptr
    void pop_front()
      { sassert( rIndex++ != wIndex ); }
    /// assign element and increase fifo write ptr
    void push_back( const value_type &v )
      { buffer[wIndex.getValue()] = v; sassert( ++wIndex != rIndex ); }
    /// same as: while ( !empty() ) pop_front();
    void clear_by_consume()
      { rIndex = wIndex; }
    /// same as: while ( !empty() ) pop_back();
    void clear_by_invalidate()
      { wIndex = rIndex; }
    /// clear fifo
    void clear()
      { rIndex = wIndex = 0; }
    
    /// maximal count of elements storable in the fifo
    size_t capacity() const
      { return N; }
    /// count of elements stored in the fifo
    size_t size() const
      { return (wIndex - rIndex).getValue(); }
    /// count of elements which could still be stored in the fifo
    size_t space() const
      { return (rIndex - wIndex - 1).getValue(); }
    /// is the fifo empty
    bool empty() const
      { return rIndex == wIndex; }
  };

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_ARRAYFIFO_HPP
