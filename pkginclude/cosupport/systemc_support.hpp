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

#ifndef _INCLUDED_SYSTEMC_SUPPORT_HPP
#define _INCLUDED_SYSTEMC_SUPPORT_HPP

#include <systemc.h>

#include <iostream>

#include <vector>
#include <set>

#include "sassert.h"

//#include <intrusive_copyonwrite_ptr.hpp>

namespace CoSupport { namespace SystemC {

  class EventWaiter;

  // Event handling
 
  /**
   *
   */ 
  class EventListener {
  public:
    friend class EventWaiter;
  protected:
    /**
     * Tell this listener about an event changing in EventWaiter e.
     * E.g. the waiter was reseted or notified.
     */
    virtual void signaled(EventWaiter *e) = 0;

    /**
     * The lifetime of the given EventWaiter is over.
     */
    virtual void eventDestroyed(EventWaiter *e) = 0;
  };

  template <class T> class EventOrList;
  template <class T> class EventAndList;
  
  /**
   *
   */ 
  class EventWaiter {
  public:
    typedef EventWaiter         this_type;
    typedef void (this_type::*unspecified_bool_type)(EventListener *);
  private:
    typedef std::set<EventListener *>  ell_ty;
    
    ell_ty  ell;
  protected:
    int     missing;

    /**
     * Forward event notifications to all listners.
     */
    void signalNotifyListener() {
      // forward notification only once, when missing is set to 0
      // if missing < 0 then the listeners must be notified allready
      if (missing == 0) {
        ell_ty::iterator iter, niter;
        
        for ( iter = ell.begin();
              iter != ell.end();
              iter = niter ) {
          ++(niter = iter);
          (*iter)->signaled(this);
        }
      }
    }

    /**
     * Forward event resets to all listners.
     */
    void signalResetListener(EventListener *el) {
      ell_ty::iterator iter, niter;
      
      for ( iter = ell.begin();
            iter != ell.end();
            iter = niter) {
        ++(niter = iter);
        if (*iter != el)
          (*iter)->signaled(this);
      }
    }
  public:
    EventWaiter(bool startNotified = false)
      :
        missing(startNotified ? 0 : 1)
      {}

    virtual EventWaiter *reset(EventListener *el = NULL) {
      if (!missing) {
        missing = 1;
        signalResetListener(el);
        return this;
      } else
        return NULL;
    }

    //
    void addListener(EventListener *el)
    {
      sassert(ell.insert(el).second); //assert "el" was NOT in set previously
    }
    void delListener(EventListener *el)
      { sassert(ell.erase(el) == 1); }

    operator unspecified_bool_type() const
      { return missing <= 0 ? &this_type::addListener : NULL; }

    class EventOrList<this_type>  operator | (EventWaiter &e);
    class EventAndList<this_type> operator & (EventWaiter &e);

#ifndef NDEBUG
    virtual
    void dump(std::ostream &out) const {
      out << "EventWaiter(" << this << ", missing: " << missing << ")";
    }
#else
    void dump(std::ostream &out) const {}
#endif

    virtual ~EventWaiter() {
      for ( ell_ty::iterator iter = ell.begin();
            iter != ell.end();
            ++iter )
        (*iter)->eventDestroyed(this);
    }
  private:
    // disable
    EventWaiter( const this_type & );
  };

#ifndef NDEBUG
  static inline
  std::ostream &operator << (std::ostream &out, const EventWaiter &se) {
    se.dump(out); return out;
  }
#endif

  /**
   *
   */ 
  class Event: public EventWaiter {
  public:
    typedef Event this_type;
  public:
    Event(bool startNotified = false)
      : EventWaiter(startNotified) {}

    void notify() {
      assert(missing == 1 || missing == 0);
      if(missing) {  // only update listener at first call of notify
        missing = 0; // toggle to notified
        signalNotifyListener();
      }
    }

#ifndef NDEBUG
    virtual
    void dump(std::ostream &out) const {
      out << "Event(" << this << ", missing: " << missing << ")";
    }
#endif

    void reset() {
      EventWaiter::reset();
    }
  };
  
