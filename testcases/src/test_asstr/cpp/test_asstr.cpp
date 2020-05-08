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
  std::cout << asStr('A') << std::endl;
  std::cout << asStr(static_cast<signed char>('B')) << std::endl;
  std::cout << asStr(static_cast<unsigned char>('C')) << std::endl;
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
  std::cout << asStr(static_cast<int8_t>('X')) << std::endl;
  std::cout << asStr(static_cast<uint8_t>('Y')) << std::endl;
  std::cout << asStr(static_cast<int16_t>(32767)) << std::endl;
  std::cout << asStr(static_cast<uint16_t>(65535)) << std::endl;
  std::cout << asStr(static_cast<int32_t>(2147483647L)) << std::endl;
  std::cout << asStr(static_cast<uint32_t>(4294967295U)) << std::endl;
  std::cout << asStr(static_cast<int64_t>(9223372036854775807LL)) << std::endl;
  std::cout << asStr(static_cast<uint64_t>(18446744073709551615ULL)) << std::endl;
  return 0;
}
