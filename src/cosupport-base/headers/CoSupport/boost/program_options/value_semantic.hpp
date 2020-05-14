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

#ifndef _INCLUDED_COSUPPORT_BOOST_PROGRAM_OPTIONS_VALUE_SEMANTIC_HPP
#define _INCLUDED_COSUPPORT_BOOST_PROGRAM_OPTIONS_VALUE_SEMANTIC_HPP

#include "../../compatibility-glue/nullptr.h"
#include "init_namespace.hpp"

#include <boost/program_options/value_semantic.hpp>

namespace CoSupport { namespace boost { namespace program_options {

/// This class extends class ::boost::program_options::typed_value by
/// support of the override_allowed option. this->override_allowed(true)
/// enables the overwrite of older option values by later ones. The
/// last option on the command line wins!
template<class T, class charT = char> 
class typed_value: public ::boost::program_options::typed_value<T, charT> {
  typedef typed_value<T, charT>                           this_type;
  typedef ::boost::program_options::typed_value<T, charT> base_type;
public:
  /// Ctor. The 'store_to' parameter tells where to store
  /// the value when it's known. The parameter can be nullptr.
  typed_value(T *store_to) 
    : base_type(store_to), m_override_allowed(false) {} 

  bool is_override_allowed() const { return m_override_allowed; }

  this_type *override_allowed(bool v = true) {
    m_override_allowed = v;
    return this;
  }

  /// Creates an instance of the 'validator' class and calls
  /// its operator() to perform the actual conversion.
  void xparse(boost::any &value_store, const std::vector<std::basic_string<charT> > &new_tokens) const {
    if (m_override_allowed)
      value_store = boost::any();
    base_type::xparse(value_store, new_tokens);
  }
private:
  bool m_override_allowed;
};

/** Creates a typed_value<T> instance. This function is the primary
    method to create value_semantic instance for a specific type, which
    can later be passed to 'option_description' constructor.
    The second overload is used when it's additionally desired to store the 
    value of option into program variable.
*/
template<class T>
typed_value<T> *value(T *v = nullptr) {
  return new typed_value<T>(v);
}

} } } // namespace CoSupport::boost::program_options

#endif // _INCLUDED_COSUPPORT_BOOST_PROGRAM_OPTIONS_VALUE_SEMANTIC_HPP
