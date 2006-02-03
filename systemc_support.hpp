#ifndef _INCLUDED_SYSTEMC_SUPPORT_HPP
#define _INCLUDED_SYSTEMC_SUPPORT_HPP

#include <systemc.h>

#include <iostream>

#include <vector>
#include <set>

namespace CoSupport { namespace SystemC {

  // Event handling
 
  class EventListener {
  public:
    friend class Event;
  protected:
    virtual void signaled( Event *e ) = 0;
  };

  class Event {
  public:
    typedef Event             this_type;
    typedef void (this_type::*unspecified_bool_type)();
  protected:
    typedef std::set<EventListener *>  ell_ty;
    
    size_t  missing;
    ell_ty  ell;
  public:
    Event(bool startNotified = false)
      : missing(startNotified ? 0 : 1)
      {}
    
    void notify() {
      missing = 0;
      for ( ell_ty::iterator iter = ell.begin();
            iter != ell.end();
            ++iter )
        (*iter)->signaled(this);
      ell.clear();
    }
    
    virtual void addListener(EventListener *el) {
      // avoid annoying compiler warnings
      //bool success = ell.insert(el).second;
      ell.insert(el).second;
  //    assert( success );
    }
    virtual void delListener(EventListener *el) {
      ell.erase(el);
    }
    
    virtual void reset()
      { missing = 1; }
    
    operator unspecified_bool_type() const
      { return missing == 0 ? &this_type::notify : NULL; }
    
    class EventOrList  operator | ( Event &p );
    class EventAndList operator & ( Event &p );
    
    virtual ~Event() {}
    
    void dump(std::ostream &out) const {
      std::cout << "Event( missing: " << missing << ")";
    }
  private:
    // disable
    // Event( const this_type & );
  };

  static inline
  std::ostream &operator << ( std::ostream &out, const Event &se ) {
    se.dump(out); return out;
  }

  class EventOrList
  : public Event,
    protected EventListener {
  protected:
    typedef std::vector<Event *> EventList;
    
    EventList eventList;
    
    void signaled( Event *e )
      { clearListener(); notify();  }
    
    void clearListener() {
      for ( EventList::iterator iter = eventList.begin();
            iter != eventList.end();
            ++iter )
        (*iter)->delListener(this);
    }
  public:
    typedef EventOrList this_type;
    
    EventOrList()
      {}
    EventOrList( Event &p )
      { *this |= p; }
    this_type operator | ( Event &p )
      { return this_type(*this) |= p; }
    this_type &operator |= ( Event &p ) {
      if (p)
        missing = 0;
      eventList.push_back(&p);
      return *this;
    }
    
    void addListener(EventListener *el) {
      if ( ell.empty() ) {
        missing = 1;
        for ( EventList::iterator iter = eventList.begin();
              iter != eventList.end();
              ++iter ) {
          if ( !**iter ) {
            (*iter)->addListener(this);
          } else {
            missing = 0;
          }
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
  };

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

  inline
  EventOrList  Event::operator | ( Event &p )
    { return EventOrList(*this) |= p; }

  inline
  EventAndList Event::operator & ( Event &p )
    { return EventAndList(*this) &= p; }

  static inline
  void notify(Event& se)
    { return se.notify(); }
  static inline
  void reset(Event& se)
    { return se.reset(); }
  static inline
  void wait( Event &se ) {
    if ( !se ) {
      struct _: public EventListener {
        sc_event e;
        void signaled( Event * )
        { e.notify(); }
        virtual ~_() {}
      } w;
      se.addListener(&w);
      wait(w.e);
    }
  }

} } // CoSupport::SystemC

#endif // _INCLUDED_SYSTEMC_SUPPORT_HPP
