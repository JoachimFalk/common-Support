//  -*- tab-width:8; intent-tabs-mode:nil;  c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c) 2007 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
 * 
 *   This program is free software; you can redistribute it and/or modify it under
 *   the terms of the GNU General Public License as published by the Free Software
 *   Foundation; either version 2 of the License, or (at your option) any later
 *   version.
 * 
 *   This program is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *   details.
 * 
 *   You should have received a copy of the GNU General Public License along with
 *   this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 *   Place, Suite 330, Boston, MA 02111-1307 USA.
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

#include <cosupport/PerformanceEvaluation.hpp>
#include <fstream>


/**
 *
 */
std::auto_ptr<PerformanceEvaluation> PerformanceEvaluation::singleton(new PerformanceEvaluation());

/**
 *
 */
PerformanceEvaluation::PerformanceEvaluation()
  : measureStart(SC_ZERO_TIME) {};

/**
 *
 */
PerformanceEvaluation& PerformanceEvaluation::getInstance(){
  return *singleton;
}

/**
 *
 */
void PerformanceEvaluation::startUnit(){
  startTimes.push_back( sc_time_stamp() );
}

/**
 *
 */
void PerformanceEvaluation::stopUnit(){
  stopTimes.push_back( sc_time_stamp() );
}

/**
 *
 */
void PerformanceEvaluation::startSimulation(){
  measureStart = sc_time_stamp();
}


/**
 *
 */
PerformanceEvaluation::~PerformanceEvaluation(){
  assert(startTimes.size() == stopTimes.size());

  // store number of samples
  size_t sampleCount = startTimes.size();
  sc_time averageLatency;

  // calculate inverse throughput
  sc_time lastSample = stopTimes.back();
  sc_time averageInverseThroughput = (lastSample - measureStart)/sampleCount;

  // sum up latencies
  while( startTimes.size() ){
    const sc_time& start = startTimes.front();
    const sc_time& stop  = stopTimes.front();

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