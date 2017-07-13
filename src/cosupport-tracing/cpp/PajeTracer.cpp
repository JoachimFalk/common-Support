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

#include <CoSupport/Tracing/PajeTracer.hpp>
#include <CoSupport/String/DoubleQuotedString.hpp>

#include <iomanip>

namespace CoSupport { namespace Tracing {

  using String::Color;
  using String::lookupColor;
  using String::getColor;
  using namespace std;

  struct PajeTracer::Resource {
    std::string   alias;
    Resource     *parent;
    ResourceMap   children;
  };

  struct PajeTracer::Activity {
    std::string   alias;
  };

  struct PajeTracer::Event {
    std::string   alias;
  };

  struct PajeTracer::Link {
    std::string   alias;
  };

#define ALIAS_ARCHITECTURE   "a"
#define ALIAS_RESOURCE       "b"
#define ALIAS_RESOURCE_STATE "c"
#define ALIAS_CREATE_CON     "d"
#define ALIAS_LINKDEF        "e"

  PajeTracer::PajeTracer(std::string const &traceFilename)
    : out(traceFilename)
    , aliasCounter(26) // a-z are for fixed aliases
    , colorCounter(0)
  {
    out << std::fixed << std::setprecision(9) <<
        "%EventDef PajeDefineContainerType 100\n"
        "% Alias string\n"
        "% ContainerType string\n"
        "% Name string\n"
        "%EndEventDef\n"
        "%EventDef PajeDefineStateType 101\n"
        "% Alias string\n"
        "% ContainerType string\n"
        "% Name string\n"
        "%EndEventDef\n"
        "%EventDef PajeDefineEventType 102\n"
        "% Alias string\n"
        "% ContainerType string\n"
        "% Name string\n"
        "% Color color\n"
        "%EndEventDef\n"
        "%EventDef PajeCreateContainer 103\n"
        "% Time date\n"
        "% Alias string\n"
        "% Type string\n"
        "% Container string\n"
        "% Name string\n"
        "%EndEventDef\n"
        "%EventDef PajeDestroyContainer 104\n"
        "% Time date\n"
        "% Name string\n"
        "% Type string\n"
        "%EndEventDef\n"
        "%EventDef PajeDefineLinkType 105\n"
        "% Alias string\n"
        "% Type string\n"
        "% StartContainerType string\n"
        "% EndContainerType string\n"
        "% Name string\n"
        "%EndEventDef\n"
        "%EventDef PajeDefineEntityValue 1\n"
        "% Alias string\n"
        "% Type string\n"
        "% Name string\n"
        "% Color color\n"
        "%EndEventDef\n"
        "%EventDef PajeNewEvent 2\n"
        "% Time date\n"
        "% Type string\n"
        "% Container string\n"
        "% Value string\n"
        "%EndEventDef\n"
        "%EventDef PajePushState 3\n"
        "% Time date\n"
        "% Type string\n"
        "% Container string\n"
        "% Value string\n"
        "%EndEventDef\n"
        "%EventDef PajePopState 4\n"
        "% Time date\n"
        "% Type string\n"
        "% Container string\n"
        "%EndEventDef\n"
        "%EventDef PajeSetState 5\n"
        "% Time date\n"
        "% Type string\n"
        "% Container string\n"
        "% Value string\n"
        "%EndEventDef\n"
        "%EventDef PajeStartLink 6\n"
        "% Time date\n"
        "% Type string\n"
        "% Container string\n"
        "% StartContainer string\n"
        "% Value string\n"
        "% Key string\n"
        "%EndEventDef\n"
        "%EventDef PajeEndLink 7\n"
        "% Time date\n"
        "% Type string\n"
        "% Container string\n"
        "% EndContainer string\n"
        "% Value string\n"
        "% Key string\n"
        "%EndEventDef\n"
        "100 " ALIAS_ARCHITECTURE " \"0\" \"Architecture\" \n"
        "100 " ALIAS_RESOURCE " " ALIAS_ARCHITECTURE " \"Top Resource\"\n"
        "101 " ALIAS_RESOURCE_STATE " " ALIAS_RESOURCE " \"Activity\"\n"
        "103 0 " ALIAS_CREATE_CON " " ALIAS_ARCHITECTURE " \"0\" \"Architecture\"\n"
        "105 " ALIAS_LINKDEF " " ALIAS_ARCHITECTURE " " ALIAS_RESOURCE " " ALIAS_RESOURCE " \"LinkDef\" \n"
        ;//
  }

  PajeTracer::~PajeTracer()
    {}

  std::string PajeTracer::getNextAlias() {
    std::string retval;
    unsigned long count = aliasCounter++;
    do {
      unsigned int digit = count % (2*26); count /= 2*26;
      if (digit >= 26)
        retval.append(1, 'A' + digit-26);
      else
        retval.append(1, 'a' + digit);
    } while (count > 0);
    return retval;
  }

  Color PajeTracer::getNextColor()
    { return getColor(colorCounter++); }

  int PajeTracer::getNextKey() {
    return (keyCounter++);
  }