  /**
   *
   */ 
  template <class T>
  class EventOrList
  : public EventWaiter,
    protected EventListener {
  public:
    typedef T EventType;
  protected:
    typedef std::vector<EventType *> EventList;
    
    EventList    eventList;
    EventType   *eventTrigger;
    
    void signaled(EventWaiter *e) {
      assert( e && "/* e must be notified */" );
      assert( find(eventList.begin(), eventList.end(), e) != eventList.end() );
      assert( dynamic_cast<EventType *>(e) );
      if (*e) {
        if (!eventTrigger)
          eventTrigger = reinterpret_cast<EventType *>(e);
        --missing;
        signalNotifyListener();
      } else {
        if (eventTrigger == e)
          eventTrigger = NULL;
        if (++missing == 1){
          signalResetListener(NULL);
        }else if(missing > 1){
          // prevent from overflow
          // an OR list at maximum misses one event
          missing = 1;
        }
      }
      // std::cout << "EventOrList::signaled: missing == " << missing << std::endl;
    }

    void eventDestroyed(EventWaiter *e) {
      for ( typename EventList::iterator iter = eventList.begin();
            iter != eventList.end();
            ++iter )
        if (*iter != e)
          (*iter)->delListener(this);
      missing = 1;
      eventList.clear();
    }
  public:
    typedef EventOrList this_type;
    
    EventOrList()
      : EventWaiter(false), eventTrigger(NULL) {
    }
    EventOrList(EventType &e )
      : EventWaiter(false), eventTrigger(NULL) {
      *this |= e;
    }
    EventOrList(const EventOrList &el)
      : EventWaiter(false), eventTrigger(NULL) {
      for ( typename EventList::const_iterator iter = el.eventList.begin();
            iter != el.eventList.end();
            ++iter )
        *this |= **iter;
    }
    
    void remove(EventType &e) {
      typename EventList::iterator iter = find(
        eventList.begin(), eventList.end(), &e);
      if (iter != eventList.end()) {
        eventList.erase(iter); e.delListener(this);
        if (&e == eventTrigger)
          eventTrigger = NULL;
        if (e){
          ++missing;
          if(missing > 0) signalResetListener(NULL);
        }
      }
    }
    
    // or´ing this event_or_lists with event e
    this_type operator | (EventType &e)
      { return this_type(*this) |= e; }

    // or´ing this event_or_lists with event e
    this_type &operator |= (EventType &e) {
      eventList.push_back(&e);
      e.addListener(this);
      if(e){
        --missing; // added another activating event
        signalNotifyListener(); // update event list
      }
      return *this;
    }

    // or´ing this event_or_lists with event_or_list eol
    this_type operator | (this_type &eol)
      { return this_type(*this) |= eol; }

    // or´ing this event_or_lists with event_or_list eol
    this_type &operator |= (this_type &eol) {
      for( typename EventList::iterator iter = eol.eventList.begin();
	   iter != eol.eventList.end(); iter++ ) {
        *this |= **iter;
      }
      return *this;
    }

    EventType &getEventTrigger() {
      for ( typename EventList::iterator iter = eventList.begin();
            iter != eventList.end() && !eventTrigger;
            ++iter )
        if ( **iter )
          eventTrigger = *iter;
      assert((eventTrigger != NULL) && (missing <= 0) && *eventTrigger);
      return *eventTrigger;
    }

    EventWaiter *reset(EventListener *el = NULL) {
      EventWaiter *retval = NULL;
      
      if (missing <= 0) {
        retval = getEventTrigger().reset(this);
        if (!getEventTrigger()) {
          ++missing; eventTrigger = NULL;
        }
        // just toggled from enabled to disabled
        if (missing > 0)
          signalResetListener(el);
      }
      return retval;
    }
    void clear() {
      for ( typename EventList::iterator iter = eventList.begin();
            iter != eventList.end();
            ++iter )
        (*iter)->delListener(this);
      missing = 1;
      eventList.clear();
    }
    
    bool empty(){
      return eventList.empty();
    }

    size_t size(){
      return eventList.size();
    }

    bool contains(EventType &e){
      typename EventList::iterator iter = find(eventList.begin(), eventList.end(), &e);
      return ( iter != eventList.end());
    }

#ifndef NDEBUG
    virtual
    void dump(std::ostream &out) const {
      out << "EventOrList([";
      for ( typename EventList::const_iterator iter = eventList.begin();
            iter != eventList.end();
            ++iter )
        out << (iter != eventList.begin() ? ", " : "") << **iter;
      out << "], missing: " << missing << ")";
    }

    /*
"#include <cosupport/filter_ostream.hpp>"

    virtual
    void dump(std::ostream &out) const {
      out << "EventOrList(missing: " << missing << ", [\n";
      out << Indent::Up;
      for ( typename EventList::const_iterator iter = eventList.begin();
            iter != eventList.end();
            ++iter )
        out << (iter != eventList.begin() ? ",\n" : "") << **iter;
      out <<Indent::Down;
      out << "\n])";
    }
    */
#endif

    ~EventOrList()
      { clear(); }
  };

