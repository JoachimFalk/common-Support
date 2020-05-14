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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_VECTOR_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_VECTOR_HPP

#include "VectorInterface.hpp"
#include "Detail/RandomAccessTraversalIterImpl.hpp"

#include <vector>

namespace CoSupport { namespace DataTypes {

template <typename T>
class Vector: public VectorInterface<Vector<T>, Detail::RandomAccessTraversalIterImpl<typename std::vector<T>::iterator>, T> {
  typedef Vector<T> this_type;
  friend class VectorInterface<Vector<T>, Detail::RandomAccessTraversalIterImpl<typename std::vector<T>::iterator>, T>;
protected:
  std::vector<T> vector;

  Detail::RandomAccessTraversalIterImpl<typename std::vector<T>::iterator> first() const
    { return const_cast<this_type *>(this)->vector.begin(); }
  Detail::RandomAccessTraversalIterImpl<typename std::vector<T>::iterator> last() const
    { return const_cast<this_type *>(this)->vector.end(); }

  Detail::RandomAccessTraversalIterImpl<typename std::vector<T>::iterator> del(const Detail::RandomAccessTraversalIterImpl<typename std::vector<T>::iterator> &iter)
    { return vector.erase(iter.iter); }
  Detail::RandomAccessTraversalIterImpl<typename std::vector<T>::iterator> add(const Detail::RandomAccessTraversalIterImpl<typename std::vector<T>::iterator> &iter, const typename this_type::value_type &value)
    { return vector.insert(iter.iter, value); }
};

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_VECTOR_HPP
