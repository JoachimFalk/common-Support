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

#include <CoSupport/Tracing/PtpTracer.hpp>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

namespace CoSupport { namespace Tracing {


//
  PtpTracer::PtpTracer(std::string id)
  : measureStart(SC_ZERO_TIME) {
    name = id;
  };

//
void PtpTracer::startSimulation(){
  measureStart = sc_time_stamp();
}

template<typename T>
std::string toString(const T& obj) {
  std::ostringstream out; out << obj; return out.str();
}

//
void PtpTracer::createCsvReport(std::ostream &result, std::ostream &absoluteStream,
    const std::vector<std::string> &sequence)
{
  std::map<std::string, std::string> resultMap;
  result << this->name;
  absoluteStream << this->name <<"\t";

  if (!stopTimes.empty()) {
    if(startTimes.empty()){
      result << this->name << " - Something very strange happened... PtpTracer has no startTimes, but stopTimes!" << std::endl;
    }else{

      // filter missing end times

      sc_time averageLatency;
      sc_time last_trip;
      sc_time min_trip = sc_time(-1, SC_NS);
      sc_time max_trip = sc_time(0,SC_NS);

      sc_time lastSample = measureStart;
      size_t sampleCount = 0;

      std::string start_stop = "";

      // sum up latencies
      int countMax = 0;
      if(stopTimes.size() < startTimes.size()){
        countMax = stopTimes.size();
      }else{
        countMax = startTimes.size();
      }
      for(size_t count = 0; count < countMax; ++count){
        const sc_time& start = startTimes[count];
        const sc_time& stop  = stopTimes[count];
        if(&start == 0){
          std::cout<<"strange error for " << this->name << " no startTime, but a stopTime (" << stop << "!" << " last start was: " << startTimes[count-1] << std::endl;
          std::cout<<"and it has: " << startTimes.size() << " start and stop: " << stopTimes.size() << std::endl;
        }
        if(start > stop) {
          continue;
        }
        lastSample = stop;
        sampleCount++;

        last_trip = stop - start;
        averageLatency  += last_trip;
        if(last_trip < min_trip) min_trip = last_trip;
        if(last_trip > max_trip) max_trip = last_trip;

        start_stop = start_stop + toString(last_trip.to_default_time_units());
        absoluteStream << toString(((long)start.to_default_time_units())) + "," + toString(((long)stop.to_default_time_units()));
        if(count < stopTimes.size()-1){
          start_stop = start_stop + ",";
          absoluteStream << ";";
        }

      }

      // calculate inverse throughput
      sc_time averageInverseThroughput = (lastSample - measureStart)/sampleCount;

      // compute average latency
      averageLatency = averageLatency / sampleCount;

      resultMap[Tracer::AVG_LATENCY] = toString(averageLatency.to_default_time_units());
      resultMap[Tracer::MAX_LATENCY] = toString(max_trip.to_default_time_units());
      resultMap[Tracer::MIN_LATENCY] = toString(min_trip.to_default_time_units());
      resultMap[Tracer::START_STOP] = start_stop;
      // write the csv line
      // write average, max and min latency starting with a tabulator
      for(std::vector<std::string>::const_iterator iter
          = sequence.begin(); iter != sequence.end(); ++iter){
        assert(resultMap.find(*iter) != resultMap.end());
        result << "\t" << resultMap[*iter];
      }
    }
  }

      absoluteStream << std::endl;
      result << std::endl;
}

/**
 *
 *
 */
std::string PtpTracer::getRAWData(){
    std::stringstream result;
    if (!stopTimes.empty() && !startTimes.empty()) {
    sc_time last_trip;
    unsigned int count = 0;
    // sum up latencies
    for(std::deque<sc_time>::const_iterator it = stopTimes.begin(); it != stopTimes.end(); ++it){
      const sc_time& start = startTimes[count];
      const sc_time& stop  = *it;
      if(count < startTimes.size() && count < stopTimes.size()){
        last_trip = stop - start;
        result << start.to_default_time_units()  <<"\t" << stop.to_default_time_units()  << "\t" << last_trip.to_default_time_units() << std::endl;
        count++;
      }
    }
    std::ofstream thr((getName() + ".RAW").c_str());
    thr<<result.str();
    thr.close();
  }
    return result.str();
}

std::string PtpTracer::getName(){
  return name;
}

} } // namespace CoSupport::Tracing
