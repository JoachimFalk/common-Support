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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_VECTORPROJECTION_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_VECTORPROJECTION_HPP

#include "Projection.hpp"
#include "VectorInterface.hpp"
#include "../Type/STLIteratorSelector.hpp"
#include "../Type/STLReferenceSelector.hpp"
#include "../Type/STLPointerSelector.hpp"

#include <boost/type_traits/remove_reference.hpp>

namespace std {
  template <class T, class A> class vector;
} // namespace std

namespace CoSupport { namespace DataTypes {

  namespace Detail {

    template <typename ITERV, typename ITERI>
    class VectorProjectionRandomAccessTraversalIter {
      typedef VectorProjectionRandomAccessTraversalIter<ITERV, ITERI> this_type;
    public:
      ITERV iterv;
      ITERI iteri;

      VectorProjectionRandomAccessTraversalIter(const ITERV &iterv, const ITERI &iteri)
        : iterv(iterv), iteri(iteri) {}

      void advance(int n) { iteri += n; }
      int  distance_to(const this_type &rhs) const { return rhs.iteri - iteri; }
      bool equal(const this_type &rhs) const { return iteri == rhs.iteri && iterv == rhs.iterv; }

      typename std::iterator_traits<ITERV>::reference deref() const { return *(iterv+*iteri); }
    };

  } // namespace Detail

  template <class V, class I>
  class VectorProjection: public VectorInterface<
      VectorProjection<V, I>,
      Detail::VectorProjectionRandomAccessTraversalIter<
        typename Type::STLIteratorSelector<typename boost::remove_reference<V>::type>::type,
        typename boost::remove_reference<I>::type::const_iterator>,
      typename boost::remove_reference<V>::type::value_type,
      typename Type::STLReferenceSelector<typename boost::remove_reference<V>::type>::type,
      typename boost::remove_reference<V>::type::const_reference,
      typename Type::STLPointerSelector<typename boost::remove_reference<V>::type>::type,
      typename boost::remove_reference<V>::type::const_pointer
    >
  {
    typedef VectorProjection<V, I> this_type;

    friend class VectorInterface<
      this_type,
      typename this_type::IterImpl,
      typename this_type::value_type,
      typename this_type::reference,
      typename this_type::const_reference,
      typename this_type::pointer,
      typename this_type::const_pointer
    >;
  public:
    VectorProjection(V v, I idx): v(v), idx(idx) {}

    template <class VV, class AA>
    this_type &operator = (std::vector<VV, AA> const &val) {
      assert(val.size() == this->size());
      for (size_t i = 0; i < val.size(); ++i)
        v[idx[i]] = val[i];
      return *this;
    }

    template <
      class DD,
      class II,
      class VV,
      class RR,
      class CRCR,
      class PP,
      class CPCP
    >
    this_type &operator = (VectorInterface<DD,II,VV,RR,CRCR,PP,CPCP> const &val) {
      assert(val.size() == this->size());
      for (size_t i = 0; i < val.size(); ++i)
        v[idx[i]] = val[i];
      return *this;
    }

    this_type &operator = (const this_type &val) {
      assert(val.size() == this->size());
      for (size_t i = 0; i < val.size(); ++i)
        v[idx[i]] = val[i];
      return *this;
    }

    this_type &operator = (typename this_type::value_type const &val) {
      for (size_t i = 0; i < this->size(); ++i)
        v[idx[i]] = val;
      return *this;
    }
  protected:
    V v;
    I idx;

    typename this_type::IterImpl first() const {
      return typename this_type::IterImpl(
        const_cast<this_type *>(this)->v.begin(),
        this->idx.begin());
    }
    typename this_type::IterImpl last() const {
      return typename this_type::IterImpl(
        const_cast<this_type *>(this)->v.begin(),
        this->idx.end());
    }

  /*Detail::VectorProjectionRandomAccessTraversalIter<typename std::vector<T>::iterator> del(const Detail::ProjectionRandomAccessTraversalIter<typename std::vector<T>::iterator> &iter)
      { return vector.erase(iter.iter); }
    Detail::VectorProjectionRandomAccessTraversalIter<typename std::vector<T>::iterator> add(const Detail::ProjectionRandomAccessTraversalIter<typename std::vector<T>::iterator> &iter, const typename this_type::value_type &value)
      { return vector.insert(iter.iter, value); }*/
  };

/*template <typename T1, class A1, typename T2, class A2>
  struct Projection<std::vector<T1, A1> &, std::vector<T2, A2> const &>
  : public VectorProjection<std::vector<T1, A1> &, std::vector<T2, A2> const &> {
    Projection(std::vector<T1, A1>       &v,
               std::vector<T2, A2> const &idx)
      : VectorProjection<std::vector<T1, A1> &, std::vector<T2, A2> const &>(v, idx) {}
  };
  template <typename T1, class A1, typename T2, class A2>
  struct Projection<std::vector<T1, A1> const &, std::vector<T2, A2> const &>
  : public VectorProjection<std::vector<T1, A1> const &, std::vector<T2, A2> const &> {
    Projection(std::vector<T1, A1> const &v,
               std::vector<T2, A2> const &idx)
      : VectorProjection<std::vector<T1, A1> const &, std::vector<T2, A2> const &>(v, idx) {}
  };*/

