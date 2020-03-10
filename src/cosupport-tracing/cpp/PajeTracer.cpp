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

#include <CoSupport/Tracing/PajeTracer.hpp>
#include <CoSupport/String/DoubleQuotedString.hpp>

#include <iomanip>
#include <stdexcept>
#include <sstream>

namespace CoSupport { namespace Tracing {

  using String::Color;
  using String::lookupColor;

  struct PajeTracer::ContainerType {
    std::string      alias;
    ContainerType   *parent;             ///< Parent container type
    ContainerTypeMap childContTypes;     ///< Child container types
    ActivityTypeMap  childActivityTypes; ///< Child activity types
    EventTypeMap     childEventTypes;    ///< Child event types
    GaugeTypeMap     childGaugeTypes;    ///< Child gauge types
  };

  struct PajeTracer::Container {
    std::string      alias;
    ContainerType   *type;               ///< Type of container
    Container       *next;               ///< Next container with identical name
    ContainerMap     childContainters;   ///< Containers nested within this container.
    ActivityMap      childActivities;    ///< Activities that can run on this container.
    EventMap         childEvents;        ///< Events that can happen on this container.
  };

  struct PajeTracer::ActivityType {
    std::string      alias;
    ContainerType   *parent; ///< Parent container type
  };

  struct PajeTracer::Activity {
    std::string      alias;
    ActivityType    *type;   ///< Type of activity
    Activity        *next;   ///< Next activity with identical name and type


    String::Color  color;
  };

  struct PajeTracer::EventType {
    std::string      alias;
    ContainerType   *parent; ///< Parent container type
    Color            color;
  };

  struct PajeTracer::Event {
    std::string      alias;
    EventType       *type;
    Event           *next;   ///< Next event with identical name and type
  };

  struct PajeTracer::GaugeType {
    std::string      alias;
    ContainerType   *parent; ///< Parent container type
  };

  struct PajeTracer::Link {
    std::string      alias;
  };

#define TYPE_DEF_CONT  "100"
#define TYPE_DEF_ACT   "101"
#define TYPE_DEF_EVENT "102"
#define TYPE_DEF_GAUGE "106"

#define CONT_CREATE    "103"
#define CONT_DESTORY   "104"

#define VALUE_DEF      "1"
#define EVENT_OCCUR    "2"

#define GAUGE_SET      "8"

