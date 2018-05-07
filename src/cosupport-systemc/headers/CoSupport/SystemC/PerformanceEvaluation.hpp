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

#ifndef _INCLUDED_COSUPPORT_SYSTEMC_PERFORMANCEEVALUATION_HPP
#define _INCLUDED_COSUPPORT_SYSTEMC_PERFORMANCEEVALUATION_HPP

#include <systemc>

#include <memory>
#include <iostream>
#include <fstream>
#include <deque>

#include "export_config.h"

namespace CoSupport { namespace SystemC {

/**
 * \brief Enables logging of simulation times
 */
class COSUPPORT_SYSTEMC_API
PerformanceEvaluation {
private:
  /**
   * Singleton design pattern
   */
  PerformanceEvaluation();
    
    
  // contains all logs associated with function calls
  std::deque<sc_core::sc_time> startTimes; 
  std::deque<sc_core::sc_time> stopTimes; 

  sc_core::sc_time measureStart;
public:
  
  /**
   * Singleton design pattern
   */
  static PerformanceEvaluation &getInstance();
  
  /**
   *
   */
  virtual ~PerformanceEvaluation();

  /**
   * trace the start of an unit (e.g. a frame, a block,)
   * here we assume that starts and stops of units occure in the same order
   * if interleaving should be support we would need an identifer
   * but thats future work
   */
  void startUnit();

  /**
   * trace the end/stop of an unit
   * here we assume that starts and stops of units occure in the same order
   * if interleaving should be support we would need an identifer
   * but thats future work
   */
  void stopUnit();

  /**
   * optionally you may want to signal another time of simulation startup
   * (another one than SC_ZERO_TIME)
   * if called the actual time stamp of the simulator is used as start time
   * this effect for example the throughput
   */
  void startSimulation();
  
};

} } // namespace CoSupport::SystemC

#endif // _INCLUDED_COSUPPORT_SYSTEMC_PERFORMANCEEVALUATION_HPP