  PajeTracer::Resource *PajeTracer::registerResource(const char *name, Resource *parent)
    { return registerResource(name, getNextColor(), parent); }

  PajeTracer::Resource *PajeTracer::registerResource(const char *name, String::Color const color, Resource *parent) {
    ResourceMap &parentResourceMap = parent
        ? parent->children
        : topResouces;

    std::pair<ResourceIter, bool> success =
        parentResourceMap.insert(ResourceMap::value_type(name, Resource()));
    if (!success.second)
      throw std::runtime_error(("Can't register duplicate resource "+std::string(name)).c_str());
    Resource &newRes = success.first->second;
    newRes.alias  = getNextAlias();
    newRes.parent = parent;
    out << "103 0 " <<  newRes.alias << " " ALIAS_RESOURCE " "
        << (parent ? parent->alias : "d") << " "
        << String::DoubleQuotedString(name) << "\n" << std::flush;
    return &newRes;
  }

  PajeTracer::Activity *PajeTracer::registerActivity(const char *description, bool useCache) {
    if (useCache) {
      std::pair<ActivityMap::iterator, bool> insertStatus(
          activityMap.find(description), true);
      if (insertStatus.first == activityMap.end()) {
        insertStatus = activityMap.insert(ActivityMap::value_type(
            description, registerActivity(description, getNextColor())));
        assert(insertStatus.second && "Oops: description not in map, but insertion failed?!");
      }
      return insertStatus.first->second;
    } else
      return registerActivity(description, getNextColor());
  }

  PajeTracer::Activity *PajeTracer::registerActivity(const char *description, String::Color const color) {
    activityList.push_back(Activity());
    Activity &newAct = activityList.back();
    newAct.alias = getNextAlias();
    out << "1 " << newAct.alias << " " ALIAS_RESOURCE_STATE " " << String::DoubleQuotedString(description) << " \""
        << static_cast<float>(color.r())/255 << " "
        << static_cast<float>(color.g())/255 << " "
        << static_cast<float>(color.b())/255 << "\"\n" << std::flush;
    return &newAct;
  }

  PajeTracer::Event *PajeTracer::registerEvent(const char *description, bool useCache) {
    if (useCache) {
      std::pair<EventMap::iterator, bool> insertStatus(
          eventMap.find(description), true);
      if (insertStatus.first == eventMap.end()) {
        insertStatus = eventMap.insert(EventMap::value_type(
            description, registerEvent(description, getNextColor())));
        assert(insertStatus.second && "Oops: description not in map, but insertion failed?!");
      }
      return insertStatus.first->second;
    } else
      return registerEvent(description, getNextColor());
  }

  PajeTracer::Event *PajeTracer::registerEvent(const char *description, String::Color const color) {
    eventList.push_back(Event());
    Event &newEv = eventList.back();
    newEv.alias = getNextAlias();

    out << "102 " << newEv.alias << " " ALIAS_RESOURCE " " << String::DoubleQuotedString(description) << " \""
        << static_cast<float>(color.r())/255 << " "
        << static_cast<float>(color.g())/255 << " "
        << static_cast<float>(color.b())/255 << "\"\n" << std::flush;
    return &newEv;
  }

  PajeTracer::Link *PajeTracer::registerLink(const char *name){
    linkList.push_back(Link());
    Link &newLink = linkList.back();
    newLink.alias = getNextAlias();

    LinkMap::const_iterator iterLink = linkMap.find(*name);
    if (iterLink == linkMap.end()) {
      linkMap[*name] = getNextKey();
    }

    out << "1 " << newLink.alias << " " ALIAS_LINKDEF " " << String::DoubleQuotedString(name) << " \""
            << 0 << " "
            << 0 << " "
            << 0 << "\"\n" << std::flush;
    return &newLink;
  }

  void PajeTracer::traceLinkBegin(const char *name, Resource const *resource, sc_core::sc_time const start){
    out << "6 " << start.to_seconds() << " " ALIAS_LINKDEF " " ALIAS_CREATE_CON " " << resource->alias << " " << name << " "  << linkMap.find(*name)->second << "\n" << std::flush;
  }

  void PajeTracer::traceLinkEnd(const char *name, Resource const *resource, sc_core::sc_time const end){
    out << "7 " << end.to_seconds() << " " ALIAS_LINKDEF " " ALIAS_CREATE_CON " " << resource->alias << " " << name << " " << linkMap.find(*name)->second << "\n" << std::flush;
  }

  void PajeTracer::traceActivity(Resource const *resource, Activity const *activity, sc_core::sc_time const start, sc_core::sc_time const end) {
      out << "3 " << start.to_seconds() << " " ALIAS_RESOURCE_STATE " " << resource->alias << " " << activity->alias << "\n";
      out << "4 " << end.to_seconds() << " " ALIAS_RESOURCE_STATE " " << resource->alias << "\n" << std::flush;
  }

  void PajeTracer::traceEvent(Resource const *resouce, Event const *event, sc_core::sc_time const time) {
    out << "2 " << time.to_seconds() << " " << event->alias << " " << resouce->alias << "\n" << std::flush;
  }

} } // namespace CoSupport::Tracing
