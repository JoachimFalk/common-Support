//  -*- tab-width:8; intent-tabs-mode:nil;  c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c) 2017 Hardware-Software-CoDesign, University of
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

namespace CoSupport { namespace Tracing {

  class PajeTracer {
  public:

    struct Resource;
    struct Activity;
    struct Event;
    struct Link;

    PajeTracer(const std::string &traceFilename);

    Resource *registerResource(const char *name, Resource *parent = nullptr);
    Resource *registerResource(const char *name, String::Color const color, Resource *parent = nullptr);
    Activity *registerActivity(const char *description, bool useCache = false);
    Activity *registerActivity(const char *description, String::Color const color);
    Event    *registerEvent(const char *description, bool useCache = false);
    Event    *registerEvent(const char *description, String::Color const color);
    Link     *registerLink(const char *name);

    void traceLinkBegin(const char *name, Resource const *resource, sc_core::sc_time const start);
    void traceLinkEnd(const char *name, Resource const *resource, sc_core::sc_time const end);
    void traceActivity(Resource const *resouce, Activity const *activity, sc_core::sc_time const start, sc_core::sc_time const end);
    void traceEvent(Resource const *resouce, Event const *event, sc_core::sc_time const time);

    ~PajeTracer();
  protected:
    std::ofstream out;


    typedef std::map<std::string, Resource>     ResourceMap;
    typedef ResourceMap::iterator               ResourceIter;
    ResourceMap topResouces;

    typedef std::list<Activity>                 ActivityList;
    ActivityList activityList;
    /// Use this map to cache activity descriptions to their Activity
    /// if useCache is true in registerActivity.
    typedef std::map<std::string, Activity *>   ActivityMap;
    ActivityMap                                 activityMap;

    typedef std::map<char, int>                 LinkMap;
    LinkMap                                     linkMap;

    typedef std::list<Event>                    EventList;
    EventList                                   eventList;
    /// Use this map to cache event descriptions to their Event
    /// if useCache is true in registerEvent.
    typedef std::map<std::string, Event *>      EventMap;
    EventMap                                    eventMap;

    typedef std::list<Link>                     LinkList;
    LinkList                                    linkList;


    unsigned long aliasCounter;
    std::string getNextAlias();

    unsigned int colorCounter;
    String::Color getNextColor();

    unsigned int keyCounter;
    int getNextKey();

/*  // Map from gate state string to event type alias
    typedef std::map<std::string, std::string> GateEventMap;
    GateEventMap gateEventMap;
    unsigned int gateEventNextColor; */

  };

} } // namespace CoSupport::Tracing

#endif //_INCLUDED_COSUPPORT_TRACING_PAJETRACER_HPP
