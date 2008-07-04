// vim: set sw=2 ts=8:
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

#ifndef _INCLUDED_COSUPPORT_SYSTEMC_SUPPORT_HPP
#define _INCLUDED_COSUPPORT_SYSTEMC_SUPPORT_HPP

#include <systemc.h>

#include <iostream>

#include <list>
#include <set>
#include <algorithm>

#include "../sassert.h"

/*#include "filter_ostream.hpp"

namespace Detail {

  struct DebugOStream : public CoSupport::FilterOStream {
    CoSupport::IndentStreambuf bufIdt;
    DebugOStream(std::ostream &os) :
      FilterOStream(os)
    {
      insert(bufIdt);
    }
  };
}

static Detail::DebugOStream outDbg(std::cout);
#define outDbg std::cout*/

namespace CoSupport { namespace SystemC {

  /**
   * forward declarations
   */
  class EventWaiter;

  /**
   * base class for all classes which must be notified upon
   * activation / deactivation of some event
   */ 
  class EventListener {
  private:
    // must call signaled / eventDestroyed
    friend class EventWaiter;
  
  protected:
    // Tell this listener about an event changing in EventWaiter e
    // (e.g. the waiter was reseted or notified; check e->isActive()(
    virtual void signaled(EventWaiter *e) = 0;

    // The lifetime of the given EventWaiter is over
    virtual void eventDestroyed(EventWaiter *e) = 0;

    // virtual destructor for derived classes
    virtual ~EventListener() {}
  };

  /**
   * base class for all classes which represent some kind of event,
   * listeners can be added which will be notified upon activation /
   * deactivation of this event:
   *
   * ....................|________________|....................
   *      not active     |     active     |     not active     
   *                     ^                ^
   *      signalNotifyListener()  signalResetListener()
   */ 
  class EventWaiter {
  public:
    typedef EventWaiter this_type;
    typedef void (this_type::*unspecified_bool_type)(EventListener *);
  protected:
    // forward event notifications to all listeners
    void signalNotifyListener() {
      ell_ty::iterator iter, niter;
      for(iter = ell.begin();
          iter != ell.end();
          iter = niter)
      {
        ++(niter = iter);
        (*iter)->signaled(this);
      }
    }

    // forward event resets to all listeners except the specified one
    void signalResetListener(EventListener *el = NULL) {
      ell_ty::iterator iter, niter;
      for(iter = ell.begin();
          iter != ell.end();
          iter = niter)
      {
        ++(niter = iter);
        if(*iter != el)
          (*iter)->signaled(this);
      }
    }

  public:
    // default constructor
    EventWaiter()
      {}

    // determines if this instance is active
    virtual bool isActive() const = 0;
    
    // determines if this instance is active
    operator unspecified_bool_type() const
      { return isActive() ? &this_type::addListener : NULL; }

    // identify active element; reset it if found; return it 
    virtual EventWaiter *reset(EventListener *el = NULL) = 0;

    // el must NOT be in set previously
    void addListener(EventListener *el)
      { sassert(ell.insert(el).second); }

    // el must be in set previously
    void delListener(EventListener *el)
      { sassert(ell.erase(el) == 1); }

    // notify listeners that this event is destroyed
    virtual ~EventWaiter() {
      ell_ty::iterator iter, niter;
      for(iter = ell.begin();
          iter != ell.end();
          iter = niter)
      {
        ++(niter = iter);
        (*iter)->eventDestroyed(this);
      }
    }

#ifndef NDEBUG
    virtual void dump(std::ostream &out) const
      { out << "EventWaiter(" << this << ", active: " << isActive() << ")"; }
#else
    void dump(std::ostream &out) const {}
#endif

  private:
    // registered listeners
    typedef std::set<EventListener *> ell_ty;
    ell_ty ell;
    
    // unimplemented
    EventWaiter(const this_type &);
  };

#ifndef NDEBUG
  static inline
  std::ostream &operator << (std::ostream &out, const EventWaiter &se)
    { se.dump(out); return out; }
#endif

  /**
   * simple implementation of the EventWaiter class
   */ 
  class Event: public EventWaiter {
  public:
    typedef Event this_type;
  public:
    Event(bool active = false) :
      active(active)
      {}

