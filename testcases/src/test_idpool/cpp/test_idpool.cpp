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

#include <CoSupport/compatibility-glue/nullptr.h>

#include <CoSupport/Allocators/IdPool.hpp>

#include <map>
#include <iostream>

using namespace CoSupport::Allocators;

class foo {};

IdPool<std::map<uint64_t, foo *> > idPool;

int main(int argc, char *argv[]) {
  for (size_t i = 0; i < 15; ++i) {
    std::cout << "idAlloc => " << idPool.idAlloc(nullptr)->first << std::endl;
  }
  idPool.idFree(7);  std::cout << "idFree(7)" << std::endl;
  idPool.idFree(3);  std::cout << "idFree(3)" << std::endl;
  idPool.idFree(13); std::cout << "idFree(13)" << std::endl;
  for (size_t i = 0; i < 15; ++i) {
    std::cout << "idAlloc => " << idPool.idAlloc(nullptr)->first << std::endl;
  }

  return 0;
}