  /**
   *
   */ 
  template <class T>
  class EventAndList
  : public EventWaiter,
    protected EventListener {
  public:
    typedef T EventType;
  protected:
    typedef std::vector<EventType *> EventList;
    
    EventList  eventList;
    
    void signaled(EventWaiter *e) {
      assert(
        e &&
        find(eventList.begin(), eventList.end(), e) != eventList.end());
      if (*e) {
        // to prevent missing from "underflow"
        // only notify if this list was not active yet
        if(!*this)
          --missing;
        signalNotifyListener();
        
      } else {
        if (++missing == 1)
          signalResetListener(NULL);
      }
      // std::cout << "EventAndList::signaled: missing == " << missing << std::endl;
    }

    void eventDestroyed(EventWaiter *e) {
      for ( typename EventList::iterator iter = eventList.begin();
            iter != eventList.end();
            ++iter )
        if (*iter != e)
          (*iter)->delListener(this);
      missing = 0;
      eventList.clear();
    }
  public:
    typedef EventAndList this_type;
    
    EventAndList()
      : EventWaiter(true) {
    }
    EventAndList(EventType &e)
      : EventWaiter(true)  {
      *this &= e;
    }
    EventAndList(const EventAndList &el)
      : EventWaiter(true)  {
      for ( typename EventList::const_iterator iter = el.eventList.begin();
            iter != el.eventList.end();
            ++iter )
        *this &= **iter;
    }

    void remove(EventType &e) {
      typename EventList::iterator iter = find(
        eventList.begin(), eventList.end(), &e);
      if (iter != eventList.end()) {
        eventList.erase(iter); e.delListener(this);
        if (!e && missing > 0){ //prevent missing from underflow
          --missing;
          signalNotifyListener();
        }
      }
    }
    
    // and´ing this event_and_lists with event e
    this_type operator & (EventType &e)
      { return this_type(*this) &= e; }

    // and´ing this event_and_lists with event e
    this_type &operator &= (EventType &e) {
      eventList.push_back(&e);
      e.addListener(this);
      if(!e){
        ++missing; // e is not signaled -> another event to wait for
        signalResetListener(NULL); // update event list
      }
      return *this;
    }
    
    EventType *reset(EventListener *el = NULL) {
      if (missing <= 0) {
        for ( typename EventList::iterator iter = eventList.begin();
              iter != eventList.end();
              ++iter ) {
          (*iter)->reset(this);
          if (!**iter)
            ++missing;
        }
        // just toggled from enabled to disabled
        if (missing > 0)
          signalResetListener(el);
        return this;
      } else
        return NULL;
    }
    void clear() {
      for ( typename EventList::iterator iter = eventList.begin();
            iter != eventList.end();
            ++iter )
        (*iter)->delListener(this);
      missing = 0;
      eventList.clear();
    }
    
#ifndef NDEBUG
    virtual
    void dump(std::ostream &out) const {
      out << "EventAndList([";
      for ( typename EventList::const_iterator iter = eventList.begin();
            iter != eventList.end();
            ++iter )
        out << (iter != eventList.begin() ? ", " : "") << **iter;
      out << "], missing: " << missing << ")";
    }

    /*
    virtual
    void dump(std::ostream &out) const {
      out << "EventAndList(missing: " << missing << ", [\n";
      out << Indent::Up;
      for ( typename EventList::const_iterator iter = eventList.begin();
            iter != eventList.end();
            ++iter )
        out << (iter != eventList.begin() ? ",\n" : "") << **iter;
      out << Indent::Down;
      out << "\n])";
    }
    */
#endif

    ~EventAndList()
      { clear(); }
  };

  inline
  EventOrList<EventWaiter>  EventWaiter::operator | (EventWaiter &e)
    { return EventOrList<EventWaiter>(*this) |= e; }

  inline
  EventAndList<EventWaiter> EventWaiter::operator & (EventWaiter &e)
    { return EventAndList<EventWaiter>(*this) &= e; }

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

#endif // _INCLUDED_SYSTEMC_SUPPORT_HPP
