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

#include <iostream>
#include <iomanip>

#include <CoSupport/SystemC/systemc_support.hpp>

using namespace CoSupport::SystemC;

enum STATUS {
  TESTED = 0,
  NOTIFIED,
  DENOTIFIED,
  NOTIFIED_TESTED,
  DENOTIFIED_TESTED,
};

std::pair<sc_core::sc_time, STATUS> w1Status(sc_core::sc_time(), DENOTIFIED_TESTED);
std::pair<sc_core::sc_time, STATUS> w2Status(sc_core::sc_time(), DENOTIFIED_TESTED);

static
void update();

class m_signaler: public sc_module {
public:
  Event e;
  bool  s;
protected:
  int   stime, rtime;
  
  void process() {
    while ( 1 ) {
      wait(stime, SC_NS);
      std::cout << "@" << std::setw(5) << std::setfill('0') << sc_time_stamp().to_default_time_units() << "ns: " << name() << ": Event notified" << std::endl;
      s = true;
      update();
      notify(e);
      //std::cout << std::endl;
      wait(rtime, SC_NS);
      std::cout << "@" << std::setw(5) << std::setfill('0') << sc_time_stamp().to_default_time_units() << "ns: " << name() << ": Event reseted" << std::endl;
      s = false;
      update();
      reset(e);
      //std::cout << std::endl;
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

static
void update() {
  if (w1Status.first != sc_time_stamp())
    assert(
      w1Status.second == TESTED ||
      w1Status.second == NOTIFIED_TESTED ||
      w1Status.second == DENOTIFIED_TESTED);
  if (a2.s || a3.s || a4.s || a5.s || a6.s) {
    if (w1Status.second != NOTIFIED_TESTED) {
      w1Status.first  = sc_time_stamp();
      w1Status.second = NOTIFIED;
    }
  } else {
    if (w1Status.second != DENOTIFIED_TESTED) {
      w1Status.first  = sc_time_stamp();
      w1Status.second = DENOTIFIED;
    }
  }
  if (w2Status.first != sc_time_stamp())
    assert(
      w2Status.second == TESTED ||
      w2Status.second == NOTIFIED_TESTED ||
      w2Status.second == DENOTIFIED_TESTED);
  if (a1.s || a2.s || a3.s || a4.s || a5.s) {
    if (w2Status.second != NOTIFIED_TESTED) {
      w2Status.first  = sc_time_stamp();
      w2Status.second = NOTIFIED;
    }
  } else {
    if (w2Status.second != DENOTIFIED_TESTED) {
      w2Status.first  = sc_time_stamp();
      w2Status.second = DENOTIFIED;
    }
  }
}

class m_waiterI: public sc_module {
  EventOrList<EventWaiter> e;
  
  void process() {
    while ( 1 ) {
      CoSupport::SystemC::wait(e);
//    wait(7, SC_NS); WTF?!
      std::cout << "@" << std::setw(5) << std::setfill('0') << sc_time_stamp().to_default_time_units() << "ns: " << name() << ": wait returned";
      assert(
          w1Status.second == NOTIFIED ||
          w1Status.second == DENOTIFIED);
      assert(w1Status.first == sc_time_stamp());
      if (e) {
        std::cout << " (active) " << std::endl;
        assert(w1Status.second == NOTIFIED);
        w1Status.second = NOTIFIED_TESTED;
        do {
          EventWaiter &x = e.getEventTrigger();
          if (&a2.e == &x) {
            assert(a2.s); a2.s = false;
          } else if (&a3.e == &x) {
            assert(a3.s); a3.s = false;
          } else if (&a4.e == &x) {
            assert(a4.s); a4.s = false;
          } else if (&a5.e == &x) {
            assert(a5.s); a5.s = false;
          } else {
            assert(&a6.e == &x);
            assert(a6.s); a6.s = false;
          }
          update();
          if (w1Status.second == DENOTIFIED)
            w1Status.second = DENOTIFIED_TESTED;
          sassert(&x == e.reset());
          assert(!x.isActive());
        } while (w1Status.second == NOTIFIED_TESTED);
      } else {
        std::cout << " (inactive)" << std::endl;
        assert(w1Status.second == DENOTIFIED);
        w1Status.second = DENOTIFIED_TESTED;
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
      std::cout << "@" << std::setw(5) << std::setfill('0') << sc_time_stamp().to_default_time_units() << "ns: " << name() << ": wait returned";
      assert(
          w2Status.second == NOTIFIED ||
          w2Status.second == DENOTIFIED);
      assert(w2Status.first == sc_time_stamp());
      if (e) {
        std::cout << " (active) " << std::endl;
        assert(w2Status.second == NOTIFIED);
        w2Status.second = NOTIFIED_TESTED;
        do {
          EventWaiter &x = e.getEventTrigger();
          if (&a1.e == &x) {
            assert(a1.s); a1.s = false;
          } else if (&a2.e == &x) {
            assert(a2.s); a2.s = false;
          } else if (&a3.e == &x) {
            assert(a3.s); a3.s = false;
          } else if (&a4.e == &x) {
            assert(a4.s); a4.s = false;
          } else {
            assert(&a5.e == &x);
            assert(a5.s); a5.s = false;
          }
          update();
          if (w2Status.second == DENOTIFIED)
            w2Status.second = DENOTIFIED_TESTED;
          sassert(&x == e.reset());
          assert(!x.isActive());
        } while (w2Status.second == NOTIFIED_TESTED);
      } else {
        std::cout << " (inactive)" << std::endl;
        assert(w2Status.second == DENOTIFIED);
        w2Status.second = DENOTIFIED_TESTED;
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

struct blub : public EventListener {
  void signaled(EventWaiter* e)
  { std::cout << "--> blub::signaled(): e = " << *e << std::endl; }
  void eventDestroyed(EventWaiter *e)
  { std::cout << "blub::eventDestroyed(): e = " << *e << std::endl; }
};

int sc_main(int argc, char *argv[]) {
  m_waiterI  w1("w1");
  m_waiterII w2("w2");
  sc_start(100000, SC_NS);
  
  Event* a = new Event();
  Event* b = new Event();

  EventOrList<EventWaiter> l;
  blub bl;
  l.addListener(&bl);

  l |= *a;
  l |= *b;

  a->notify(); // trigger 1
  b->notify();

  a->reset();
  l.remove(*a);

  b->reset(); // trigger 2
  std::cout << "deleting " << *b << std::endl;
  delete b;

  a->notify();
  l |= *a; // trigger 3

  l.clear(); // trigger 4

  l.delListener(&bl);
  
  return 0;
}
