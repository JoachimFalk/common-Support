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

#include <CoSupport/Tracing/TracingFactory.hpp>
#include <fstream>
#include <vector>

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

//
PtpTracer::Ptr TracingFactory::createPtpTracer(std::string key){
  std::cerr << "createPtpTracer " << key << " ... ";
  if (ptpMap.find(key) == ptpMap.end()){
    PtpTracer::Ptr tracer = PtpTracer::Ptr(new PtpTracer(key));
    ptpMap[key] = tracer;
    std::cerr << "create new ";
  }
  std::cerr << std::endl;
  return ptpMap[key];
}


/**
 * Destructor - generates the report for every Trace-Object and extracts the RAW-Data
 */
TracingFactory::~TracingFactory(){
  //assert(startTimes.size() == stopTimes.size());
  std::ofstream stream("tracing.log");

  stream << "#\n"
         << "# PtpTacer";
  std::vector<std::string> sequence;
  sequence.push_back(Tracer::AVG_LATENCY);
  sequence.push_back(Tracer::MIN_LATENCY);
  sequence.push_back(Tracer::MAX_LATENCY);
  //TODO: sequence.push_back(throughput);

  for (std::vector<std::string>::const_iterator iter = sequence.begin(); iter
          != sequence.end(); ++iter){
    stream << "\t" << *iter;
  }
  stream << std::endl;
    for(PtpMap::const_iterator it = ptpMap.begin(); it != ptpMap.end(); ++it)
      {
          //std::string name =(it->second->getName());
          //name +="result.inversethroughput";
          it->second->createCsvReport(stream, sequence);

          //it->second->getRAWData();
      }
    stream.close();
  ptpMap.clear();
}

} } // namespace CoSupport::Tracing
