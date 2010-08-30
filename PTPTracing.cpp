//  -*- tab-width:8; intent-tabs-mode:nil;  c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c) 2004-2009 Hardware-Software-CoDesign, University of
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

#include <CoSupport/Tracing/PTPTracing.hpp>
#include <fstream>

namespace CoSupport { namespace Tracing {


/**
 *
 */

  PTPTracing::PTPTracing(std::string id)
  : measureStart(SC_ZERO_TIME) {
    name = id;
  };


/**
 *
 */
void PTPTracing::startUnit(){
  std::cout<<"PTPTracing::startUnit() "<< name << " "<<this << std::endl;
  startTimes.push_back( sc_time_stamp() );
}

/**
 *
 */
void PTPTracing::stopUnit(){
  std::cout<<"PTPTracing::stopUnit() "<< name << " "<<this << std::endl;
  stopTimes.push_back( sc_time_stamp() );
}

/**
 *
 */
void PTPTracing::startSimulation(){
  measureStart = sc_time_stamp();
}


/**
 *
 */
PTPTracing::~PTPTracing(){
  //assert(startTimes.size() >= stopTimes.size());
  std::cerr << "PTPTracing::~PTPTracing size" <<startTimes.size() << " " << stopTimes.size() << std::endl;

  if (!startTimes.empty()) {
    // store number of samples
    size_t sampleCount = startTimes.size();
    sc_time averageLatency;

    // calculate inverse throughput
    sc_time lastSample = stopTimes.back();
    sc_time averageInverseThroughput = (lastSample - measureStart)/sampleCount;

    int count = 0;
    // sum up latencies
    for(std::deque<sc_time>::const_iterator it = stopTimes.begin(); it != stopTimes.end(); ++it){

      const sc_time& start = startTimes[count++];
      const sc_time& stop  = *it;

      //std::cerr << "sample: " << start << " -> " << stop << std::endl;
      averageLatency  += stop-start;

      startTimes.pop_front();
      stopTimes.pop_front();
    }

    // compute avarage latency
    averageLatency = averageLatency / sampleCount;

    // write latency
    std::ofstream lat(((getName() + ".result.latency")).c_str());
    if( lat.is_open() ){
      lat << averageLatency.to_default_time_units() << std::endl;
    }
    lat.close();

    //write inverse throughput
    std::ofstream thr((getName() + ".result.inversethroughput").c_str());
    if( thr.is_open() ){
      thr << averageInverseThroughput.to_default_time_units() << std::endl;
    }
    thr.close();
  }
}

std::string PTPTracing::createReport(){
//  std::cerr<<"startTimes:" << startTimes.size() << " stopTimes:" << stopTimes.size() << std::endl;
  std::stringstream result;

  if (!stopTimes.empty()) {

    if(startTimes.empty()){
      result << "Something very strange happend... PTPTracing has no start_times!" << std::endl;
    }
  // store number of samples
  size_t sampleCount = stopTimes.size();
  sc_time averageLatency;
  sc_time last_trip;
  sc_time min_trip = sc_time(-1, SC_NS);
  sc_time max_trip = sc_time(0,SC_NS);

  // calculate inverse throughput
  sc_time lastSample = stopTimes.back();
  sc_time averageInverseThroughput = (lastSample - measureStart)/sampleCount;

//  result << "trip_duration";
  // sum up latencies
  int count = 0;
  for(std::deque<sc_time>::const_iterator it = stopTimes.begin(); it != stopTimes.end(); ++it){
        const sc_time& start = startTimes[count];
        const sc_time& stop  = *it;
        //std::cerr << "sample: " << start << " -> " << stop << std::endl;
        last_trip = stop - start;
        averageLatency  += last_trip;
        if(last_trip < min_trip) min_trip = last_trip;
        if(last_trip > max_trip) max_trip = last_trip;
        //std::cerr<<"last_trip="<<last_trip<<" min_trip="<<min_trip<<" max_trip="<<max_trip << std::endl;
  //      result << "," << last_trip.to_default_time_units();

        count++;
  }
  //result<<std::endl;

  // compute avarage latency
  averageLatency = averageLatency / sampleCount;

  // write average and max latency
    result << "average,"<< averageLatency.to_default_time_units() << std::endl;
    result << "max_trip," << max_trip.to_default_time_units()  << std::endl;
}
  return result.str();
}



void PTPTracing::writeReportToFile(std::string filename){

}

std::string PTPTracing::getRAWData(){
  //  std::cerr<<"startTimes:" << startTimes.size() << " stopTimes:" << stopTimes.size() << std::endl;

    std::stringstream result;

    if (!stopTimes.empty()) {
    // store number of samples
    sc_time last_trip;

    int count = 0;
    // sum up latencies
    for(std::deque<sc_time>::const_iterator it = stopTimes.begin(); it != stopTimes.end(); ++it){
      const sc_time& start = startTimes[count];
      const sc_time& stop  = *it;

      //std::cerr << "sample: " << start << " -> " << stop << std::endl;
      last_trip = stop - start;
      result << count++ << " " << last_trip.to_default_time_units() << std::endl;

    }

  }
    std::ofstream thr((getName() + ".RAW").c_str());
    thr<<result.str();
    thr.close();
    return result.str();
}

std::string PTPTracing::getName(){
  return name;
}

} } // namespace CoSupport::SystemC