// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
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

#ifndef _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_TOFROM_PYTHON_CONVERTER_STDLIST_HPP
#define _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_TOFROM_PYTHON_CONVERTER_STDLIST_HPP

#include "../../compatibility-glue/nullptr.h"

#include <list>
#include "init_namespace.hpp"

#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/stl_iterator.hpp>

namespace CoSupport { namespace boost { namespace python {

/// This class implements a rvalue from python converter for sts::list<T,A>
template<typename T, typename A>
class register_from_python_converter<std::list<T,A> > {
  typedef std::list<T,A>                              value_type;
  typedef register_from_python_converter<value_type>  this_type;
public:
  register_from_python_converter() {
    // Insert an rvalue from_python converter at the tail of the chain.
    // This is used to register implicit conversion converters.
    boost::python::converter::registry::push_back(
      &convertible,
      &construct,
      boost::python::type_id<value_type>()
#ifdef BOOST_PYTHON_SUPPORTS_PY_SIGNATURES
    , &boost::python::converter::expected_pytype_for_arg<T>::get_pytype
#endif
      );
  }

  static void *convertible(PyObject *obj_ptr) {
    /// Check if PyObject is iterable.
    return PyObject_GetIter(obj_ptr) ? obj_ptr : nullptr;
  }

  static void construct(
      PyObject *obj_ptr,
      boost::python::converter::rvalue_from_python_stage1_data *data)
  {
    // Obtain a handle to the memory block that the converter has allocated
    // for the C++ type.
    void *storage =
      reinterpret_cast<boost::python::converter::rvalue_from_python_storage<value_type> *>(data)
      ->storage.bytes;
    // Object is a borrowed reference, so create a handle indicting it is
    // borrowed for proper reference counting.
    boost::python::handle<> handle(boost::python::borrowed(obj_ptr));
    // Allocate the C++ type into the converter's memory block, and assign
    // its handle to the converter's convertible variable.  The C++
    // container is populated by passing the begin and end iterators of
    // the python object to the container's constructor.
    data->convertible = new (storage) value_type(
      boost::python::stl_input_iterator<T>(boost::python::object(handle)), // begin
      boost::python::stl_input_iterator<T>());                             // end
  }
};

/// This class implements a to python converter for std::list<T,A>
template<typename T, typename A>
class register_to_python_converter<std::list<T,A> > {
  typedef std::list<T,A>                            value_type;
  typedef register_to_python_converter<value_type>  this_type;
public:
  register_to_python_converter() {
    boost::python::to_python_converter<
      value_type,
      this_type
#ifdef BOOST_PYTHON_SUPPORTS_PY_SIGNATURES
      , true
#endif
      >();
  }

  static PyObject *convert(value_type const &v) {
    boost::python::list pyList;
    for (typename value_type::const_iterator iter = v.begin();
         iter != v.end();
         ++iter)
      pyList.append(boost::python::object(*iter));
    return boost::python::incref(pyList.ptr());
  }

#ifdef BOOST_PYTHON_SUPPORTS_PY_SIGNATURES
  static PyTypeObject const *get_pytype() {
    return &PyList_Type;
  }
#endif
};

} } } // namespace CoSupport::boost::python

#endif // _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_TOFROM_PYTHON_CONVERTER_STDLIST_HPP
