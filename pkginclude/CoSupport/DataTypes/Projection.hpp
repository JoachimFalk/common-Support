/*
 * Copyright (c) 2004-2011 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_PROJECTION_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_PROJECTION_HPP

#include "VectorInterface.hpp"

#include <boost/type_traits/remove_reference.hpp>

namespace CoSupport { namespace DataTypes {

namespace Detail {

  template <typename ITERV, typename ITERI>
  class ProjectionRandomAccessTraversalIter {
    typedef ProjectionRandomAccessTraversalIter<ITERV, ITERI> this_type;
  public:
    ITERV iterv;
    ITERI iteri;

    ProjectionRandomAccessTraversalIter(const ITERV &iterv, const ITERI &iteri)
      : iterv(iterv), iteri(iteri) {}

    void advance(int n) { iteri += n; }
    int  distance_to(const this_type &rhs) const { return rhs.iteri - iteri; }
    bool equal(const this_type &rhs) const { return iteri == rhs.iteri && iterv == rhs.iterv; }

    typename std::iterator_traits<ITERV>::reference deref() const { return *(iterv+*iteri); }
  };

} // namespace Detail

template<class V, class I>
class Projection: public VectorInterface<
    Projection<V, I>,
    Detail::ProjectionRandomAccessTraversalIter<
      typename boost::remove_reference<V>::type::iterator,
      typename boost::remove_reference<I>::type::const_iterator>,
    typename boost::remove_reference<V>::type::value_type,
    typename boost::remove_reference<V>::type::reference,
    typename boost::remove_reference<V>::type::const_reference,
    typename boost::remove_reference<V>::type::pointer,
    typename boost::remove_reference<V>::type::const_pointer
  >
{
  typedef Projection<V,I> this_type;
  friend class VectorInterface<
    Projection<V, I>,
    Detail::ProjectionRandomAccessTraversalIter<
      typename boost::remove_reference<V>::type::iterator,
      typename boost::remove_reference<I>::type::const_iterator>,
    typename boost::remove_reference<V>::type::value_type,
    typename boost::remove_reference<V>::type::reference,
    typename boost::remove_reference<V>::type::const_reference,
    typename boost::remove_reference<V>::type::pointer,
    typename boost::remove_reference<V>::type::const_pointer
  >;
public:
  Projection(V v, I idx): v(v), idx(idx) {}
protected:
  typedef Detail::ProjectionRandomAccessTraversalIter<
    typename boost::remove_reference<V>::type::iterator,
    typename boost::remove_reference<I>::type::const_iterator> IterImpl;

  V v;
  I idx;

  IterImpl first() const
    { return IterImpl(const_cast<this_type *>(this)->v.begin(), idx.begin()); }
  IterImpl last() const
    { return IterImpl(const_cast<this_type *>(this)->v.begin(), idx.end()); }

/*Detail::ProjectionRandomAccessTraversalIter<typename std::vector<T>::iterator> del(const Detail::ProjectionRandomAccessTraversalIter<typename std::vector<T>::iterator> &iter)
    { return vector.erase(iter.iter); }
  Detail::ProjectionRandomAccessTraversalIter<typename std::vector<T>::iterator> add(const Detail::ProjectionRandomAccessTraversalIter<typename std::vector<T>::iterator> &iter, const typename this_type::value_type &value)
    { return vector.insert(iter.iter, value); }*/
};

} } // namespace CoSupport::DataTypes

template<class V, class I>
inline
CoSupport::DataTypes::Projection<const V &, const I &> proj(const V &v, const I &i)
  { return CoSupport::DataTypes::Projection<const V &, const I &>(v,i); }
template<class V, class I>
inline
CoSupport::DataTypes::Projection<V       &, const I &> proj(V       &v, const I &i)
  { return CoSupport::DataTypes::Projection<V       &, const I &>(v,i); }

#endif // _INCLUDED_COSUPPORT_DATATYPES_PROJECTION_HPP
