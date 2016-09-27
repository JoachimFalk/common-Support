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

#ifndef _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_SETFACADE_HPP
#define _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_SETFACADE_HPP

#include "../../DataTypes/SetFacade.hpp"
#include "init_namespace.hpp"

#include <boost/python.hpp>

namespace CoSupport { namespace boost { namespace python {

template <typename T> class register_setfacade;

template <class T, class R, class CR, class P, class CP>
class register_setfacade<CoSupport::DataTypes::SetFacadeInterface<T,R,CR,P,CP> > {
  typedef register_setfacade<CoSupport::DataTypes::SetFacadeInterface<T,R,CR,P,CP> > this_type;
protected:
  typedef CoSupport::DataTypes::SetFacadeInterface<T,R,CR,P,CP> SetIf;
  typedef boost::intrusive_ptr<SetIf>                           PSetIf;
public:
  register_setfacade(const char *name) {
    class_<typename SetIf::VIter, boost::noncopyable>((name+std::string("Iter")).c_str(), no_init)
      .def("__copy__",         &SetIf::VIter::duplicate, return_value_policy<manage_new_object>())
      .def("_cxx_increment",   &SetIf::VIter::increment)
      .def("_cxx_decrement",   &SetIf::VIter::decrement)
      .def("_cxx_equal",       &SetIf::VIter::equal)
      .def("_cxx_dereference", &SetIf::VIter::dereference)
      ;
#if BOOST_VERSION >= 106000 && BOOST_VERSION < 106100
    // See https://github.com/boostorg/python/issues/29
    // and https://github.com/boostorg/python/issues/56
    boost::python::register_ptr_to_python<PSetIf>();
#endif //BOOST_VERSION >= 106000 && BOOST_VERSION < 106100
    class_<SetIf, PSetIf, boost::noncopyable>(name, no_init)
      .def("__init__",         make_constructor(&this_type::implSetConstruct, default_call_policies()), "create an empty list object")
      .def("__copy__",         &this_type::implSetDuplicate)
      .def("_cxx_begin",       &SetIf::implPBegin, return_value_policy<manage_new_object>())
      .def("_cxx_end",         &SetIf::implPEnd, return_value_policy<manage_new_object>())
      .def("_cxx_erase",       static_cast<void (SetIf::*)(typename SetIf::VIter const &)>(&SetIf::implErase))
      .def("_cxx_erase",       static_cast<void (SetIf::*)(typename SetIf::VIter const &, typename SetIf::VIter const &)>(&SetIf::implErase))
      .def("_cxx_insert",      &this_type::implSetPInsert)
      .def("_cxx_size",        &SetIf::implSize)
      .def("_cxx_lower_bound", &SetIf::implPLowerBound, return_value_policy<manage_new_object>())
      .def("_cxx_upper_bound", &SetIf::implPUpperBound, return_value_policy<manage_new_object>())
      .def("_cxx_find",        &SetIf::implPFind, return_value_policy<manage_new_object>())
      ;
  };
protected:
  static
  PSetIf implSetDuplicate(SetIf const *src) {
    PSetIf retval(new CoSupport::DataTypes::Detail::SetFacadeImpl<T,R,CR,P,CP>());
    std::set<T> &dst = static_cast<CoSupport::DataTypes::Detail::SetFacadeImpl<T,R,CR,P,CP> *>(retval.get())->set;
    boost::scoped_ptr<typename SetIf::VIter > iter(src->implPBegin());
    boost::scoped_ptr<typename SetIf::VIter > end(src->implPEnd());
    for (; !iter->equal(*end); iter->increment())
      dst.insert(iter->dereference());
    return retval;
  }

  static
  PSetIf implSetConstruct()
    { return new CoSupport::DataTypes::Detail::SetFacadeImpl<T,R,CR,P,CP>(); }

  static
  boost::python::tuple implSetPInsert(SetIf *set, T const &v) {
    std::pair<typename SetIf::VIter *, bool> retval = set->implPInsert(v);
    return boost::python::make_tuple(handle<>(typename manage_new_object::apply<typename SetIf::VIter *>::type()(retval.first)), retval.second);
  }

};

} } } // namespace CoSupport::boost::python

#endif // _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_SETFACADE_HPP
