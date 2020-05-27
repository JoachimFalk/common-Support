// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2010 FAU -- Martin Streubuehr <martin.streubuehr@fau.de>
 *   2014 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_SYSTEMC_SUPPORT_HPP
#define _INCLUDED_COSUPPORT_SYSTEMC_SUPPORT_HPP

#include <systemc>

#include <iostream>

#include <list>
#include <set>
#include <algorithm>

#include <CoSupport/cosupport_config.h>

#include <CoSupport/sassert.h>
#include <CoSupport/compatibility-glue/nullptr.h>
#include <CoSupport/SmartPtr/RefCountObject.hpp>

#include <boost/functional/hash.hpp>
#include <boost/intrusive_ptr.hpp>

#include "export_config.h"

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

//#define DEBUG_COSUPPORT_SYSTEMC_SUPPORT

namespace CoSupport { namespace SystemC {

/**
 * forward declarations
 */
class COSUPPORT_SYSTEMC_API
EventWaiter;

/**
 * base class for all classes which must be notified upon
 * activation / deactivation of some event
 */ 
class COSUPPORT_SYSTEMC_API
EventListener {
private:
  // must call signaled / eventDestroyed
  friend class EventWaiter;

protected:
  // Tell this listener about an event changing in EventWaiter e
  // (e.g. the waiter was reseted or notified; check e->isActive().
  // Must return true if this EventListener should be removed from
  // the EventWaiter e, false otherwise.
  virtual bool signaled(EventWaiter *e) = 0;

  // The lifetime of the given EventWaiter is over
  virtual void eventDestroyed(EventWaiter *e) = 0;

  // May be called when Event is active
  virtual void renotified(EventWaiter *e) {}

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
 *      notifyListener()  resetListener()
 */ 
class COSUPPORT_SYSTEMC_API
EventWaiter {
public:
  typedef EventWaiter this_type;
  typedef void (this_type::*unspecified_bool_type)(EventListener *, bool);
protected:
  // forward event notifications to all listeners
  void notifyListener() {
#ifdef COSUPPORT_ENABLE_DEBUG
    usedCounter++;
#endif //COSUPPORT_ENABLE_DEBUG
    for (ell_ty::iterator iter = ell.begin(); iter != ell.end();) {
      if ((*iter)->signaled(this))
        iter = ell.erase(iter);
      else
        iter++;
    }
#ifdef COSUPPORT_ENABLE_DEBUG
    usedCounter--;
#endif //COSUPPORT_ENABLE_DEBUG
  }

  // forward event resets to all listeners except the specified one
  void resetListener(EventListener *el = nullptr) {
#ifdef COSUPPORT_ENABLE_DEBUG
    usedCounter++;
#endif //COSUPPORT_ENABLE_DEBUG
    for (ell_ty::iterator iter = ell.begin(); iter != ell.end();) {
      if (*iter != el) {
        if ((*iter)->signaled(this))
          iter = ell.erase(iter);
        else
          iter++;
      } else
        iter++;
    }
#ifdef COSUPPORT_ENABLE_DEBUG
    usedCounter--;
#endif //COSUPPORT_ENABLE_DEBUG
  }

public:
  // default constructor
  EventWaiter()
#ifdef COSUPPORT_ENABLE_DEBUG
    : usedCounter(0)
#endif //COSUPPORT_ENABLE_DEBUG
    {}
  // determines if this instance is active
  virtual bool isActive() const = 0;
  
  // determines if this instance is active
  operator unspecified_bool_type() const
    { return isActive() ? &this_type::addListener : nullptr; }

  // identify active element; reset it if found; return it 
  virtual EventWaiter *reset(EventListener *el = nullptr) = 0;

  // el must NOT be in set previously
  void addListener(EventListener *el, bool laxly = false) {
#ifdef COSUPPORT_ENABLE_DEBUG
    assert(usedCounter == 0);
#endif //COSUPPORT_ENABLE_DEBUG
    sassert(ell.insert(el).second || laxly);
  }

  // el must be in set previously
  void delListener(EventListener *el, bool laxly = false) {
#ifdef COSUPPORT_ENABLE_DEBUG
    assert(usedCounter == 0);
#endif //COSUPPORT_ENABLE_DEBUG
    sassert(ell.erase(el) == 1 || laxly);
  }
  
  // notify listeners that this event is destroyed
  virtual ~EventWaiter() {
#ifdef COSUPPORT_ENABLE_DEBUG
    assert(usedCounter == 0);
#endif //COSUPPORT_ENABLE_DEBUG
    for (EventListener *e : ell)
      e->eventDestroyed(this);
  }

  void renotifyListener() {
    for (EventListener *e : ell)
      e->renotified(this);
  }

  virtual void dump(std::ostream &out) const
    { out << "EventWaiter(" << this << ", active: " << isActive() << ")"; }
private:
  // registered listeners
  typedef std::set<EventListener *> ell_ty;
  ell_ty ell;
#ifdef COSUPPORT_ENABLE_DEBUG
  // In some strange cases, an EventWaiter is deleted, but code of the
  // object to be deleted is still executed (e.g. signalNotifyListener).
  // This behavior is undefined and causes errors on some platforms, e.g.,
  // Microsoft Visual Studio.
  unsigned int usedCounter;
#endif //COSUPPORT_ENABLE_DEBUG

  // unimplemented
  EventWaiter(const this_type &);
};

COSUPPORT_SYSTEMC_API
inline
std::ostream &operator << (std::ostream &out, const EventWaiter &se)
  { se.dump(out); return out; }

/**
 * simple implementation of the EventWaiter class
 */ 
class COSUPPORT_SYSTEMC_API
Event: public EventWaiter {
public:
  typedef Event this_type;
public:
  Event(bool active = false) :
    active(active) {}

  Event(const Event& e) :
    active(e.active) {}

  // see EventWaiter
  bool isActive() const
    { return active; }

  // set event to active; first time activation will notify
  // listeners
  void notify() {
    if(!active) {
      active = true;
      notifyListener();
    }
  }

  // set event to inactive; first time deactivation will notify
  // listeners (returns event upon notification; nullptr otherwise)
  EventWaiter *reset(EventListener *el = nullptr) {
    EventWaiter *ret = nullptr;
    if(isActive()) {
      active = false;
      resetListener(el);
      ret = this;
    }
    return ret;
  }

  virtual void dump(std::ostream &out) const
    { out << "Event(" << this << ", active: " << isActive() << ")"; }
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
  typedef EventType* ELEntry;
  typedef std::set<ELEntry> EventList;
  typedef typename EventList::iterator       ELIter;
  typedef typename EventList::const_iterator ELCIter;
  EventList eventList;

  // this list is a listener for each contained event, so it gets
  // notified if any event was notified / reset, which will update
  // this list in turn
  bool signaled(EventWaiter *ew) {
    // must be compatible with our list type
    EventType *e = dynamic_cast<EventType *>(ew);
    assert(e);

    // must be in list
    assert(contains(*e));
    
    //outDbg << "EventOrList::signaled(" << *e << ")" << std::endl;
    if(e->isActive()) {
      
      cache = e;

      //outDbg << "e is active; active: " << (active+1) << std::endl;
      if(!active++)
        notifyListener();
    }
    else {
      //outDbg << "e is not active; active: " << (active-1) << std::endl;
      assert(active);
      if(!--active)
        resetListener();
    }
    return false;
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
      //resetListener();
    }
    cache = 0;
  }
public:
  typedef EventOrList this_type;
  
  // constructors
  EventOrList()
    : active(0), cache(0) {}
  EventOrList(EventType &e)
    : active(0), cache(0) { *this |= e; }
  EventOrList(const EventOrList &l)
    : active(0), cache(0) { *this |= l; }

  // see EventWaiter
  bool isActive() const
    { return active; }

  void remove(EventType &e) {
    if(eventList.erase(&e)) {
      if(cache == &e)
        cache = 0;
      e.delListener(this);
      if(e.isActive()) {
        assert(active);
        if(!--active)
          resetListener();
      }
    }
  }

  void insert(EventType &e) {
    if(eventList.insert(&e).second) {
      e.addListener(this);
      if(e.isActive()) {
        
        cache = &e;
        
        if(!active++)
          notifyListener();
      }
    }
  }

  // or´ing this list with event e
  this_type operator|(EventType& e)
    { return this_type(*this) |= e; }

  // or´ing this list with event e
  this_type& operator|=(EventType &e)
    { insert(e); return *this; }

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
    if(cache && cache->isActive()) {
      return *cache;
    }
    for(ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
      if((*i)->isActive())
        return **i;
    }
    assert(0);
    return *((EventType*)(nullptr));
  }

