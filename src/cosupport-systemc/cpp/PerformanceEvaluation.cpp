// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2010 FAU -- Martin Streubuehr <martin.streubuehr@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <CoSupport/SystemC/PerformanceEvaluation.hpp>
#include <fstream>
#include <memory>

namespace CoSupport { namespace SystemC {

/**
 *
 */
PerformanceEvaluation::PerformanceEvaluation()
  : measureStart(sc_core::SC_ZERO_TIME) {}

/**
 * Singleton design pattern
 */
PerformanceEvaluation &PerformanceEvaluation::getInstance() {
  static std::unique_ptr<PerformanceEvaluation> singleton(new PerformanceEvaluation());
  return *singleton;
}

/**
 *
 */
void PerformanceEvaluation::startUnit(){
  startTimes.push_back( sc_core::sc_time_stamp() );
}

/**
 *
 */
void PerformanceEvaluation::stopUnit(){
  stopTimes.push_back( sc_core::sc_time_stamp() );
}

/**
 *
 */
void PerformanceEvaluation::startSimulation(){
  measureStart = sc_core::sc_time_stamp();
}


/**
 *
 */
PerformanceEvaluation::~PerformanceEvaluation(){
	//removed (sg)
  //assert(startTimes.size() == stopTimes.size());
  
  if (!startTimes.empty()) {
    // store number of samples
    size_t sampleCount = startTimes.size();
    sc_core::sc_time averageLatency;
    
    // calculate inverse throughput
    sc_core::sc_time lastSample = stopTimes.back();
    sc_core::sc_time averageInverseThroughput = (lastSample - measureStart)/sampleCount;
    
    // sum up latencies
    while( startTimes.size() ){
      const sc_core::sc_time& start = startTimes.front();
      const sc_core::sc_time& stop  = stopTimes.front();
      
      //std::cerr << "sample: " << start << " -> " << stop << std::endl;
      averageLatency  += stop-start;
      
      startTimes.pop_front();
      stopTimes.pop_front();
    }
    
    // compute avarage latency
    averageLatency = averageLatency / sampleCount;
    
    // write latency
    std::ofstream lat("result.latency");
    if( lat.is_open() ){
      lat << averageLatency.to_default_time_units() << std::endl;
    }
    lat.close();
    
    //write inverse throughput
    std::ofstream thr("result.inversethroughput");
    if( thr.is_open() ){
      thr << averageInverseThroughput.to_default_time_units() << std::endl;
    }
    thr.close();
  }
}

} } // namespace CoSupport::SystemC
