#include <iostream>

#include <systemc_support.hpp>

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
      // std::cout << name() << ": Event notify at " << sc_simulation_time() << "ns" << std::endl;
      wait(rtime, SC_NS);
      s = false; reset(e);
      // std::cout << name() << ": Event reset at " << sc_simulation_time() << "ns" << std::endl;
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
  EventAndList<EventWaiter> e;
  
  void process() {
    while ( 1 ) {
      CoSupport::SystemC::wait(e);
      if (e.reset()) {
        assert(a2.s && a3.s && a4.s && a5.s && a6.s);
        a2.s = a3.s = a4.s = a5.s = a6.s = false;
        std::cout << name() << ": Event received at " << sc_simulation_time() << "ns" << std::endl;
      }
    }
  }
  
  SC_HAS_PROCESS(m_waiterI);
  
public:
  m_waiterI(sc_module_name name)
    : sc_module(name), e(a2.e & a3.e & a4.e & a5.e & a6.e) {
    SC_THREAD(process);
  }
};

class m_waiterII: public sc_module {
  EventAndList<EventWaiter> e;
  
  void process() {
    while ( 1 ) {
      CoSupport::SystemC::wait(e);
      if (e.reset()) {
        assert(a1.s && a2.s && a3.s && a4.s && a5.s);
        a1.s = a2.s = a3.s = a4.s = a5.s = false;
        std::cout << name() << ": Event received at " << sc_simulation_time() << "ns" << std::endl;
      }
    }
  }
  
  SC_HAS_PROCESS(m_waiterII);
public:
  m_waiterII(sc_module_name name)
    : sc_module(name), e(a1.e & a2.e & a3.e & a4.e & a5.e) {
    SC_THREAD(process);
  }
};

int sc_main(int argc, char *argv[]) {
  m_waiterI  w1("w1");
  m_waiterII w2("w2");
  
  sc_start(100000, SC_NS);
  return 0;
}
