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

#ifndef _INCLUDED_COSUPPORT_TRACING_TRACINGFACTORY_HPP
#define _INCLUDED_COSUPPORT_TRACING_TRACINGFACTORY_HPP

#include <memory>
#include <iostream>
#include <fstream>
#include <map>

#include <deque>

#include "Tracer.hpp"
#include "PtpTracer.hpp"
#include "TaskTracer.hpp"

#include "export_config.h"

namespace CoSupport { namespace Tracing {

/**
 * \brief Enables logging of simulation times
 * @author graf
 */
class COSUPPORT_TRACING_API
TracingFactory {
private:
  /**
   * Singleton design pattern
   */
  TracingFactory();

  // TODO: we may need a map for all (different types) of tracer
  //std::map<std::string, Trace*> traceMap;

  // contains all PtpTracer
  typedef std::map<std::string, PtpTracer::Ptr> PtpMap;
  PtpMap ptpMap;

  typedef std::map<std::string, TaskTracer::Ptr> TaskTracerMap;
  typedef std::map<std::string, TaskTracerMap> ResourceMap;
  ResourceMap resourceMap;

  //std::ofstream traceStream;
  std::string filename;
  std::string absoluteFilename;


public:

  //TracingFactory(std::string id);

  /**
    * Singleton design pattern
    */
   static TracingFactory& getInstance();

  /**
   *
   */
  virtual ~TracingFactory();

  /**
   * set file name for trace output
   * setting a file name is required, no trace output will be written otherwise
   */
  void setTraceFile(std::string fileName);

  /**
     * set file name for absolute timestamp trace output (e.g.
     * setting a file name is required, no trace output will be written otherwise
     */
  void setAbsoluteTraceFile(std::string fileName);

  PtpTracer::Ptr createPtpTracer(std::string id);

  TaskTracer::Ptr createTaskTracer(std::string task, std::string resource);

};

} } // namespace CoSupport::Tracing

#endif // _INCLUDED_COSUPPORT_TRACING_TRACINGFACTORY_HPP
