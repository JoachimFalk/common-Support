// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2014 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_TOFROM_PYTHON_CONVERTER_FACADE_HPP
#define _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_TOFROM_PYTHON_CONVERTER_FACADE_HPP

#include "../../compatibility-glue/nullptr.h"
#include "../../DataTypes/Facade.hpp"

#include "init_namespace.hpp"

#include <boost/python.hpp>

namespace CoSupport { namespace boost { namespace python {

/// This class implements a rvalue from python converter for FacadePtr<T,C>
template <class T, template <class> class C>
class register_from_python_converter<CoSupport::DataTypes::FacadePtr<T,C> > {
  typedef CoSupport::DataTypes::FacadePtr<T,C>        value_type;
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
    if (obj_ptr != Py_None) {
      boost::python::extract<typename T::ImplType *> proxy(obj_ptr);
      if (!proxy.check())
        return nullptr;
    }
    return obj_ptr;
  }

  static void construct(
      PyObject *obj_ptr,
      boost::python::converter::rvalue_from_python_stage1_data *data)
  {
    void *storage =
      reinterpret_cast<boost::python::converter::rvalue_from_python_storage<value_type> *>(data)
      ->storage.bytes;
    if (obj_ptr != Py_None) {
      boost::python::extract<typename T::ImplType *> proxy(obj_ptr);
      new (storage) value_type(proxy());
    } else
      new (storage) value_type();
    data->convertible = storage;
  }
};

/// This class implements a to python converter for FacadePtr<T,C>
template <class T, template <class> class C>
class register_to_python_converter<CoSupport::DataTypes::FacadePtr<T,C> > {
  typedef CoSupport::DataTypes::FacadePtr<T,C>      value_type;
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
    return boost::python::incref((v != nullptr
      ? boost::python::object(typename T::SmartPtr(CoSupport::DataTypes::FacadeCoreAccess::getImpl(v)))
      : boost::python::object()).ptr());
  }

#ifdef BOOST_PYTHON_SUPPORTS_PY_SIGNATURES
  static PyTypeObject const *get_pytype() {
    return boost::python::converter::registered<typename T::ImplType *>::converters
      .to_python_target_type();
  }
#endif
};

/// This class implements a rvalue from python converter for FacadeRef<T,C>
template <class T, template <class> class C>
class register_from_python_converter<CoSupport::DataTypes::FacadeRef<T,C> > {
  typedef CoSupport::DataTypes::FacadeRef<T,C>  value_type;
  typedef register_from_python_converter<value_type>     this_type;
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
    if (obj_ptr != Py_None) {
      boost::python::extract<typename T::ImplType *> proxy(obj_ptr);
      if (proxy.check())
        return obj_ptr;
    }
    return nullptr;
  }

  static void construct(
      PyObject *obj_ptr,
      boost::python::converter::rvalue_from_python_stage1_data *data)
  {
    void *storage =
      reinterpret_cast<boost::python::converter::rvalue_from_python_storage<value_type> *>(data)
      ->storage.bytes;
    assert(obj_ptr != Py_None);
    boost::python::extract<typename T::ImplType *> proxy(obj_ptr);
    new (storage) value_type(proxy());
    data->convertible = storage;
  }
};

/// This class implements a to python converter for FacadeRef<T,C>
template <class T, template <class> class C>
class register_to_python_converter<CoSupport::DataTypes::FacadeRef<T,C> > {
  typedef CoSupport::DataTypes::FacadeRef<T,C>      value_type;
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
    return boost::python::incref(
      boost::python::object(typename T::SmartPtr(CoSupport::DataTypes::FacadeCoreAccess::getImpl(v))).ptr());
  }

#ifdef BOOST_PYTHON_SUPPORTS_PY_SIGNATURES
  static PyTypeObject const *get_pytype() {
    return boost::python::converter::registered<typename T::ImplType *>::converters
      .to_python_target_type();
  }
#endif
};

} } } // namespace CoSupport::boost::python

#endif // _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_TOFROM_PYTHON_CONVERTER_FACADE_HPP
