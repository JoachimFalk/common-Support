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

  // Event handling
 
  class EventListener {
  public:
    friend class EventWaiter;
  protected:
    // return true if event was handled
    virtual bool signaled(EventWaiter *e) = 0;
    virtual void eventDestroyed(EventWaiter *e) = 0;
  };

  template <class T> class EventOrList;
  template <class T> class EventAndList;
  
  class EventWaiter {
  public:
    typedef EventWaiter         this_type;
    typedef void (this_type::*unspecified_bool_type)(EventListener *);
  private:
    typedef std::set<EventListener *>  ell_ty;
    
    ell_ty  ell;
#ifndef NDEBUG
    bool    notifyRun;
#endif
  protected:
    int     missing;

    bool signalNotifyListener() {
      if (--missing == 0) {
#ifndef NDEBUG
        assert(!notifyRun); notifyRun = true;
#endif
        ell_ty::iterator iter, niter;
        
        for ( iter = ell.begin();
              iter != ell.end();
              iter = niter ) {
          ++(niter = iter);
          if ((*iter)->signaled(this)) {
            // event handled => therefore one missing again
            assert(missing == 0); missing = 1;
            while ( iter != ell.begin() )
              sassert(!(*iter)->signaled(this));
          }
        }
#ifndef NDEBUG
        notifyRun = false;
#endif
        return missing == 1;
      } else
        return false;
    }
    void signalResetListener(EventListener *el) {
#ifndef NDEBUG
      assert(!notifyRun); notifyRun = true;
#endif
      ell_ty::iterator iter, niter;
      
      for ( iter = ell.begin();
            iter != ell.end();
            iter = niter) {
        ++(niter = iter);
        if (*iter != el)
          sassert(!(*iter)->signaled(this));
      }
#ifndef NDEBUG
      notifyRun = false;
#endif
    }
  public:
    EventWaiter(bool startNotified = false)
      :
#ifndef NDEBUG
        notifyRun(false),
#endif
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

    void addListener(EventListener *el)
      { sassert(ell.insert(el).second); }
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

  class Event: public EventWaiter {
  public:
    typedef Event this_type;
  public:
    Event(bool startNotified = false)
      : EventWaiter(startNotified) {}

    void notify() {
      assert(missing == 1 || missing == 0);
      if (missing)
        signalNotifyListener();
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
    
    bool signaled(EventWaiter *e) {
      bool retval;
      
      assert( e && "/* e must be notified */" );
      assert( find(eventList.begin(), eventList.end(), e) != eventList.end() );
      assert( dynamic_cast<EventType *>(e) );
      if (*e) {
        if (!eventTrigger)
          eventTrigger = reinterpret_cast<EventType *>(e);
        retval = signalNotifyListener();
      } else {
        if (eventTrigger == e)
          eventTrigger = NULL;
        if (++missing == 1)
          signalResetListener(NULL);
        retval = false;
      }
      // std::cout << "EventOrList::signaled: missing == " << missing << std::endl;
      return retval;
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
        if (e)
          ++missing;
      }
    }
    
    this_type operator | (EventType &e)
      { return this_type(*this) |= e; }
    this_type &operator |= (EventType &e) {
      if (e) {
        --missing;
        if (!eventTrigger)
          eventTrigger = &e;
      }
      eventList.push_back(&e);
      e.addListener(this);
      return *this;
    }

    this_type operator | (this_type &ew)
      { return this_type(*this) |= ew; }
    this_type &operator |= (this_type &eol) {
      for( typename EventList::iterator iter = eol.eventList.begin();
	   iter != eol.eventList.end(); iter++ ) {
	if(**iter){
	  --missing;
	  if (!eventTrigger) eventTrigger = *iter;
	}
	eventList.push_back(*iter);
	(*iter)->addListener(this);
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
#endif

    ~EventOrList()
      { clear(); }
  };

  template <class T>
  class EventAndList
  : public EventWaiter,
    protected EventListener {
  public:
    typedef T EventType;
  protected:
    typedef std::vector<EventType *> EventList;
    
    EventList  eventList;
    
    bool signaled(EventWaiter *e) {
      bool retval;
      
      assert(
        e &&
        find(eventList.begin(), eventList.end(), e) != eventList.end());
      if (*e) {
        retval = signalNotifyListener();
      } else {
        if (++missing == 1)
          signalResetListener(NULL);
        retval = false;
      }
      // std::cout << "EventAndList::signaled: missing == " << missing << std::endl;
      return retval;
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
        if (!e)
          --missing;
      }
    }
    
    this_type operator & (EventType &e)
      { return this_type(*this) &= e; }
    this_type &operator &= (EventType &e) {
      if (!e)
        ++missing;
      eventList.push_back(&e);
      e.addListener(this);
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
        bool signaled(EventWaiter *_e) {
          sce.notify();
          return false;
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