    // see EventWaiter
    bool isActive() const
      { return active; }

    // set event to active; first time activation will notify
    // listeners
    void notify() {
      if(!active) {
        active = true;
        signalNotifyListener();
      }
    }

    // set event to inactive; first time deactivation will notify
    // listeners (returns event upon notification; NULL otherwise)
    EventWaiter *reset(EventListener *el = NULL) {
      EventWaiter *ret = NULL;
      if(isActive()) {
        active = false;
        signalResetListener(el);
        ret = this;
      }
      return ret;
    }

#ifndef NDEBUG
    virtual void dump(std::ostream &out) const
      { out << "Event(" << this << ", active: " << isActive() << ")"; }
#endif
  private:
    // true if event is active, false otherwise
    bool active;
  };
  
  /**
   * an EventOrList is active if at least one entry is active
   */ 
  template <class T>
  class EventOrList : public EventWaiter, protected EventListener {
  public:
    typedef T EventType;
  protected:
    typedef std::list<EventType *> EventList;
    typedef typename EventList::iterator       ELIter;
    typedef typename EventList::const_iterator ELCIter;
    EventList eventList;

    // this list is a listener for each contained event, so it gets
    // notified if any event was notifed / reset, which will update
    // this list in turn
    void signaled(EventWaiter *ew) {
      // must be compatible with our list type
      EventType *e = dynamic_cast<EventType *>(ew);
      assert(e);

      // must be in list
      assert(contains(*e));
      
      //outDbg << "EventOrList::signaled(" << *e << ")" << std::endl;
      if(e->isActive()) {
        //outDbg << "e is active; active: " << (active+1) << std::endl;
        if(!active++)
          signalNotifyListener();
      }
      else {
        //outDbg << "e is not active; active: " << (active-1) << std::endl;
        assert(active);
        if(!--active)
          signalResetListener();
      }
    }

    void eventDestroyed(EventWaiter *ew) {
      //outDbg << "EventOrList::eventDestroyed(" << *e << ")" << std::endl; 
      for(ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
        if(*i != ew) {
          (*i)->delListener(this);
        }
      }
      eventList.clear();
      if(active) {
        active = 0;
        //signalResetListener();
      }
    }
  public:
    typedef EventOrList this_type;
    
    // constructors
    EventOrList() : active(0) {}
    EventOrList(EventType &e) : active(0) { *this |= e; }
    EventOrList(const EventOrList &l) : active(0) { *this |= l; }

    // see EventWaiter
    bool isActive() const
      { return active; }

    void remove(EventType &e) {
      ELIter i = find(eventList.begin(), eventList.end(), &e);
      if(i != eventList.end()) {
        eventList.erase(i);
        e.delListener(this);
        if(e.isActive()) {
          assert(active);
          if(!--active)
            signalResetListener();
        }
      }
    }
    
    // or´ing this list with event e
    this_type operator|(EventType& e)
      { return this_type(*this) |= e; }

    // or´ing this list with event e
    this_type& operator|=(EventType &e) {
      eventList.push_back(&e);
      e.addListener(this);
      if(e.isActive()) {
        if(!active++)
          signalNotifyListener();
      }
      return *this;
    }

    // or´ing this list with list l
    this_type operator|(const this_type &l)
      { return this_type(*this) |= l; }

    // or´ing this list with list l
    this_type& operator|=(const this_type& l) {
      for(ELCIter i = l.eventList.begin(); i != l.eventList.end(); ++i) {
        *this |= **i;
      }
      return *this;
    }

    EventType &getEventTrigger() {
      for(ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
        if((*i)->isActive())
          return **i;
      }
      assert(0);
    }

    EventWaiter *reset(EventListener *el = NULL) {
      EventWaiter *ret = NULL;
      if(active) {
        ret = getEventTrigger().reset(this);
        if(!ret->isActive()) {
          if(!--active)
            signalResetListener(el);
        }
      }
      return ret;
    }

    void clear() {
      for(ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
        (*i)->delListener(this);
      }
      eventList.clear();
      if(active) {
        active = 0;
        //signalResetListener();
      }
    }
    
    bool empty()
      { return eventList.empty(); }

