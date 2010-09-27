//  -*- tab-width:8; intent-tabs-mode:nil;  c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c) 2004-2010 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_TRACING_PTPTRACER_HPP
#define _INCLUDED_COSUPPORT_TRACING_PTPTRACER_HPP

#include <CoSupport/Tracing/Tracer.hpp>
#include <systemc.h>
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
class PtpTracer : public Tracer {
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
  void start(){
    startTimes.push_back( sc_time_stamp() );
  }

  /**
   * trace the end/stop of an unit
   * here we assume that starts and stops of units occur in the same order
   * if interleaving should be support we would need an identifier
   * but thats future work
   */
  void stop(){
    stopTimes.push_back( sc_time_stamp() );
  }

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
  void createCsvReport(std::ostream &stream,
      const std::vector<std::string> &sequence);

  std::string getRAWData();

  std::string getName();

  /**
   * start and return sequence Id (used by OooTicket)
   */
  Ticket startOoo() {
    startTimes.push_back( sc_time_stamp() );
    stopTimes.push_back( SC_ZERO_TIME );
    return (startTimes.size() - 1);
  }

  /**
   * trace the end of a ticket
   */
  void stopOoo(const Ticket& ticket){
    //  if (ticket>= stopTimes.size()){
    //    stopTimes.resize(ticket+ 100, SC_ZERO_TIME);
    //  }
      stopTimes[ticket] = sc_time_stamp();
    }
private:

  // contains all logs associated with function calls
  std::deque<sc_time> startTimes;
  std::deque<sc_time> stopTimes;

  sc_time measureStart;
  std::string name;
};

} } // namespace CoSupport::Tracing

#endif // _INCLUDED_COSUPPORT_TRACING_PTPTRACER_HPP
