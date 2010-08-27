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

#include <CoSupport/Tracing/TracingFactory.hpp>
#include <fstream>

namespace CoSupport { namespace Tracing {

/**
 *
 */
std::auto_ptr<TracingFactory> TracingFactory::singleton(new TracingFactory());

/**
 *
 */
TracingFactory::TracingFactory()
{};

/**
 *
 */
TracingFactory& TracingFactory::getInstance(){
  return *singleton;
}

/**
 *
 */
Tracing* TracingFactory::getInstancefromID(std::string id){
  if(traceMap[id] != 0){
    return traceMap[id];
  }else{
    std::cerr<<"Error! ID not registered!"<<std::endl;
    return 0;
  }

}

Tracing* TracingFactory::getPTPTracingObject(std::string key){
  if(traceMap[key] != 0){
      return traceMap[key];
    }else{
      std::cerr<<"creating new Trace-object " << key <<std::endl;
      Tracing* newTraceObject = new PTPTracing(key);
      traceMap[key] = newTraceObject;
      return newTraceObject;
    }

}

/**
 *
 */
void TracingFactory::startUnit(std::string id){
  Tracing* trace = getInstancefromID(id);
  ((PTPTracing*)trace)->startUnit();
}


/**
 *
 */
void TracingFactory::stopUnit(std::string id){
  Tracing* trace = getInstancefromID(id);
  ((PTPTracing*)trace)->stopUnit();
}


/**
 *
 */
TracingFactory::~TracingFactory(){
        //removed (sg)
  //assert(startTimes.size() == stopTimes.size());
  if(traceMap.size() != 0){
    std::cout<<"we have " << traceMap.size() << " elements in the traceMap!"<<std::endl;
  }

for(std::map<std::string, Tracing*>::const_iterator it = traceMap.begin(); it != traceMap.end(); ++it)
      {
          //std::string name =(it->second->getName());
          //name +="result.inversethroughput";
          std::ofstream thr((it->second->getName()).append(".result.csv").c_str());
          thr<<it->second->createReport();
          thr.close();

          it->second->getRAWData();
          //std::cerr << "delete it" << it->second << std::endl;
          //delete((it->second));
      }
//traceMap.clear();



  /*
  if (!startTimes.empty()) {
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
  */
}

} } // namespace CoSupport::SystemC