    size_t size()
      { return eventList.size(); }

    bool contains(EventType &e)
      { return find(eventList.begin(), eventList.end(), &e) != eventList.end(); }
    
    ~EventOrList()
      { clear(); }

#ifndef NDEBUG
    virtual void dump(std::ostream &out) const {
      out << "EventOrList([";
      for(ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
        out << (i != eventList.begin() ? ", " : "") << **i;
      }
      out << "], active: " << active << ")";
    }
#endif

    private:
      size_t active;
  };

  /**
   * an EventAndList is active if all entries are active
   */ 
  template <class T>
  class EventAndList : public EventWaiter, protected EventListener {
  public:
    typedef T EventType;
  protected:
    typedef std::list<EventType *> EventList;
    typedef typename EventList::iterator       ELIter;
    typedef typename EventList::const_iterator ELCIter;
    EventList eventList;
    
    void signaled(EventWaiter *ew) {
      // must be compatible with our list type
      EventType *e = dynamic_cast<EventType *>(ew);
      assert(e);

      // must be in list
      assert(contains(*e));

      //outDbg << "EventAndList::signaled(" << *e << ")" << std::endl;
      if(e->isActive()) {
        //outDbg << "e is active; missing: " << (missing-1) << std::endl;
        assert(missing);
        if(!--missing)
          signalNotifyListener();
      }
      else {
        //outDbg << "e is not active; missing: " << (missing+1) << std::endl;
        if(!missing++)
          signalResetListener();
      }
    }

    void eventDestroyed(EventWaiter *ew) {
      //outDbg << "EventAndList::eventDestroyed(" << *ew << ")" << std::endl; 
      for(ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
        if(*i != ew)
          (*i)->delListener(this);
      }
      eventList.clear();
      if(missing) {
        missing = 0;
        //signalNotifyListener();
      }
    }

  public:
    typedef EventAndList this_type;
    
    // constructors
    EventAndList() : missing(0) {}
    EventAndList(EventType &e) : missing(0) { *this &= e; }
    EventAndList(const EventAndList &l) : missing(0) { *this &= l; }

    // see EventWaiter
    bool isActive() const
      { return !missing; }

    void remove(EventType &e) {
      ELIter i = find(eventList.begin(), eventList.end(), &e);
      if(i != eventList.end()) {
        eventList.erase(i);
        e.delListener(this);
        if(!e.isActive()) {
          assert(missing);
          if(!--missing)
            signalNotifyListener();
        }
      }
    }
    
    // and´ing this list with event e
    this_type operator&(EventType &e)
      { return this_type(*this) &= e; }

    // and´ing this list with event e
    this_type& operator&=(EventType &e) {
      eventList.push_back(&e);
      e.addListener(this);
      if(!e.isActive()) {
        if(!missing++)
          signalResetListener();
      }
      return *this;
    }
    
    // and´ing this list with list l
    this_type operator&(const this_type &l)
      { return this_type(*this) &= l; }

    // and´ing this list with list l
    this_type& operator&=(const this_type& l) {
      for(ELCIter i = l.eventList.begin(); i != l.eventList.end(); ++i) {
        *this &= **i;
      }
      return *this;
    }
    
    EventType *reset(EventListener *el = NULL) {
      EventWaiter *ret = NULL;
      if(!missing) {
        for(ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
          (*i)->reset(this);
          if(!(*i)->isActive())
            ++missing;
        } 
        if(missing)
          signalResetListener(el);
        ret = this;
      }
      return ret;
    }

    void clear() {
      for(ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
        (*i)->delListener(this);
      }
      eventList.clear();
      if(missing) {
        missing = 0;
        //signalNotifyListener();
      }
    }
    
    bool empty()
      { return eventList.empty(); }

    size_t size()
      { return eventList.size(); }

    bool contains(EventType &e)
      { return find(eventList.begin(), eventList.end(), &e) != eventList.end(); }
    
    ~EventAndList()
      { clear(); }
    
#ifndef NDEBUG
    virtual void dump(std::ostream &out) const {
      out << "EventAndList([";
      for(ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
        out << (i != eventList.begin() ? ", " : "") << **i;
      }
      out << "], missing: " << missing << ")";
    }
#endif

