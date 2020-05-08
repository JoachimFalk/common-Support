#include <CoSupport/String/convert.hpp>

#include <cassert>

#include <iostream>
#include <stdint.h>

using CoSupport::String::asStr;

class Dummy {

};

std::ostream &operator <<(std::ostream &out, Dummy const &) {
  return out << "DUMMY";
}

int main(int argc, char *argv[]) {
  std::cout << asStr(Dummy()) << std::endl;
  return 0;
}
