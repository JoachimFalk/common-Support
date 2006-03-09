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
    virtual void removeEvent(EventWaiter *e) = 0;
  };
  
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
        for ( ell_ty::iterator iter = ell.begin();
              iter != ell.end();
              ++iter ) {
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
      for ( ell_ty::iterator iter = ell.begin();
            iter != ell.end();
            ++iter )
        if (*iter != el)
          sassert(!(*iter)->signaled(this));
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

    class EventOrList  operator | (EventWaiter &e);
    class EventAndList operator & (EventWaiter &e);

    void dump(std::ostream &out) const {
      std::cout << "EventWaiter( missing: " << missing << ")";
    }

    virtual ~EventWaiter() {
      for ( ell_ty::iterator iter = ell.begin();
            iter != ell.end();
            ++iter )
        (*iter)->removeEvent(this);
    }
  private:
    // disable
    EventWaiter( const this_type & );
  };

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

    void reset() {
      EventWaiter::reset();
    }
  };
  
  static inline
  std::ostream &operator << (std::ostream &out, const EventWaiter &se) {
    se.dump(out); return out;
  }

  class EventOrList
  : public EventWaiter,
    protected EventListener {
  protected:
    typedef std::vector<EventWaiter *> EventList;
    
    EventList    eventList;
    EventWaiter *eventTrigger;
    
    bool signaled(EventWaiter *e) {
      bool retval;
      
      assert(
        e &&
        find(eventList.begin(), eventList.end(), e) != eventList.end());
      if (*e) {
        if (!eventTrigger)
          eventTrigger = e;
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

    void removeEvent(EventWaiter *e) {
      for ( EventList::iterator iter = eventList.begin();
            iter != eventList.end();
            ++iter )
        if (*iter != e)
          (*iter)->delListener(this);
      missing = 1;
      eventList.clear();
    }

    EventWaiter *getEventTrigger() {
      for ( EventList::iterator iter = eventList.begin();
            iter != eventList.end() && !eventTrigger;
            ++iter )
        if ( **iter )
          eventTrigger = *iter;
      assert((eventTrigger != NULL) == (missing <= 0));
      return eventTrigger;
    }
  public:
    typedef EventOrList this_type;
    
    EventOrList()
      : EventWaiter(false), eventTrigger(NULL) {
    }
    EventOrList(EventWaiter &e )
      : EventWaiter(false), eventTrigger(NULL) {
      *this |= e;
    }
    EventOrList(const EventOrList &el)
      : EventWaiter(false), eventTrigger(NULL) {
      for ( EventList::const_iterator iter = el.eventList.begin();
            iter != el.eventList.end();
            ++iter )
        *this |= **iter;
    }
    
    this_type operator | (EventWaiter &e)
      { return this_type(*this) |= e; }
    this_type &operator |= (EventWaiter &e) {
      if (e) {
        --missing;
        if (!eventTrigger)
          eventTrigger = &e;
      }
      eventList.push_back(&e);
      e.addListener(this);
      return *this;
    }
    
    EventWaiter *reset(EventListener *el = NULL) {
      EventWaiter *retval = NULL;
      
      if (missing <= 0) {
        retval = getEventTrigger()->reset(this);
        if (!*getEventTrigger()) {
          ++missing; eventTrigger = NULL;
        }
        // just toggled from enabled to disabled
        if (missing > 0)
          signalResetListener(el);
      }
      return retval;
    }
    void clear() {
      for ( EventList::iterator iter = eventList.begin();
            iter != eventList.end();
            ++iter )
        (*iter)->delListener(this);
      missing = 1;
      eventList.clear();
    }
    
    ~EventOrList()
      { clear(); }
  };

  class EventAndList
  : public EventWaiter,
    protected EventListener {
  protected:
    typedef std::vector<EventWaiter *> EventList;
    
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

    void removeEvent(EventWaiter *e) {
      for ( EventList::iterator iter = eventList.begin();
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
    EventAndList(EventWaiter &e)
      : EventWaiter(true)  {
      *this &= e;
    }
    EventAndList(const EventAndList &el)
      : EventWaiter(true)  {
      for ( EventList::const_iterator iter = el.eventList.begin();
            iter != el.eventList.end();
            ++iter )
        *this &= **iter;
    }
    
    this_type operator & (EventWaiter &e)
      { return this_type(*this) &= e; }
    this_type &operator &= (EventWaiter &e) {
      if (!e)
        ++missing;
      eventList.push_back(&e);
      e.addListener(this);
      return *this;
    }
    
    EventWaiter *reset(EventListener *el = NULL) {
      if (missing <= 0) {
        for ( EventList::iterator iter = eventList.begin();
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
      for ( EventList::iterator iter = eventList.begin();
            iter != eventList.end();
            ++iter )
        (*iter)->delListener(this);
      missing = 0;
      eventList.clear();
    }
    
    ~EventAndList()
      { clear(); }
  };

/*
  class EventAndList
  : public Event,
    protected EventListener {
  protected:
    typedef std::vector<Event *> EventList;

    EventList eventList;

    void signaled( Event *e ) {
      assert( missing > 0 );
      if ( !--missing )
        notify();
    }
  public:
    typedef EventAndList this_type;
    
    EventAndList()
      { missing = 0; }
    EventAndList( Event &p )
      { missing = 0; *this &= p; }
    this_type operator & ( Event &p )
      { return this_type(*this) &= p; }
    this_type &operator &= ( Event &p ) {
      if ( !p )
        ++missing;
      eventList.push_back(&p);
      return *this;
    }
    
    void addListener(EventListener *el) {
      if ( ell.empty() ) {
        missing = 0;
        for ( EventList::iterator iter = eventList.begin();
              iter != eventList.end();
              ++iter )
          if ( !**iter ) {
            ++missing;
            (*iter)->addListener(this);
          }
      }
      Event::addListener(el);
      if ( !missing )
        notify();
    }
    void reset() {
      missing = 0;
      for ( EventList::iterator iter = eventList.begin();
            iter != eventList.end();
            ++iter ) {
        ++missing;
        (*iter)->reset();
      }
    }
    void clear()
      { eventList.clear(); }
    
    virtual ~EventAndList() {}
  };
*/

  inline
  EventOrList  EventWaiter::operator | (EventWaiter &e)
    { return EventOrList(*this) |= e; }

  inline
  EventAndList EventWaiter::operator & (EventWaiter &e)
    { return EventAndList(*this) &= e; }

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
        void removeEvent(EventWaiter *_e) {
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
