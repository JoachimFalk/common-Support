// vim: set sw=2 ts=8:
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

#include <CoSupport/SystemC/systemc_time.hpp>

namespace CoSupport { namespace SystemC {

sc_core::sc_time createSCTime(const char* timeString) {
  assert(timeString != nullptr);
  double value = -1;
  std::string unit;

  sc_core::sc_time_unit scUnit = sc_core::SC_NS;

  std::stringstream data(timeString);
  if(data.good()){
    data >> value;
  }else{
    std::string msg("Parsing Error: Unknown argument: <");
    msg += timeString;
    msg += "> How to creating a sc_core::sc_time from?";
    throw msg;
  }
  if( data.fail() ){
    std::string msg("Parsing Error: Unknown argument: <");
    msg += timeString;
    msg += "> How to creating a sc_core::sc_time from?";
    throw msg;
  }
  if(data.good()){
    data >> unit;
    if(data.fail()){
      //std::cerr << "No time unit, taking default: SC_NS!"
      //          << std::endl;
      scUnit = sc_core::SC_NS;
    }else{
      std::transform (unit.begin(),
                      unit.end(),
                      unit.begin(),
                      (int(*)(int))tolower);
      if(      0==unit.compare(0, 2, "fs") ) scUnit = sc_core::SC_FS;
      else if( 0==unit.compare(0, 2, "ps") ) scUnit = sc_core::SC_PS;
      else if( 0==unit.compare(0, 2, "ns") ) scUnit = sc_core::SC_NS;
      else if( 0==unit.compare(0, 2, "us") ) scUnit = sc_core::SC_US;
      else if( 0==unit.compare(0, 2, "ms") ) scUnit = sc_core::SC_MS;
      else if( 0==unit.compare(0, 1, "s" ) ) scUnit = sc_core::SC_SEC;
    }
  }

  return sc_core::sc_time(value, scUnit);
}

} } // CoSupport::SystemC