  EventWaiter *reset(EventListener *el = nullptr) {
    EventWaiter *ret = nullptr;
    if(active) {
      ret = getEventTrigger().reset(this);
      if(!ret->isActive()) {
        if(!--active)
          resetListener(el);
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
      //resetListener();
    }
    cache = 0;
  }
  
  bool empty() const
    { return eventList.empty(); }

  size_t size() const
    { return eventList.size(); }

  bool contains(EventType &e) const
    { return eventList.count(&e); }

  ~EventOrList()
    { clear(); }

  virtual void dump(std::ostream &out) const {
    out << "EventOrList([";
    for(ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
      out << (i != eventList.begin() ? ", " : "") << **i;
    }
    out << "], active: " << active << ")";
  }

  private:
    size_t active;
    EventType* cache;
};

/**
 * an EventAndList is active if all entries are active
 */ 
template <class T>
class EventAndList : public EventWaiter, protected EventListener {
public:
  typedef T EventType;
protected:
  typedef EventType                         *ELEntry;
  typedef std::set<ELEntry>                  EventList;
  typedef typename EventList::iterator       ELIter;
  typedef typename EventList::const_iterator ELCIter;

  EventList                                  eventList;
  size_t                                     missing;
  bool                                       lazy;

  bool signaled(EventWaiter *ew) {
    // must be compatible with our list type and must be in list
    assert(dynamic_cast<EventType *>(ew) &&
      contains(*static_cast<EventType *>(ew)));
#ifdef DEBUG_COSUPPORT_SYSTEMC_SUPPORT
    std::cerr << "[" << this << "] EventAndList signaled by: [" << ew << "] " << *ew << std::endl;
#endif // DEBUG_COSUPPORT_SYSTEMC_SUPPORT      
    if (ew->isActive()) {
#ifdef DEBUG_COSUPPORT_SYSTEMC_SUPPORT
      std::cerr << "  ew is active; missing: " << (missing-1) << std::endl;
#endif // DEBUG_COSUPPORT_SYSTEMC_SUPPORT      
      oneLessMissing(ew);
      return false;
    } else {
      if (missing == 0) { // 0 -> 1
        assert(!lazy); missing = 1;
#ifdef DEBUG_COSUPPORT_SYSTEMC_SUPPORT
        std::cerr << "  ew is not active; missing: " << missing << std::endl;
#endif // DEBUG_COSUPPORT_SYSTEMC_SUPPORT      
        resetListener();
        return false;
      } else {
#ifdef DEBUG_COSUPPORT_SYSTEMC_SUPPORT
        std::cerr << "  ew is not active; going lazy; missing: " << missing << std::endl;
#endif // DEBUG_COSUPPORT_SYSTEMC_SUPPORT      
        lazy = true;
        return true; // Still blocked on something else.
      }
    }
    // Not reached
  }

