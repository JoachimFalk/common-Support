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

using CoSupport::String::Environment;

#include <iostream>

#include <cassert>
#include <cstring>

void dumpEnv(std::string const &envName, Environment &env) {
  std::cout << envName << ":" << std::endl;
  for (Environment::value_type v : env) {
    std::cout << "  " << v.first << " => " << v.second << std::endl;
  }
  std::cout << envName << ".getEnv():" << std::endl;
  for (const char **str = env.getEnv(); *str != nullptr; ++str) {
    std::cout << "  " << *str << std::endl;
  }
}

int main(int argc, char *argv[]) {
  const char *var1 = "A=A";
  const char *var2 = "AA=C=X";
  const char *var3 = "A=B";
  const char *envAArray[] = { var1, var2, var3, nullptr };

  Environment envA(envAArray);

  dumpEnv("envA", envA);
  envA["FOO"] = "b=ar";
  dumpEnv("envA", envA);
  envA["X=Y"] = "Z";
  try {
    dumpEnv("envA", envA);
  } catch (std::runtime_error const &e) {
    std::cout << e.what() << std::endl;
  }

  const char *var4 = "KKK";
  const char *envBArray[] = { var1, var4, var2, var3, nullptr };
  try {
    Environment envB(envBArray);
  } catch (std::runtime_error const &e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
