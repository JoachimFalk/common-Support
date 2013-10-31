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
