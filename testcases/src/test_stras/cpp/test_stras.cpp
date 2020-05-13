#include <CoSupport/String/convert.hpp>

#include <cassert>
#include <cstdint>

#include <iostream>
#include <limits>

using CoSupport::String::strAs;
using CoSupport::String::asStr;

class Dummy {

};

std::ostream &operator <<(std::ostream &out, Dummy const &) {
  return out << "DUMMY";
}

int main(int argc, char *argv[]) {

  uint8_t x = strAs<uint8_t>(asStr(static_cast<uint64_t>(std::numeric_limits<uint8_t>::min())));
  assert(x == std::numeric_limits<uint8_t>::min());
  if (x != std::numeric_limits<uint8_t>::min())
    throw std::runtime_error("test error");

  return 0;
}
