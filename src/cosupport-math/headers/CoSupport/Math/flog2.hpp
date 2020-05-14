// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Christian Zebelein <christian.zebelein@fau.de>
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2014 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef INCLUDED_COSUPPORT_FLOG2_HPP
#define INCLUDED_COSUPPORT_FLOG2_HPP

#include <stddef.h>
#include <CoSupport/commondefs.h>
#include <CoSupport/compatibility-glue/integertypes.h>

#include "export_config.h"

namespace CoSupport { namespace Math {

// Returns the log base 2 of an integer (floor variant)
// e.g. flog2f(1) == 0
//      ...
//      flog2f(7) == 2
//      flog2f(8) == 3
//      flog2f(9) == 3

COSUPPORT_MATH_API
size_t flog2f(uint32_t n);

COSUPPORT_MATH_API
size_t flog2f(uint64_t n);

// Returns the log base 2 of an integer (ceil variant)
// e.g. flog2c(7) == 3
//      flog2c(8) == 3
//      flog2c(9) == 4
//      (Special case: flog2c(1) == 1)

COSUPPORT_MATH_API
size_t flog2c(uint32_t n);

COSUPPORT_MATH_API
size_t flog2c(uint64_t n);

} } // namespace CoSupport::Math

#endif // INCLUDED_COSUPPORT_FLOG2_HPP
