#include "config.h"

#include <CoSupport/Allocators/IdPool.hpp>

#include <map>
#include <iostream>

using namespace CoSupport::Allocators;

class foo {};

IdPool<std::map<uint64_t, foo *> > idPool;

int main(int argc, char *argv[]) {
  for (size_t i = 0; i < 15; ++i) {
    std::cout << "idAlloc => " << idPool.idAlloc(NULL)->first << std::endl;
  }
  idPool.idFree(7);  std::cout << "idFree(7)" << std::endl;
  idPool.idFree(3);  std::cout << "idFree(3)" << std::endl;
  idPool.idFree(13); std::cout << "idFree(13)" << std::endl;
  for (size_t i = 0; i < 15; ++i) {
    std::cout << "idAlloc => " << idPool.idAlloc(NULL)->first << std::endl;
  }

  return 0;
}
