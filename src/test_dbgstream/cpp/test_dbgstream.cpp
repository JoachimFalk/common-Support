
#include <stdlib.h>

#include <iostream>

#include <CoSupport/compatibility-glue/nullptr.h>

#include <CoSupport/Streams/DebugOStream.hpp>

namespace CS = CoSupport::Streams;

int main(int argc, char *argv[]) {
  CS::DebugOStream dbg(std::cout);
  dbg << CS::Color::Red() << "foo" << std::endl;

//foo->close();
//foo->open("bar");
  
//*foo << "bar" << std::endl;

  dbg << CS::Color::Green() << "bar" << std::endl;

//delete foo;

  return 0;

}
