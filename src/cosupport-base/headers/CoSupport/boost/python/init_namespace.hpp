// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2014 FAU -- Tobias Schwarzer <tobias.schwarzer@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_BOOST_PYTHON_INIT_NAMESPACE_HPP
#define _INCLUDED_COSUPPORT_BOOST_PYTHON_INIT_NAMESPACE_HPP

#include "../init_namespace.hpp"

#include <boost/python.hpp>

namespace CoSupport { namespace boost { namespace python {

using namespace ::boost::python;

template <typename T>
class register_to_python_converter;

template <typename T>
class register_from_python_converter;

template <typename T>
class register_tofrom_python_converter
  : public register_to_python_converter<T>,
    public register_from_python_converter<T> {};

} } } // namespace CoSupport::boost::python

#endif // _INCLUDED_COSUPPORT_BOOST_PYTHON_INIT_NAMESPACE_HPP
