// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2010 FAU -- Martin Streubuehr <martin.streubuehr@fau.de>
 *   2010 FAU -- Nina Muehleis <nina.muehleis@cs.fau.de>
 *   2010 FAU -- Sebastian Graf <sebastian.graf@fau.de>
 *   2012 FAU -- Sebastian Graf <sebastian.graf@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_TRACING_TRACER_HPP
#define _INCLUDED_COSUPPORT_TRACING_TRACER_HPP

#include <memory>
#include <iostream>
#include <fstream>

#include <deque>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "export_config.h"

namespace CoSupport { namespace Tracing {

/**
 * \brief Enables logging of simulation times
 */
class COSUPPORT_TRACING_API Tracer {
public:
  typedef boost::shared_ptr<Tracer>  Ptr;

  static const std::string AVG_LATENCY;
  static const std::string MIN_LATENCY;
  static const std::string MAX_LATENCY;
  static const std::string START_STOP;

  virtual ~Tracer() {}
  /*
   * creates a CVS-Report from the Simulation-results
   */
  virtual void createCsvReport(std::ostream &stream, std::ostream &absoluteStream,
       const std::vector<std::string> &sequence) = 0;

  /*
   * dumps the acquired data
   */
  virtual std::string getRAWData() = 0;

  /**
   *
   */
  virtual std::string getName() = 0;

};

} } // namespace CoSupport::SystemC

#endif // _INCLUDED_COSUPPORT_SYSTEMC_TRACER_HPP
