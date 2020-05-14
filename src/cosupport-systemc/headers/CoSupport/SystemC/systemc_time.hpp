// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2017 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2018 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_SYSTEMC_SYSTEMC_TIME_SUPPORT_HPP
#define _INCLUDED_COSUPPORT_SYSTEMC_SYSTEMC_TIME_SUPPORT_HPP

#include <CoSupport/compatibility-glue/nullptr.h>
#include <CoSupport/sassert.h>
#include <CoSupport/String/convert.hpp>

#include <systemc>

#include <algorithm>
#include <iostream>
#include <sstream>

#include "export_config.h"

namespace CoSupport {

namespace SystemC {

  /**
   * \brief Takes a string representation of a time (e.g. a delay)
   *        and constructs a sc_core::sc_time object.
   */
  COSUPPORT_SYSTEMC_API
  sc_core::sc_time createSCTime(const char* timeString);

} // SystemC

namespace String {

  template <>
  inline
  sc_core::sc_time strAs<sc_core::sc_time>(const std::string &str)
    { return SystemC::createSCTime(str.c_str()); }

} // String

} // CoSupport

#endif // _INCLUDED_COSUPPORT_SYSTEMC_SYSTEMC_TIME_SUPPORT_HPP