  PajeTracer::PajeTracer(std::string const &traceFilename)
    : out(traceFilename)
    , aliasCounter(0)
    , colorCounter(0)
    , keyCounter(0)
  {
    out << std::fixed << std::setprecision(12) <<
        "%EventDef PajeDefineContainerType " TYPE_DEF_CONT "\n"
        "% Alias string\n"
        "% ContainerType string\n"
        "% Name string\n"
        "%EndEventDef\n"
        "%EventDef PajeDefineStateType " TYPE_DEF_ACT "\n"
        "% Alias string\n"
        "% ContainerType string\n"
        "% Name string\n"
        "%EndEventDef\n"
        "%EventDef PajeDefineEventType " TYPE_DEF_EVENT "\n"
        "% Alias string\n"
        "% ContainerType string\n"
        "% Name string\n"
        "% Color color\n"
        "%EndEventDef\n"
//      "%EventDef PajeDefineLinkType 105\n"
//      "% Alias string\n"
//      "% Type string\n"
//      "% StartContainerType string\n"
//      "% EndContainerType string\n"
//      "% Name string\n"
//      "%EndEventDef\n"
        "%EventDef PajeDefineVariableType " TYPE_DEF_GAUGE "\n"
        "% Alias string\n"
        "% Type string\n"
        "% Name string\n"
//      "% Color color\n"
        "%EndEventDef\n"
        "%EventDef PajeDefineEntityValue " VALUE_DEF "\n"
        "% Alias string\n"
        "% Type string\n"
        "% Name string\n"
        "% Color color\n"
        "%EndEventDef\n"
        "%EventDef PajeCreateContainer " CONT_CREATE "\n"
        "% Time date\n"
        "% Alias string\n"
        "% Type string\n"
        "% Container string\n"
        "% Name string\n"
        "%EndEventDef\n"
        "%EventDef PajeDestroyContainer " CONT_DESTORY "\n"
        "% Time date\n"
        "% Name string\n"
        "% Type string\n"
        "%EndEventDef\n"
        "%EventDef PajeNewEvent " EVENT_OCCUR "\n"
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
//      "%EventDef PajeStartLink 6\n"
//      "% Time date\n"
//      "% Type string\n"
//      "% Container string\n"
//      "% StartContainer string\n"
//      "% Value string\n"
//      "% Key string\n"
//      "%EndEventDef\n"
//      "%EventDef PajeEndLink 7\n"
//      "% Time date\n"
//      "% Type string\n"
//      "% Container string\n"
//      "% EndContainer string\n"
//      "% Value string\n"
//      "% Key string\n"
//      "%EndEventDef\n"
        "%EventDef PajeSetVariable " GAUGE_SET "\n"
        "% Time date\n"
        "% Type string\n"
        "% Container string\n"
        "% Value double\n"
        "%EndEventDef\n"
        ;
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
    { return String::getColor(colorCounter++); }

  int PajeTracer::getNextKey() {
    return (keyCounter++);
  }

  PajeTracer::Container *PajeTracer::registerContainer(
      const char *name
    , bool        unique
    , Container  *parent
    , const char *type_)
  {
    std::string    type;
    ContainerType *parentType = parent ? parent->type : nullptr;

    if (!type_) {
      int level = 0;
      for (ContainerType *p = parentType; p; p = p->parent)
        ++level;
      type = "Level" + std::to_string(level);
    } else
      type = type_;

    ContainerMap &parentContainerMap = parent
        ? parent->childContainters
        : containerMap;
    assert(name != nullptr);
    std::pair<ContainerMap::iterator, bool> insertCont =
        parentContainerMap.insert(ContainerMap::value_type(name, nullptr));
    if (insertCont.second || !unique) {
      ContainerTypeMap &parentContainerTypeMap = parentType
          ? parentType->childContTypes
          : containerTypeMap;
      std::pair<ContainerTypeMap::iterator, bool> insertType =
          parentContainerTypeMap.insert(ContainerTypeMap::value_type(type, nullptr));
      if (insertType.second) {
        // New container type needed
        containerTypeList.push_back(ContainerType());
        ContainerType &newType = containerTypeList.back();
        newType.alias  = getNextAlias();
        newType.parent = parentType;
        out << TYPE_DEF_CONT " " << newType.alias << " "
            << (parentType ? parentType->alias : "0") << " "
            << String::DoubleQuotedString(type) << "\n" << std::flush;
        insertType.first->second = &newType;
      }

      containerList.push_back(Container());
      Container &newCont = containerList.back();
      newCont.alias = getNextAlias();
      newCont.type  = insertType.first->second;
      newCont.next  = insertCont.first->second;
      assert(!insertCont.second || insertCont.first->second == nullptr);
      out << CONT_CREATE " 0 " << newCont.alias << " " << newCont.type->alias
          << " " << (parent ? parent->alias : "0")
          << " " << String::DoubleQuotedString(name) << "\n" << std::flush;
      insertCont.first->second = &newCont;
    } else {
      if (insertCont.first->second->next) {
        std::stringstream msg;
        msg << "The " << type << " container name " << name << " is not unique!";
        throw std::runtime_error(msg.str());
      }
    }
    return insertCont.first->second;
  }

  PajeTracer::Activity *PajeTracer::registerActivity(
      const char *name
    , Color      *color
    , bool        unique
    , Container  *parent
    , const char *type_)
  {
    assert(parent);

    std::string    type;
    ContainerType *parentType = parent->type;

    if (!type_) {
      int level = 0;
      for (ContainerType *p = parentType; p; p = p->parent)
        ++level;
      type = "Level" + std::to_string(level);
    } else
      type = type_;

    ActivityMap &parentContainerChildActivities = parent->childActivities;
    assert(name != nullptr);
    std::pair<ActivityMap::iterator, bool> insertAvtivity =
        parentContainerChildActivities.insert(ActivityMap::value_type(
            std::make_pair(std::string(name), type), nullptr));
    if (insertAvtivity.second || !unique) {
      ActivityTypeMap &parentContainerChildActivityTypes = parentType->childActivityTypes;
      std::pair<ActivityTypeMap::iterator, bool> insertType =
          parentContainerChildActivityTypes.insert(ActivityTypeMap::value_type(type, nullptr));
      if (insertType.second) {
        // New activity type needed
        activityTypeList.push_back(ActivityType());
        ActivityType &newType = activityTypeList.back();
        newType.alias  = getNextAlias();
        newType.parent = parentType;
        out << TYPE_DEF_ACT " " << newType.alias << " " << parentType->alias << " "
            << String::DoubleQuotedString(type) << "\n" << std::flush;
        insertType.first->second = &newType;
      }

      activityList.push_back(Activity());
      Activity &newAct = activityList.back();
      newAct.alias = getNextAlias();
      newAct.type  = insertType.first->second;
      assert(!insertAvtivity.second || insertAvtivity.first->second == nullptr);
      newAct.next  = insertAvtivity.first->second;
      newAct.color = color ? *color : getNextColor();
      out << VALUE_DEF " " << newAct.alias << " " << newAct.type->alias << " "
          << String::DoubleQuotedString(name) << " \""
          << static_cast<float>(newAct.color.r())/255 << " "
          << static_cast<float>(newAct.color.g())/255 << " "
          << static_cast<float>(newAct.color.b())/255 << "\"\n" << std::flush;
      insertAvtivity.first->second = &newAct;
    } else {
      if (insertAvtivity.first->second->next) {
        std::stringstream msg;
        msg << "The " << type << " activity name " << name << " is not unique!";
        throw std::runtime_error(msg.str());
      }
      if (color && insertAvtivity.first->second->color != *color) {
        std::stringstream msg;
        msg << "Inconsistent color used for " << type << " activity " << name << "!";
        throw std::runtime_error(msg.str());
      }
    }
    return insertAvtivity.first->second;
  }

  PajeTracer::Event *PajeTracer::registerEvent(
      const char  *name
    , Color       *color_
    , bool         unique
    , Container   *parent
    , const char  *type_)
  {
    assert(parent);

    std::string    type;
    ContainerType *parentType = parent->type;

    if (!type_) {
      int level = 0;
      for (ContainerType *p = parentType; p; p = p->parent)
        ++level;
      type = "Level" + std::to_string(level);
    } else
      type = type_;

    EventMap &parentContainerChildEvents = parent->childEvents;
    assert(name != nullptr);
    std::pair<EventMap::iterator, bool> insertEvent =
        parentContainerChildEvents.insert(EventMap::value_type(
            std::make_pair(std::string(name), type), nullptr));
    if (insertEvent.second || !unique) {
      Color color = color_ ? *color_ : getNextColor();

      EventTypeMap &parentContainerChildEventTypes = parentType->childEventTypes;
      std::pair<EventTypeMap::iterator, bool> insertType =
          parentContainerChildEventTypes.insert(EventTypeMap::value_type(
              std::make_pair(type, color), nullptr));
      if (insertType.second) {
        // New activity type needed
        eventTypeList.push_back(EventType());
        EventType &newType = eventTypeList.back();
        newType.alias  = getNextAlias();
        newType.parent = parentType;
        newType.color  = color;
        out << TYPE_DEF_EVENT " " << newType.alias << " " << parentType->alias << " "
            << String::DoubleQuotedString(type) << " \""
            << static_cast<float>(color.r())/255 << " "
            << static_cast<float>(color.g())/255 << " "
            << static_cast<float>(color.b())/255 << "\"\n" << std::flush;
        insertType.first->second = &newType;
      }

      eventList.push_back(Event());
      Event &newEvent = eventList.back();
      newEvent.alias = getNextAlias();
      newEvent.type  = insertType.first->second;
      assert(!insertEvent.second || insertEvent.first->second == nullptr);
      newEvent.next  = insertEvent.first->second;
      out << VALUE_DEF " " << newEvent.alias << " " << newEvent.type->alias << " "
          << String::DoubleQuotedString(name) << " \""
          << static_cast<float>(color.r())/255 << " "
          << static_cast<float>(color.g())/255 << " "
          << static_cast<float>(color.b())/255 << "\"\n" << std::flush;
      insertEvent.first->second = &newEvent;
    } else {
      if (insertEvent.first->second->next) {
        std::stringstream msg;
        msg << "The " << type << " activity name " << name << " is not unique!";
        throw std::runtime_error(msg.str());
      }
      if (color_ && insertEvent.first->second->type->color != *color_) {
        std::stringstream msg;
        msg << "Inconsistent color used for " << type << " event " << name << "!";
        throw std::runtime_error(msg.str());
      }
    }
    return insertEvent.first->second;
  }

  PajeTracer::Gauge *PajeTracer::registerGauge(
      const char *type_
    , Container  *parent)
  {
    assert(parent);

    std::string    type;
    ContainerType *parentType = parent->type;

    if (!type_) {
      int level = 0;
      for (ContainerType *p = parentType; p; p = p->parent)
        ++level;
      type = "Level" + std::to_string(level);
    } else
      type = type_;

    GaugeTypeMap &parentContainerChildGaugeTypes = parentType->childGaugeTypes;
    std::pair<GaugeTypeMap::iterator, bool> insertType =
        parentContainerChildGaugeTypes.insert(GaugeTypeMap::value_type(type, nullptr));
    if (insertType.second) {
      // New activity type needed
      gaugeTypeList.push_back(GaugeType());
      GaugeType &newType = gaugeTypeList.back();
      newType.alias  = getNextAlias();
      newType.parent = parentType;
      out << TYPE_DEF_GAUGE " " << newType.alias << " " << parentType->alias << " "
          << String::DoubleQuotedString(type) << "\n" << std::flush;
      insertType.first->second = &newType;
    }
    return insertType.first->second;
  }

  Color const &PajeTracer::getColor(Activity *a) const
    { return a->color; }
  Color const &PajeTracer::getColor(Event    *e) const
    { return e->type->color; }

/*
  PajeTracer::Link *PajeTracer::registerLink(const char *name){
    linkList.push_back(Link());
    Link &newLink = linkList.back();
    newLink.alias = getNextAlias();

    LinkMap::const_iterator iterLink = linkMap.find(*name);
    if (iterLink == linkMap.end()) {
      linkMap[*name] = getNextKey();
    }

    out << DEF_VALUE " " << newLink.alias << " " ALIAS_LINKDEF " " << String::DoubleQuotedString(name) << " \""
            << 0 << " "
            << 0 << " "
            << 0 << "\"\n" << std::flush;
    return &newLink;
  }

  void PajeTracer::traceLinkBegin(const char *name, Container const *resource, sc_core::sc_time const start){
    out << "6 " << start.to_seconds() << " " ALIAS_LINKDEF " " ALIAS_CREATE_CON " " << resource->alias << " " << name << " "  << linkMap.find(*name)->second << "\n" << std::flush;
  }

  void PajeTracer::traceLinkEnd(const char *name, Container const *resource, sc_core::sc_time const end){
    out << "7 " << end.to_seconds() << " " ALIAS_LINKDEF " " ALIAS_CREATE_CON " " << resource->alias << " " << name << " " << linkMap.find(*name)->second << "\n" << std::flush;
  }
 */
  void PajeTracer::traceActivity(Container const *r, Activity const *a, sc_core::sc_time const start, sc_core::sc_time const end) {
      out << "3 " << start.to_seconds() << " " << a->type->alias <<  " " << r->alias << " " << a->alias << "\n";
      out << "4 " << end.to_seconds() << " " << a->type->alias << " " << r->alias << "\n" << std::flush;
  }

  void PajeTracer::traceEvent(Container const *resouce, Event const *event, sc_core::sc_time const time) {
    out << EVENT_OCCUR " " << time.to_seconds() << " " << event->type->alias << " " << resouce->alias << " " << event->alias << "\n" << std::flush;
  }

  void PajeTracer::traceGauge(Container const *resouce, Gauge const *gauge, sc_core::sc_time const time, double value) {
    out << GAUGE_SET " " << time.to_seconds() << " " << gauge->alias << " " << resouce->alias << " " << value << "\n" << std::flush;
  }

} } // namespace CoSupport::Tracing
