/*
 * Copyright (c) 2004-2006 Hardware-Software-CoDesign, University of
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

#include <iostream>

#include <cosupport/systemc_support.hpp>

using namespace CoSupport::SystemC;

class m_signaler: public sc_module {
public:
  Event e;
  bool  s;
protected:
  int   stime, rtime;
  
  void process() {
    while ( 1 ) {
      wait(stime, SC_NS);
      s = true; notify(e);
      std::cout << name() << ": Event notify at " << sc_time_stamp().to_default_time_units() << "ns" << std::endl;
      wait(rtime, SC_NS);
      s = false; reset(e);
      std::cout << name() << ": Event reset at " << sc_time_stamp().to_default_time_units() << "ns" << std::endl;
    }
  }
  
  SC_HAS_PROCESS(m_signaler);

public:
  m_signaler(sc_module_name name, int stime, int rtime)
    : sc_module(name), s(false), stime(stime), rtime(rtime) {
    SC_THREAD(process);
  }
};

m_signaler a1("a1",  10,  9);
m_signaler a2("a2", 101, 32);
m_signaler a3("a3",  21, 55);
m_signaler a4("a4",  12, 13);
m_signaler a5("a5",   3,  1);
m_signaler a6("a6",  63, 57);


class m_waiterI: public sc_module {
  EventOrList<EventWaiter> e;
  
  void process() {
    while ( 1 ) {
      CoSupport::SystemC::wait(e);
      wait(7, SC_NS);
      
      EventWaiter *x = e.reset();
      if (x) {
        if (&a2.e == x) {
          assert(a2.s); a2.s = false;
        } else if (&a3.e == x) {
          assert(a3.s); a3.s = false;
        } else if (&a4.e == x) {
          assert(a4.s); a4.s = false;
        } else if (&a5.e == x) {
          assert(a5.s); a5.s = false;
        } else {
          assert(&a6.e == x && a6.s); a6.s = false;
        }
        std::cout << name() << ": Event received at " << sc_time_stamp().to_default_time_units() << "ns" << std::endl;
      }
    }
  }
  
  SC_HAS_PROCESS(m_waiterI);
  
public:
  m_waiterI(sc_module_name name)
    : sc_module(name), e(a2.e | a3.e | a4.e | a5.e | a6.e) {
    SC_THREAD(process);
  }
};

class m_waiterII: public sc_module {
  EventOrList<EventWaiter> e;
  
  void process() {
    while ( 1 ) {
      CoSupport::SystemC::wait(e);
      EventWaiter *x = e.reset();
      
      if (x) {
        if (&a1.e == x) {
          assert(a1.s); a1.s = false;
        } else if (&a2.e == x) {
          assert(a2.s); a2.s = false;
        } else if (&a3.e == x) {
          assert(a3.s); a3.s = false;
        } else if (&a4.e == x) {
          assert(a4.s); a4.s = false;
        } else {
          assert(&a5.e == x);
          assert(a5.s); a5.s = false;
        }
        std::cout << name() << ": Event received at " << sc_time_stamp().to_default_time_units() << "ns" << std::endl;
        wait(13, SC_NS);
      }
    }
  }
  
  SC_HAS_PROCESS(m_waiterII);
public:
  m_waiterII(sc_module_name name)
    : sc_module(name), e(a1.e | a2.e | a3.e | a4.e | a5.e) {
    SC_THREAD(process);
  }
};

int sc_main(int argc, char *argv[]) {
  m_waiterI  w1("w1");
  m_waiterII w2("w2");
  
  sc_start(100000, SC_NS);
  return 0;
}
