// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
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

#include <CoSupport/String/convert.hpp>

#include <cassert>
#include <cstdint>

#include <iostream>
#include <limits>

using CoSupport::String::asStr;

class Dummy {

};

std::ostream &operator <<(std::ostream &out, Dummy const &) {
  return out << "DUMMY";
}

int main(int argc, char *argv[]) {
  std::cout << asStr(Dummy()) << std::endl;
  std::cout << asStr(1) << std::endl;
  std::cout << asStr(-1) << std::endl;
  std::cout << asStr(2U) << std::endl;
  std::cout << asStr(2L) << std::endl;
  std::cout << asStr(-2L) << std::endl;
  std::cout << asStr(3ULL) << std::endl;
  std::cout << asStr(3LL) << std::endl;
  std::cout << asStr(-3LL) << std::endl;
  std::cout << asStr(static_cast<float>(1.5)) << std::endl;
  std::cout << asStr(static_cast<double>(2.5)) << std::endl;
  std::cout << asStr(static_cast<long double>(3.5)) << std::endl;
  std::cout << asStr(65535) << std::endl;
  std::cout << asStr(32767) << std::endl;
  std::cout << asStr(-32768) << std::endl;
  std::cout << asStr(4294967295) << std::endl;
  std::cout << asStr(2147483647) << std::endl;
  std::cout << asStr(-2147483648) << std::endl;
  std::cout << asStr(18446744073709551615ULL) << std::endl;
  std::cout << asStr(9223372036854775807LL) << std::endl;
  std::cout << asStr(-9223372036854775807LL-1LL) << std::endl;
  std::cout << asStr(static_cast<short>(32767)) << std::endl;
  std::cout << asStr(static_cast<signed short>(32767)) << std::endl;
  std::cout << asStr(static_cast<unsigned short>(65535)) << std::endl;
  std::cout << asStr(static_cast<int>(32767)) << std::endl;
  std::cout << asStr(static_cast<signed int>(32767)) << std::endl;
  std::cout << asStr(static_cast<unsigned int>(65535)) << std::endl;
  std::cout << asStr(static_cast<long>(2147483647L)) << std::endl;
  std::cout << asStr(static_cast<signed long>(2147483647L)) << std::endl;
  std::cout << asStr(static_cast<unsigned long>(4294967295U)) << std::endl;
  std::cout << asStr(static_cast<long long>(9223372036854775807LL)) << std::endl;
  std::cout << asStr(static_cast<signed long long>(9223372036854775807LL)) << std::endl;
  std::cout << asStr(static_cast<unsigned long long>(18446744073709551615ULL)) << std::endl;
  std::cout << asStr(static_cast<int8_t>(127)) << std::endl;
  std::cout << asStr(static_cast<uint8_t>(255)) << std::endl;
  std::cout << asStr(static_cast<int16_t>(32767)) << std::endl;
  std::cout << asStr(static_cast<uint16_t>(65535)) << std::endl;
  std::cout << asStr(static_cast<int32_t>(2147483647L)) << std::endl;
  std::cout << asStr(static_cast<uint32_t>(4294967295U)) << std::endl;
  std::cout << asStr(static_cast<int64_t>(9223372036854775807LL)) << std::endl;
  std::cout << asStr(static_cast<uint64_t>(18446744073709551615ULL)) << std::endl;
  std::cout << asStr(true) << std::endl;
  std::cout << asStr(false) << std::endl;
  return 0;
}
