// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 Shawniac <l1m3z0r@gmail.com>
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

#ifndef _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_LISTFACADE_HPP
#define _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_LISTFACADE_HPP

#include "../../DataTypes/ListFacade.hpp"
#include "init_namespace.hpp"

#include <boost/python.hpp>

namespace CoSupport { namespace boost { namespace python {

template <typename T> class register_listfacade;

template <class T, class R, class CR, class P, class CP>
class register_listfacade<CoSupport::DataTypes::ListFacadeInterface<T,R,CR,P,CP> > {
  typedef register_listfacade<CoSupport::DataTypes::ListFacadeInterface<T,R,CR,P,CP> > this_type;
protected:
  typedef CoSupport::DataTypes::ListFacadeInterface<T,R,CR,P,CP> ListIf;
  typedef boost::intrusive_ptr<ListIf>                           PListIf;
public:
  register_listfacade(const char *name) {
    class_<typename ListIf::VIter, boost::noncopyable>((name+std::string("Iter")).c_str(), no_init)
      .def("__copy__",         &ListIf::VIter::duplicate, return_value_policy<manage_new_object>())
      .def("_cxx_increment",   &ListIf::VIter::increment)
      .def("_cxx_decrement",   &ListIf::VIter::decrement)
      .def("_cxx_equal",       &ListIf::VIter::equal)
      .def("_cxx_dereference", &ListIf::VIter::dereference)
      ;
#if BOOST_VERSION >= 106000 && BOOST_VERSION < 106100
    // See https://github.com/boostorg/python/issues/29
    // and https://github.com/boostorg/python/issues/56
    boost::python::register_ptr_to_python<PListIf>();
#endif //BOOST_VERSION >= 106000 && BOOST_VERSION < 106100
    class_<ListIf, PListIf, boost::noncopyable>(name, no_init)
      .def("__init__",         make_constructor(&this_type::implListConstruct, default_call_policies()), "create an empty list object")
      .def("__copy__",         &this_type::implListDuplicate)
      .def("_cxx_begin",       &ListIf::implPBegin, return_value_policy<manage_new_object>())
      .def("_cxx_end",         &ListIf::implPEnd, return_value_policy<manage_new_object>())
      .def("_cxx_erase",       static_cast<typename ListIf::VIter *(ListIf::*)(typename ListIf::VIter const &)>(&ListIf::implPErase), return_value_policy<manage_new_object>())
      .def("_cxx_erase",       static_cast<typename ListIf::VIter *(ListIf::*)(typename ListIf::VIter const &, typename ListIf::VIter const &)>(&ListIf::implPErase), return_value_policy<manage_new_object>())
      .def("_cxx_insert",      &ListIf::implPInsert, return_value_policy<manage_new_object>())
      ;
  };
protected:
  static
  PListIf implListDuplicate(ListIf *src) {
    PListIf retval(new CoSupport::DataTypes::Detail::ListFacadeImpl<T,R,CR,P,CP>());
    std::list<T> &dst = static_cast<CoSupport::DataTypes::Detail::ListFacadeImpl<T,R,CR,P,CP> *>(retval.get())->list;
    boost::scoped_ptr<typename ListIf::VIter> iter(src->implPBegin());
    boost::scoped_ptr<typename ListIf::VIter> end(src->implPEnd());
    for (; !iter->equal(*end); iter->increment())
      dst.push_back(iter->dereference());
    return retval;
  }

  static
  PListIf implListConstruct()
    { return new CoSupport::DataTypes::Detail::ListFacadeImpl<T,R,CR,P,CP>(); }
};

} } } // namespace CoSupport::boost::python

#endif // _INCLUDED_COSUPPORT_BOOST_PYTHON_REGISTER_LISTFACADE_HPP