  private:
    size_t missing;
  };

  /**
   * contains any EventWaiter which can be exchanged
   * (trimmed down EventOrList)
   */
  struct VariantEventWaiter : public EventWaiter, protected EventListener {
  public:
    // construct object with no EventWaiter
    VariantEventWaiter() : ew(0), active(false)
    {}

    // exchange EventWaiter
    void set(EventWaiter* e) {
      if(ew) ew->delListener(this);
      ew = e;
      if(ew) ew->addListener(this);
      update();
    }

    // destructor
    ~VariantEventWaiter()
    { if(ew) ew->delListener(this); }

#ifndef NDEBUG
    virtual void dump(std::ostream &out) const
    { out << "VariantEventWaiter([" << ew << "], active: " << active << ")"; }
#endif

  protected:
    // see EventWaiter
    bool isActive() const
    { return active; }

    // see EventWaiter
    EventWaiter* reset(EventListener* el = 0) {
      EventWaiter* ret = 0;
      if(active) {
        ret = ew->reset(this);
        if(!ew->isActive()) {
          active = false;
          signalResetListener(el);
        }
      }
      return ret;
    }

    // see EventListener
    void signaled(EventWaiter *e)
    { assert(e == ew); update(); }

    // see EventListener
    void eventDestroyed(EventWaiter *e)
    { assert(e == ew); ew = 0; active = false; }

  private:
    EventWaiter* ew;
    bool active;

    void update() {
      if(ew && ew->isActive()) {
        if(!active) {
          active = true;
          signalNotifyListener();
        }
      }
      else {
        if(active) {
          active = false;
          signalResetListener();
        }
      }
    }
  };

  /**
   * @brief Wrapper EventWaiter -> sc_event
   */
  class SCEventWrapper: protected EventListener {
  public:
    /// @brief Constructor
    SCEventWrapper(EventWaiter& e) : _e(e) {
      _e.addListener(this);
      // if a was already active _before_ we added us as listener,
      // we won't receive a "signaled" event. Defect???
      update();
    }

    /// @brief Virtual Destructor for derived classes
    virtual ~SCEventWrapper()
    { _e.delListener(this); }

    /// @brief Returns sc_event (use "wait(x.getSCEvent())" !!!)
    sc_event& getSCEvent()
    { update(); return scev; }

    /// @brief Returns sc_event (use "wait(x.getSCEvent())" !!!)
    const sc_event& getSCEvent() const
    { update(); return scev; }

  protected:
    /// @brief see EventListener
    void signaled(EventWaiter* e) {
      assert(e == &_e);
      update();
    }

    /// @brief see EventListener
    void eventDestroyed(EventWaiter* e)
    {}

  private:
    void update() const {
      // we won't get notified if already notifed, but sc_event
      // resets in next delta cycle. so we have a problem...
      // Also, we can't use immediate notification (because
      // wait() will not return)
      if(_e.isActive())
        scev.notify(SC_ZERO_TIME);
      else
        scev.cancel(); // allowed but should not happen??
    }

    EventWaiter& _e;
    mutable sc_event scev;
  };

  inline
  EventOrList<EventWaiter> operator|(EventWaiter &a, EventWaiter &b)
    { return EventOrList<EventWaiter>(a) |= b; }

  inline
  EventAndList<EventWaiter> operator&(EventWaiter &a, EventWaiter &b)
    { return EventAndList<EventWaiter>(a) &= b; }

  static inline
  void notify(Event& e)
    { return e.notify(); }
  
  static inline
  EventWaiter *reset(EventWaiter& e)
    { return e.reset(); }
  
  static inline
  void wait(EventWaiter &e) {
    if (!e) {
      struct _: public EventListener {
        sc_event sce;
        void signaled(EventWaiter *_e) {
          sce.notify();
          return;// false;
        }
        void eventDestroyed(EventWaiter *_e) {
          // sce.notify();
        }
        virtual ~_() {}
      } w;
      e.addListener(&w);
      wait(w.sce);
      e.delListener(&w);
    }
  }

} } // CoSupport::SystemC

//#undef outDbg

#endif // _INCLUDED_COSUPPORT_SYSTEMC_SUPPORT_HPP
