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
 * returns the Tracing-Object to a given key
 */
Tracing* TracingFactory::getInstancefromID(std::string key){
  if(traceMap[key] != 0){
    return traceMap[key];
  }else{
    std::cerr<<"Error! ID not registered!"<<std::endl;
    return 0;
  }

}

/**
 * returns a PTPTracingObject for the given key - and will create it, if it's not present
 */
Tracing* TracingFactory::getPTPTracingObject(std::string key){
  if(traceMap[key] != 0){
      return traceMap[key];
    }else{
      Tracing* newTraceObject = new PTPTracing(key);
      traceMap[key] = newTraceObject;
      return newTraceObject;
    }

}

/**
 * should not be used - starts the tracing of the Trace-Object with the given key (PTPTracing)
 */
void TracingFactory::startUnit(std::string key){
  Tracing* trace = getInstancefromID(key);
  ((PTPTracing*)trace)->startUnit();
}


/**
 * should not be used - stops the tracing of the Trace-Object with the given key (PTPTracing)
 */
void TracingFactory::stopUnit(std::string key){
  Tracing* trace = getInstancefromID(key);
  ((PTPTracing*)trace)->stopUnit();
}


/**
 * Destructor - generates the report for every Trace-Object and extracts the RAW-Data
 */
TracingFactory::~TracingFactory(){
  //assert(startTimes.size() == stopTimes.size());
  for(std::map<std::string, Tracing*>::const_iterator it = traceMap.begin(); it != traceMap.end(); ++it)
      {
          //std::string name =(it->second->getName());
          //name +="result.inversethroughput";
          std::ofstream thr((it->second->getName() + ".result.csv").c_str());
          thr<<it->second->createReport();
          thr.close();

          it->second->getRAWData();
          delete((it->second));
      }
  traceMap.clear();
}

} } // namespace CoSupport::Tracing
