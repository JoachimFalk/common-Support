// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2021 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_STRING_ENVIRONMENT_HPP
#define _INCLUDED_COSUPPORT_STRING_ENVIRONMENT_HPP

#include "export_config.h"

#include <map>

namespace CoSupport { namespace String {

/// Represent (environment) variables as a key (i.e., variable name) value (i.e.,
/// variable value) map.
class COSUPPORT_STRING_API
Environment
  : public std::map<std::string, std::string>
{
public:
  Environment(std::map<std::string, std::string> const &env)
    : std::map<std::string, std::string>(env)
    , env(nullptr) {}
  /// Initialize environment from an array of character pointers to environment
  /// strings. That is from a POSIX environ variable. The Environment instance
  /// will not take ownership of the env argument but use it to populate the
  /// std::map.
  Environment(const char **env);

  ~Environment() {
    if (env)
      freeEnv();
  }

  /// Returns a POSIX environ variable. Deallocation of the return environment
  /// will be performed by the Environment instance. Thus, the returned environment
  /// remains valid until the next call to Environment::getEnv or until the
  /// Environment instance is destroyed.
  const char **getEnv();

  /// Deallocate environment returned by the last Environment::getEnv call.
  void         freeEnv();
private:
  const char **env;
};

} } // namespace CoSupport::String

#endif /* _INCLUDED_COSUPPORT_STRING_ENVIRONMENT_HPP */
