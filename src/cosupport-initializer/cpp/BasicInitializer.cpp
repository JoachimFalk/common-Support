// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <CoSupport/compatibility-glue/nullptr.h>

#include <CoSupport/Initializer/BasicInitializer.hpp>

#include <boost/thread/mutex.hpp>
#include <cassert>

namespace CoSupport { namespace Initializer {

namespace Detail {
  typedef GlobalBasicInitializerRegistry::Map Map;

  size_t  GlobalBasicInitializerRegistry::refCount = 0;
  void   *GlobalBasicInitializerRegistry::mtx      = nullptr;
  Map    *GlobalBasicInitializerRegistry::map      = nullptr;

  // This should be the first global object derived from
  // GlobalBasicInitializerRegistry. But you can never tell
  // as the C++ standard leaves global object initialization
  // sequence for global objects in different link units as
  // implementation defined!
  static GlobalBasicInitializerRegistry globalStartup(nullptr,nullptr,nullptr);

  GlobalBasicInitializerRegistry::GlobalBasicInitializerRegistry(
    const char *_key, void (_initialize)(), void (_terminate)())
    : key(_key), initialize(_initialize), terminate(_terminate)
  {
    // I surely hope global object ctor/dtor calls don't have any races!
    if (mtx == nullptr) {
      // I am the first global object ctor!
      assert(refCount == 0 && map == nullptr);
      // Allocate map and mutex
      try {
        mtx = new boost::mutex();
        map = new Map();
      } catch (...) {
        delete reinterpret_cast<boost::mutex *>(mtx); mtx = nullptr;
        delete map; map = nullptr;
        throw;
      }
    }
    // mtx and map must be setup!
    assert(mtx != nullptr && map != nullptr);
    {
      boost::mutex::scoped_lock lck(*reinterpret_cast<boost::mutex *>(mtx));
      ++refCount;
      if (_key != nullptr && map->operator[](_key)++ == 0)
        (*_initialize)();
    }
  }

  GlobalBasicInitializerRegistry::~GlobalBasicInitializerRegistry() {
    bool cleanup;
    // mtx and map must be setup!
    assert(mtx != nullptr && map != nullptr);
    {
      boost::mutex::scoped_lock lck(*reinterpret_cast<boost::mutex *>(mtx));
      cleanup = --refCount == 0;
      if (key != nullptr && --map->operator[](key) == 0) {
        map->erase(key);
        (*terminate)();
      }
    }
    // I surely hope global object ctor/dtor calls don't have any races!
    if (cleanup) {
      // I am the last global object dtor!
      assert(map->empty());
      delete reinterpret_cast<boost::mutex *>(mtx); mtx = nullptr;
      delete map; map = nullptr;
    }
  }

} // namespace Detail

} } // namespace CoSupport::Initializer
