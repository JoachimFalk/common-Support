
#include <stdlib.h>

#include <iostream>
#include <fstream>

#include <CoSupport/compatibility-glue/nullptr.h>

#include <CoSupport/Streams/DebugOStream.hpp>

namespace CS = CoSupport::Streams;

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    std::cerr << "Usage: "
      << (argv[0] != nullptr ? argv[0] : "???")
      << " testfile.txt" << std::endl;
    exit(-1);
  }
  
  std::ofstream *foo = new std::ofstream(argv[1]);

//*foo << "foo" << std::endl;
  
  CS::DebugOStream dbg(*foo);
  dbg << CS::Color::Red() << "foo" << std::endl;

//foo->close();
//foo->open("bar");
  
//*foo << "bar" << std::endl;

  dbg << CS::Color::Red() << "bar" << std::endl;

//delete foo;

  return 0;

}
