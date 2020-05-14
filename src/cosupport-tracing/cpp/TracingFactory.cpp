// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2010 FAU -- Martin Streubuehr <martin.streubuehr@fau.de>
 *   2010 FAU -- Nina Muehleis <nina.muehleis@cs.fau.de>
 *   2010 FAU -- Sebastian Graf <sebastian.graf@fau.de>
 *   2012 FAU -- Sebastian Graf <sebastian.graf@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <CoSupport/Tracing/TracingFactory.hpp>
#include <CoSupport/Tracing/TaskTracer.hpp>
#include <CoSupport/Tracing/PtpTracer.hpp>
#include <fstream>
#include <vector>
#include <memory>

namespace CoSupport { namespace Tracing {


/**
 *
 */
TracingFactory::TracingFactory()
  {}

/**
 * Singleton design pattern
 */
TracingFactory &TracingFactory::getInstance() {
  static std::unique_ptr<TracingFactory> singleton(new TracingFactory());
  return *singleton;
}

//
void TracingFactory::setTraceFile(std::string fileName){
  filename = fileName;
}

void TracingFactory::setAbsoluteTraceFile(std::string fileName){
  absoluteFilename = fileName;
}

//
PtpTracer::Ptr TracingFactory::createPtpTracer(std::string key){
  if (ptpMap.find(key) == ptpMap.end()){
    PtpTracer::Ptr tracer = PtpTracer::Ptr(new PtpTracer(key));
    ptpMap[key] = tracer;
  }
  return ptpMap[key];
}

TaskTracer::Ptr TracingFactory::createTaskTracer(std::string task,
    std::string resource)
{
  TaskTracerMap &taskTracerMap = resourceMap[resource];
  if (taskTracerMap.find(task) == taskTracerMap.end()) {
    TaskTracer::Ptr tracer = TaskTracer::Ptr(new TaskTracer(task, resource));
    taskTracerMap[task] = tracer;
  }
  return taskTracerMap[task];
}


/**
 * Destructor - generates the report for every Trace-Object and extracts the RAW-Data
 */
TracingFactory::~TracingFactory(){
  //assert(startTimes.size() == stopTimes.size());
  std::ofstream traceStream(filename.c_str());
  std::ofstream aTraceStream(absoluteFilename.c_str());
  if( traceStream.good() && aTraceStream.good() ){
    traceStream << "#\n" << "# PtpTacer";
    std::vector<std::string> sequence;
    sequence.push_back(Tracer::AVG_LATENCY);
    sequence.push_back(Tracer::MIN_LATENCY);
    sequence.push_back(Tracer::MAX_LATENCY);
    sequence.push_back(Tracer::START_STOP);
    //TODO: sequence.push_back(throughput);

    // write header
    for (std::vector<std::string>::const_iterator iter = sequence.begin();
        iter!= sequence.end(); ++iter){
      traceStream << "\t" << *iter;
    }
    traceStream << std::endl;

    // for each PtpTracer: write data
    for(PtpMap::const_iterator it = ptpMap.begin(); it != ptpMap.end(); ++it) {
      it->second->createCsvReport(traceStream, aTraceStream, sequence);
      it->second->getRAWData();
    }

    // for each TaskTracer: write data
    for(ResourceMap::const_iterator resIter = resourceMap.begin();
        resIter != resourceMap.end();
        ++resIter) {
      const TaskTracerMap &taskTracerMap = resIter->second;
      for (TaskTracerMap::const_iterator it = taskTracerMap.begin(); it
          != taskTracerMap.end(); ++it) {
        it->second->createCsvReport(traceStream, aTraceStream, sequence);
      }
    }

    traceStream.close();
    aTraceStream.close();
  }

  ptpMap.clear();
}

} } // namespace CoSupport::Tracing
