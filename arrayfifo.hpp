/* vim: set sw=2 ts=8: */
/*
 * Copyright (C) Joachim Falk <joachim.falk@gmx.de> $Date: 2001/06/26 18:27:53 $
 *
 * fifo.h is part of the jf_libs distribution of Joachim Falk;
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * The jf_libs distributio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
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

#ifndef _INCLUDED_ARRAYFIFO_HPP
#define _INCLUDED_ARRAYFIFO_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

#include "sassert.h"
#include "modulo_arith.hpp"

namespace jflibs {

  /*
   * Fixed Array FIFO buffer
   */
  template <typename T, size_t N = 1>
  class ArrayFifo {
  public:
    typedef ArrayFifo<T,N>  this_type;
    typedef T               value_type;
  private:
    struct enabler {};
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
      friend class fifo_type;
      
      F               *f;
      ModuloInt<N+1>   i;
    public:
      IterTemplate()
        : f(NULL) {}
      
      template <typename FF, typename VV>
      IterTemplate(
          IterTemplate<FF,VV> const &iter,
          typename boost::enable_if<
             boost::is_convertible<FF *, F *>,
             enabler>::type = enabler() )
        : f(iter.f), i(iter.i) {}
    protected:
      IterTemplate( F *f, ModuloInt<N+1> i )
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
    
    value_type      buffer[N+1];
    ModuloInt<N+1>  rIndex;
    ModuloInt<N+1>  wIndex;
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
    
    value_type &front()
      { return buffer[rIndex.getValue()]; }
    const value_type &front() const
      { return buffer[rIndex.getValue()]; }
    value_type &back()
      { return buffer[(wIndex-1).getValue()]; }
    const value_type &back() const
      { return buffer[(wIndex-1).getValue()]; }
    
    void pop_front()
      { sassert( rIndex++ != wIndex ); }
    void push_back( const value_type &v )
      { buffer[wIndex.getValue()] = v; sassert( ++wIndex != rIndex ); }
    
    size_t capacity() const
      { return N; }
    size_t size() const
      { return (wIndex - rIndex).getValue(); }
    size_t space() const
      { return (rIndex - wIndex - 1).getValue(); }
  };

}

#endif /* _INCLUDED_ARRAYFIFO_HPP */