  void eventDestroyed(EventWaiter *ew) {
    //outDbg << "EventAndList::eventDestroyed(" << *ew << ")" << std::endl;
    for (ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
      if (*i != ew)
        (*i)->delListener(this, true);
    }
    eventList.clear();
    missing = 0;
  }

  void renotified(EventWaiter *e) {
    if(!missing)
      renotifyListener();
  }
private:
  void oneLessMissing(EventWaiter *ew = nullptr) {
    if (--missing == 0) {
      if (lazy) {
        // 1 -> 0 => check with deregistered EventWaiters if they are active
        for (ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
          if (*i != ew)
            (*i)->addListener(this, true);
          if (!(*i)->isActive())
            ++missing;
        }
        lazy = false;
#ifdef DEBUG_COSUPPORT_SYSTEMC_SUPPORT
        std::cerr << "  was lazy; missing: " << missing << std::endl;
#endif // DEBUG_COSUPPORT_SYSTEMC_SUPPORT      
        if (missing != 0)
          return;
      }
      notifyListener();
    }
  }
public:
  typedef EventAndList this_type;

  // constructors
  EventAndList() : missing(0), lazy(false) {}
  EventAndList(EventType &e) : missing(0), lazy(false) { *this &= e; }
  EventAndList(const EventAndList &l) : missing(0), lazy(false) { *this &= l; }

  // see EventWaiter
  bool isActive() const
    { return !missing; }

  void remove(EventType &e) {
    if (eventList.erase(&e)) {
      e.delListener(this, true);
      if (!e.isActive())
        oneLessMissing();
    }
  }

  void insert(EventType &e) {
    if (eventList.insert(&e).second) {
      e.addListener(this);
      if (!e.isActive() && missing++ == 0)
        resetListener();
    }
  }

  // and´ing this list with event e
  this_type operator&(EventType &e)
    { return this_type(*this) &= e; }

  // and´ing this list with event e
  this_type& operator&=(EventType &e)
    { insert(e); return *this; }

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

  EventWaiter *reset(EventListener *el = nullptr) {
    EventWaiter *ret = nullptr;
    if (!missing) {
      for(ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
        assert((*i)->isActive());
        (*i)->reset(this);
        if (!(*i)->isActive())
          ++missing;
      }
      if (missing)
        resetListener(el);
      ret = this;
    }
    return ret;
  }

  void clear() {
    for(ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
      (*i)->delListener(this, true);
    }
    eventList.clear();
    missing = 0;
  }

