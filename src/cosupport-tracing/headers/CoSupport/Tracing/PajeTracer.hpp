//  -*- tab-width:8; intent-tabs-mode:nil;  c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c) 2017-2020 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_TRACING_PAJETRACER_HPP
#define _INCLUDED_COSUPPORT_TRACING_PAJETRACER_HPP

#include <systemc>

#include <CoSupport/String/color.hpp>

#include <map>
#include <list>
#include <fstream>

namespace CoSupport { namespace Tracing {

  class PajeTracer {
  protected:
    struct ContainerType;
    struct ActivityType;
    struct EventType;
    struct GaugeType;
  public:

    typedef String::Color Color;
    typedef GaugeType     Gauge;

    struct Container;
    struct Activity;
    struct Event;
    struct Link;

    PajeTracer(const std::string &traceFilename);

    Container *getOrCreateContainer(
        const char *name
      , Container  *parent = nullptr
      , const char *type = nullptr)
      { return registerContainer(name, true, parent, type); }
    Container *createContainer(
        const char *name
      , Container  *parent = nullptr
      , const char *type = nullptr)
      { return registerContainer(name, false, parent, type); }

    Activity *getOrCreateActivity(
        const char *name
      , Container  *parent = nullptr
      , const char *type = nullptr)
      { return registerActivity(name, nullptr, true, parent, type); }
    Activity *getOrCreateActivity(
        const char *name
      , Color       color
      , Container  *parent = nullptr
      , const char *type = nullptr)
      { return registerActivity(name, &color, true, parent, type); }
    Activity *createActivity(
        const char *name
      , Container  *parent = nullptr
      , const char *type = nullptr)
      { return registerActivity(name, nullptr, false, parent, type); }
    Activity *createActivity(
        const char *name
      , Color       color
      , Container  *parent = nullptr
      , const char *type = nullptr)
      { return registerActivity(name, &color, false, parent, type); }

    Event *getOrCreateEvent(
        const char *name
      , Container  *parent = nullptr
      , const char *type = nullptr)
      { return registerEvent(name, nullptr, true, parent, type); }
    Event *getOrCreateEvent(
        const char *name
      , Color       color
      , Container  *parent = nullptr
      , const char *type = nullptr)
      { return registerEvent(name, &color, true, parent, type); }
    Event *createEvent(
        const char *name
      , Container  *parent = nullptr
      , const char *type = nullptr)
      { return registerEvent(name, nullptr, false, parent, type); }
    Event *createEvent(
        const char *name
      , Color       color
      , Container  *parent = nullptr
      , const char *type = nullptr)
      { return registerEvent(name, &color, false, parent, type); }

    Gauge *getOrCreateGauge(
        const char *type
      , Container  *parent = nullptr)
      { return registerGauge(type, parent); }

//  Link      *registerLink(const char *name);

    Color const &getColor(Activity *a) const;
    Color const &getColor(Event    *e) const;

//  void traceLinkBegin(const char *name, Container const *resource, sc_core::sc_time const start);
//  void traceLinkEnd(const char *name, Container const *resource, sc_core::sc_time const end);
    void traceActivity(Container const *resouce, Activity const *activity, sc_core::sc_time const start, sc_core::sc_time const end);
    void traceEvent(Container const *resouce, Event const *event, sc_core::sc_time const time);
    void traceGauge(Container const *resouce, Gauge const *gauge, sc_core::sc_time const time, double value);

    ~PajeTracer();
  protected:

    Container *registerContainer(
        const char *name
      , bool        unique
      , Container  *parent
      , const char *type);

    Activity  *registerActivity(
        const char  *name
      , Color       *color
      , bool         unique
      , Container   *parent
      , const char  *type);

    Event     *registerEvent(
        const char  *name
      , Color       *color
      , bool         unique
      , Container   *parent
      , const char  *type);

    Gauge     *registerGauge(
        const char *type
      , Container  *parent);

    std::ofstream out;

    typedef std::list<ContainerType> ContainerTypeList;
    ContainerTypeList                containerTypeList;
    typedef std::map<std::string
      , ContainerType *>             ContainerTypeMap;
    ContainerTypeMap                 containerTypeMap;

    typedef std::list<ActivityType>  ActivityTypeList;
    ActivityTypeList                 activityTypeList;
    typedef std::map<std::string
      , ActivityType *>              ActivityTypeMap;


    typedef std::list<EventType>     EventTypeList;
    EventTypeList                    eventTypeList;
    typedef std::map<
        std::pair<std::string, Color>
      , EventType *>                 EventTypeMap;

    typedef std::list<GaugeType>     GaugeTypeList;
    GaugeTypeList                    gaugeTypeList;
    typedef std::map<std::string
      , GaugeType *>                 GaugeTypeMap;

    typedef std::list<Container>     ContainerList;
    ContainerList                    containerList;
    typedef std::map<std::string
      , Container *>                 ContainerMap;
    /// Use this map to cache resource name to their Resource
    /// if useCache is true and parent is nullptr in registerContainer.
    ContainerMap                     containerMap;

    typedef std::list<Activity>      ActivityList;
    ActivityList                     activityList;
    typedef std::map<
        std::pair<std::string, std::string>
      , Activity *>                  ActivityMap;

    typedef std::list<Event>         EventList;
    EventList                        eventList;
    typedef std::map<
        std::pair<std::string, std::string>
      , Event *>                     EventMap;

    typedef std::list<Link>          LinkList;
    LinkList                         linkList;
    typedef std::map<char, int>      LinkMap;
    LinkMap                          linkMap;

    unsigned long aliasCounter;
    std::string getNextAlias();

    unsigned int colorCounter;
    String::Color getNextColor();

    unsigned int keyCounter;
    int getNextKey();
  };

} } // namespace CoSupport::Tracing

#endif //_INCLUDED_COSUPPORT_TRACING_PAJETRACER_HPP
