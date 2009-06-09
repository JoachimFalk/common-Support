/*
 * Copyright (c) 2004-2007 Hardware-Software-CoDesign, University of
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

#include <CoSupport/Initializer/BasicInitializer.hpp>

#include <boost/thread/mutex.hpp>
#include <cassert>

namespace CoSupport { namespace Initializer {

namespace Detail {
  typedef GlobalBasicInitializerRegistry::Map Map;

  size_t  GlobalBasicInitializerRegistry::refCount = 0;
  void   *GlobalBasicInitializerRegistry::mtx      = NULL;
  Map    *GlobalBasicInitializerRegistry::map      = NULL;

  // This should be the first global object derived from
  // GlobalBasicInitializerRegistry. But you can never tell
  // as the C++ standard leaves global object initialization
  // sequence for global objects in different link units as
  // implementation defined!
  static GlobalBasicInitializerRegistry globalStartup(NULL,NULL,NULL);

  GlobalBasicInitializerRegistry::GlobalBasicInitializerRegistry(
    const char *_key, void (_initialize)(), void (_terminate)())
    : key(_key), initialize(_initialize), terminate(_terminate)
  {
    // I surely hope global object ctor/dtor calls don't have any races!
    if (mtx == NULL) {
      // I am the first global object ctor!
      assert(refCount == 0 && map == NULL);
      // Allocate map and mutex
      try {
        mtx = new boost::mutex();
        map = new Map();
      } catch (...) {
        delete reinterpret_cast<boost::mutex *>(mtx); mtx = NULL;
        delete map; map = NULL;
        throw;
      }
    }
    // mtx and map must be setup!
    assert(mtx != NULL && map != NULL);
    {
      boost::mutex::scoped_lock lck(*reinterpret_cast<boost::mutex *>(mtx));
      ++refCount;
      if (_key != NULL && map->operator[](_key)++ == 0)
        (*_initialize)();
    }
  }

  GlobalBasicInitializerRegistry::~GlobalBasicInitializerRegistry() {
    bool cleanup;
    // mtx and map must be setup!
    assert(mtx != NULL && map != NULL);
    {
      boost::mutex::scoped_lock lck(*reinterpret_cast<boost::mutex *>(mtx));
      cleanup = --refCount == 0;
      if (key != NULL && --map->operator[](key) == 0) {
        map->erase(key);
        (*terminate)();
      }
    }
    // I surely hope global object ctor/dtor calls don't have any races!
    if (cleanup) {
      // I am the last global object dtor!
      assert(map->empty());
      delete reinterpret_cast<boost::mutex *>(mtx); mtx = NULL;
      delete map; map = NULL;
    }
  }

} // namespace Detail

} } // namespace CoSupport::Initializer