  bool operator==(const this_type& l) const
    { return eventList == l.eventList; }

  bool operator<(const this_type& l) const
    { return eventList < l.eventList; }

  bool empty() const
    { return eventList.empty(); }

  size_t size() const
    { return eventList.size(); }

  bool contains(EventType &e) const
    { return eventList.count(&e); }

  size_t hash_value() const
    { return boost::hash_value(eventList); }

  ~EventAndList()
    { clear(); }

  virtual void dump(std::ostream &out) const {
    out << "EventAndList([";
    for(ELCIter i = eventList.begin(); i != eventList.end(); ++i) {
      out << (i != eventList.begin() ? ", " : "") << **i;
    }
    out << "], missing: " << missing << ", lazy: " << lazy << ")";
  }
};

template<class T>
size_t hash_value(const EventAndList<T>& l)
  { return l.hash_value(); }

/**
 * contains any EventWaiter which can be exchanged
 * (trimmed down EventOrList)
 */
struct COSUPPORT_SYSTEMC_API
VariantEventWaiter : public EventWaiter, protected EventListener {
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

  virtual void dump(std::ostream &out) const
    { out << "VariantEventWaiter([" << ew << "], active: " << active << ")"; }
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
        resetListener(el);
      }
    }
    return ret;
  }

  // see EventListener
  bool signaled(EventWaiter *e)
  { assert(e == ew); update(); return false; }

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
        notifyListener();
      }
    }
    else {
      if(active) {
        active = false;
        resetListener();
      }
    }
  }
};

/**
 * @brief Wrapper EventWaiter -> sc_core::sc_event
 */
class COSUPPORT_SYSTEMC_API
SCEventWrapper: protected EventListener {
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

  /// @brief Returns sc_core::sc_event (use "wait(x.getSCEvent())" !!!)
  sc_core::sc_event& getSCEvent()
  { update(); return scev; }

  /// @brief Returns sc_core::sc_event (use "wait(x.getSCEvent())" !!!)
  const sc_core::sc_event& getSCEvent() const
  { update(); return scev; }

protected:
  /// @brief see EventListener
  bool signaled(EventWaiter* e) {
    assert(e == &_e);
    update();
    return false;
  }

  /// @brief see EventListener
  void eventDestroyed(EventWaiter* e)
  {}

private:
  void update() const {
    // we won't get notified if already notifed, but sc_core::sc_event
    // resets in next delta cycle. so we have a problem...
    // Also, we can't use immediate notification (because
    // wait() will not return)
    if(_e.isActive())
      scev.notify(sc_core::SC_ZERO_TIME);
    else
      scev.cancel(); // allowed but should not happen??
  }

  EventWaiter& _e;
  mutable sc_core::sc_event scev;
};

COSUPPORT_SYSTEMC_API
inline
EventOrList<EventWaiter> operator|(EventWaiter &a, EventWaiter &b)
  { return EventOrList<EventWaiter>(a) |= b; }

COSUPPORT_SYSTEMC_API
inline
EventAndList<EventWaiter> operator&(EventWaiter &a, EventWaiter &b)
  { return EventAndList<EventWaiter>(a) &= b; }

COSUPPORT_SYSTEMC_API
inline
void notify(Event& e)
  { return e.notify(); }

COSUPPORT_SYSTEMC_API
inline
EventWaiter *reset(EventWaiter& e)
  { return e.reset(); }

COSUPPORT_SYSTEMC_API
inline
void wait(EventWaiter &e) {
  if (!e) {
    struct _: public EventListener {
      sc_core::sc_event sce;
#ifndef NDEBUG
      EventWaiter &e;
#endif //NDEBUG
      _(EventWaiter &e)
#ifndef NDEBUG
        : e(e)
#endif //NDEBUG
      {
        e.addListener(this);
      }
      bool signaled(EventWaiter *_e) {
        assert(_e == &e);
        assert(_e->isActive());
        sce.notify();
        return true;
      }
      void eventDestroyed(EventWaiter *_e) {
        // waiting for destroyed events will hang forever
      }
      virtual ~_() {}
    } w(e);
    wait(w.sce);
  }
}

class COSUPPORT_SYSTEMC_API
RefCountEvent
: public CoSupport::SmartPtr::RefCountObject, public Event {
public:
  typedef RefCountEvent this_type;
public:
  RefCountEvent(bool startNotified = false)
    : Event(startNotified) {}
};

typedef boost::intrusive_ptr<RefCountEvent> RefCountEventPtr;

} } // CoSupport::SystemC

//#undef outDbg

#endif // _INCLUDED_COSUPPORT_SYSTEMC_SUPPORT_HPP
