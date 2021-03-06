// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2010 FAU -- Martin Streubuehr <martin.streubuehr@fau.de>
 *   2010 FAU -- Sebastian Graf <sebastian.graf@fau.de>
 *   2012 FAU -- Sebastian Graf <sebastian.graf@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2017 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_TRACING_PTPTRACER_HPP
#define _INCLUDED_COSUPPORT_TRACING_PTPTRACER_HPP

#include "Tracer.hpp"

#include <systemc>

#include <memory>
#include <deque>
#include <string>
#include <boost/shared_ptr.hpp>

namespace CoSupport { namespace Tracing {

/**
 * \brief Enables logging of simulation times - PointToPoint Connections with start and stop
 * @author graf
 *
 * use the PtpTracer to trace in-order operations only
 * i.e. the n-th stop() signal corresponds to the n-th start() signal
 *
 * e.g..
 * ptpTracer = factory->createPtpTracer("foo");
 * ptpTracer->start() #0
 * ptpTracer->start() #1
 * ptpTracer->stop()  #0
 * ptpTracer->stop()  #1
 *
 * use a Ticket to trace out-of-order operations
 * ptpTracer = factory->createPtpTracer("foo");
 * Ticket ticket0 = ptpTracer->startOoo() #0
 * Ticket ticket1 = ptpTracer->startOoo() #1
 * ptpTracer->stopOoo(ticket1)            #1
 * ptpTracer->stopOoo(ticket0)            #0
 */
class COSUPPORT_TRACING_API PtpTracer: public Tracer {
public:
  typedef boost::shared_ptr<PtpTracer>  Ptr;
  typedef size_t                        Ticket;
  /**
   * Creates a new PTP-Tracing Object with the given name
   */
  PtpTracer(std::string id);

  /**
   * trace the start of an unit (e.g. a frame, a block,)
   * here we assume that starts and stops of units occur in the same order
   * if interleaving should be support we would need an identifier
   * but thats future work
   */
  void start()
    { startTimes.push_back(sc_core::sc_time_stamp()); }

  /**
   * trace the end/stop of an unit
   * here we assume that starts and stops of units occur in the same order
   * if interleaving should be support we would need an identifier
   * but thats future work
   */
  void stop()
    { stopTimes.push_back(sc_core::sc_time_stamp()); }

  /**
   * optionally you may want to signal another time of simulation startup
   * (another one than SC_ZERO_TIME)
   * if called the actual time stamp of the simulator is used as start time
   * this effect for example the throughput
   */
  void startSimulation();

  /**
   * Creates a CSV-Report (type,value) with some calculates results of the measures (average, max_trip, min_trip)
   */
  void createCsvReport(std::ostream &stream, std::ostream &absoluteStream,
      const std::vector<std::string> &sequence);

  std::string getRAWData();

  std::string getName();

  /**
   * start and return sequence Id (used by OooTicket)
   */
  Ticket startOoo() {
    startTimes.push_back(sc_core::sc_time_stamp());
    stopTimes.push_back(sc_core::SC_ZERO_TIME);
    return (startTimes.size() - 1);
  }

  /**
   * trace the end of a ticket
   */
  void stopOoo(Ticket const ticket) {
    assert(ticket < stopTimes.size());
//  if (ticket>= stopTimes.size())
//    stopTimes.resize(ticket+ 100, sc_core::SC_ZERO_TIME);
    stopTimes[ticket] = sc_core::sc_time_stamp();
  }
private:

  // contains all logs associated with function calls
  std::deque<sc_core::sc_time> startTimes;
  std::deque<sc_core::sc_time> stopTimes;

  sc_core::sc_time measureStart;
  std::string name;
};

} } // namespace CoSupport::Tracing

#endif // _INCLUDED_COSUPPORT_TRACING_PTPTRACER_HPP
