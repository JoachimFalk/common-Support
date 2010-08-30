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

#ifndef _INCLUDED_COSUPPORT_TRACING_TRACINGFACTORY_HPP
#define _INCLUDED_COSUPPORT_TRACING_TRACINGFACTORY_HPP

#include <systemc.h>

#include <memory>
#include <iostream>
#include <fstream>
#include <CoSupport/Tracing/Tracing.hpp>
#include <CoSupport/Tracing/PTPTracing.hpp>
#include <map>

#include <deque>

namespace CoSupport { namespace Tracing {

/**
 * \brief Enables logging of simulation times
 */
class TracingFactory {

private:
  /**
   * Singleton design pattern
   */
  static std::auto_ptr<TracingFactory> singleton;

  /**
   * Singleton design pattern
   */
  TracingFactory();



  // contains all Trace-Objects associated with Trace-IDs
  std::map<std::string, Tracing*> traceMap;

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


  Tracing* getInstancefromID(std::string id);


  Tracing* getPTPTracingObject(std::string id);

  void startUnit(std::string id);

  void stopUnit(std::string id);

};

} } // namespace CoSupport::Tracing

#endif // _INCLUDED_COSUPPORT_TRACING_TRACINGFACTORY_HPP