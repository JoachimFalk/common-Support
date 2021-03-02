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

#include <CoSupport/String/Environment.hpp>

#include <cassert>
#include <cstring>
#include <stdexcept>

namespace CoSupport { namespace String {

Environment::Environment(const char **env_)
  : env(nullptr) {
  if (!env_)
    return;
  for (const char **str = env_; *str != nullptr; ++str) {
    const char *delim = strchr(*str, '=');
    if (!delim)
      throw std::runtime_error(std::string("Not an environment string: ")+*str);
    (*this)[std::string(*str, delim)] = delim+1;
  }
}

const char **Environment::getEnv() {
  freeEnv();
  env = new const char *[size()+1];
  try {
    const char **str = env;
    *str = nullptr;
    for (value_type const &v : *this) {
      if (v.first.find('=') != std::string::npos)
        throw std::runtime_error("Not an environment variable: "+v.first);
      std::string::size_type size = v.first.size()+1+v.second.size()+1;
      char *envstr = new char[size];
      *str++ = envstr;
      *str = nullptr;
      memcpy(envstr, v.first.data(), v.first.size());
      envstr += v.first.size();
      *envstr++ = '=';
      memcpy(envstr, v.second.data(), v.second.size());
      envstr += v.second.size();
      *envstr++ = '\0';
      assert(envstr == str[-1]+size);
    }
    assert(str == env + size() && *str == nullptr);
  } catch (std::exception const &e) {
    freeEnv();
    throw;
  }
  return env;
}

void         Environment::freeEnv() {
  if (!env)
    return;
  for (const char **str = env; *str != nullptr; ++str)
    delete[] *str;
  delete[] env;
  env = nullptr;
}

} } // namespace CoSupport::String
