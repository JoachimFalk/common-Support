// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c) 2013 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_TOFROM_PYTHON_CONVERTER_MAYBEVALUE_HPP
#define _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_TOFROM_PYTHON_CONVERTER_MAYBEVALUE_HPP

#include "../../compatibility-glue/nullptr.h"
#include "../../DataTypes/MaybeValue.hpp"

#include "init_namespace.hpp"

#include <boost/python.hpp>

namespace CoSupport { namespace boost { namespace python {

/// This class implements a rvalue from python converter for MaybeValue<T>
template <typename T>
class register_from_python_converter<CoSupport::DataTypes::MaybeValue<T> > {
  typedef CoSupport::DataTypes::MaybeValue<T>         value_type;
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
      boost::python::extract<T> proxy(obj_ptr);
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
      boost::python::extract<T> proxy(obj_ptr);
      new (storage) value_type(proxy());
    } else
      new (storage) value_type();
    data->convertible = storage;
  }
};

/// This class implements a to python converter for MaybeValue<T>
template <typename T>
class register_to_python_converter<CoSupport::DataTypes::MaybeValue<T> > {
  typedef CoSupport::DataTypes::MaybeValue<T>       value_type;
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
    return boost::python::incref((v.isDefined()
      ? boost::python::object(v.get())
      : boost::python::object()).ptr());
  }

#ifdef BOOST_PYTHON_SUPPORTS_PY_SIGNATURES
  static PyTypeObject const *get_pytype() {
    return boost::python::converter::registered<T>::converters
      .to_python_target_type();
  }
#endif
};

} } } // namespace CoSupport::boost::python

#endif // _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_TOFROM_PYTHON_CONVERTER_MAYBEVALUE_HPP