  template <class V, class I>
  struct Projection
  : public VectorProjection<V, I> {
    Projection(V v, I idx)
      : VectorProjection<V, I>(v, idx) {}
  public:
    using VectorProjection<V, I>::operator =;
  };

  template<
    class DERIVED,
    class ITER_,
    class VALUE,
    class REFERENCE,
    class CONSTREFERENCE,
    class PTR_,
    class CONSTPTR_
  >
  class VectorInterface;

  template <class D1, class I1, class V1, class R1, class CR1, class P1, class CP1,
            typename T2, class A2>
  Projection<VectorInterface<D1,I1,V1,R1,CR1,P1,CP1> &, std::vector<T2, A2> const &>
  proj(VectorInterface<D1,I1,V1,R1,CR1,P1,CP1> &v, std::vector<T2, A2> const &idx)
    { return Projection<VectorInterface<D1,I1,V1,R1,CR1,P1,CP1> &, std::vector<T2, A2> const &>(v, idx); }
  template <class D1, class I1, class V1, class R1, class CR1, class P1, class CP1,
            typename T2, class A2>
  Projection<VectorInterface<D1,I1,V1,R1,CR1,P1,CP1> const &, std::vector<T2, A2> const &>
  proj(VectorInterface<D1,I1,V1,R1,CR1,P1,CP1> const &v, std::vector<T2, A2> const &idx)
    { return Projection<VectorInterface<D1,I1,V1,R1,CR1,P1,CP1> &, std::vector<T2, A2> const &>(v, idx); }

} } // namespace CoSupport::DataTypes

namespace std {

  template <typename T1, class A1, typename T2, class A2>
  CoSupport::DataTypes::Projection<std::vector<T1, A1> &, std::vector<T2, A2> const &>
  proj(std::vector<T1, A1> &v, std::vector<T2, A2> const &idx)
    { return CoSupport::DataTypes::Projection<std::vector<T1, A1> &, std::vector<T2, A2> const &>(v, idx); }

  template <typename T1, class A1, typename T2, class A2>
  CoSupport::DataTypes::Projection<std::vector<T1, A1> const &, std::vector<T2, A2> const &>
  proj(std::vector<T1, A1> const &v, std::vector<T2, A2> const &idx)
    { return CoSupport::DataTypes::Projection<std::vector<T1, A1> &, std::vector<T2, A2> const &>(v, idx); }

};

#endif // _INCLUDED_COSUPPORT_DATATYPES_VECTORPROJECTION_HPP
