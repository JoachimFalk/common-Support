// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_INITIALIZER_BASICINITIALIZER_HPP
#define _INCLUDED_COSUPPORT_INITIALIZER_BASICINITIALIZER_HPP

#include <string>
#include <map>
#include <typeinfo>

#include "export_config.h"

namespace CoSupport { namespace Initializer {

namespace Detail {

  class COSUPPORT_INITIALIZER_API
  GlobalBasicInitializerRegistry {
  public:
    typedef std::map<std::string, size_t> Map;
  private:
    static size_t  refCount;
    static void   *mtx; // Data hiding this is a boost::mutex *
  protected:
    static Map    *map; // BasicInitializerMap
  protected:
    const char *key;
    void      (*initialize)();
    void      (*terminate)();
  public:
    GlobalBasicInitializerRegistry(
      const char *_key, void (_initialize)(), void (_terminate)());
    ~GlobalBasicInitializerRegistry();
  };

} // namespace Detail


template <typename T> struct
BasicInitializerTraits {
  static void initialize()
    { T::initialize(); }
  static void terminate()
    { T::terminate(); }
};

template <typename T> struct
BasicInitializer: public Detail::GlobalBasicInitializerRegistry {
  BasicInitializer()
    : Detail::GlobalBasicInitializerRegistry(
        typeid(T()).name(),
        BasicInitializerTraits<T>::initialize,
        BasicInitializerTraits<T>::terminate)
    {}
};

} } // namespace CoSupport::Initializer

#endif // _INCLUDED_COSUPPORT_INITIALIZER_BASICINITIALIZER_HPP
