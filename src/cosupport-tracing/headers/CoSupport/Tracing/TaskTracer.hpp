// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2010 FAU -- Martin Streubuehr <martin.streubuehr@fau.de>
 *   2010 FAU -- Sebastian Graf <sebastian.graf@fau.de>
 *   2011 FAU -- Martin Streubuehr <martin.streubuehr@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_TRACING_TASKTRACER_HPP
#define _INCLUDED_COSUPPORT_TRACING_TASKTRACER_HPP

#include "PtpTracer.hpp"

#include <string>
#include <sstream>

namespace CoSupport { namespace Tracing {

/*
 * TaskTracer
 *      Author: streubuehr
 */
class COSUPPORT_TRACING_API TaskTracer: private PtpTracer {
public:
  typedef boost::shared_ptr<TaskTracer> Ptr;
  typedef PtpTracer::Ticket Ticket;

  TaskTracer(std::string task, std::string resource) :
    PtpTracer(task), resource_(resource)
  {
  }

  void createCsvReport(std::ostream &stream, std::ostream &absoluteStream,
      const std::vector<std::string> &sequence)
  {
    stream << resource_ << "/";
    absoluteStream << resource_ << "/";
    PtpTracer::createCsvReport(stream, absoluteStream, sequence);
  }

  Ticket releaseTask()
  {
    return this->startOoo();
  }

  void finishTaskLatency(const Ticket& ticket)
  {
    this->stopOoo(ticket);
  }

  std::string getRAWData()
  {
    return PtpTracer::getRAWData();
  }

  std::string getName()
  {
    std::stringstream result;
    result << PtpTracer::getName() << "@" << resource_;
    return result.str();
  }
private:
  std::string resource_;
};

} } // namespace CoSupport::Tracing

#endif /* _INCLUDED_COSUPPORT_TRACING_TASKTRACER_HPP */
