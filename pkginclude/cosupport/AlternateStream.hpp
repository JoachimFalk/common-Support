#ifndef _INCLUDED_COSUPPORT_ALTERNATESTREAM_HPP
#define _INCLUDED_COSUPPORT_ALTERNATESTREAM_HPP

#include <ostream>
#include <istream>
#include <fstream>
#include <string>

namespace CoSupport {
  
  struct FileNotOpenException {};

  template<class Base, class FStream>
  struct AlternateStream :
    public Base
  {
    Base* obj;
    Base* def;
    
    AlternateStream(Base& _def, const std::string& file, const std::string& cmp) :
      Base(0), def(&_def)
    {
      if(file == cmp)
        obj = def;
      else {
        FStream* fs = new FStream(file.c_str());
        if(!fs->is_open())
          throw FileNotOpenException();
        obj = fs;
      }
      rdbuf(obj->rdbuf());
    }
    
    ~AlternateStream()
    {
      if(obj != def)
        delete obj;
    }
  };
  
  typedef AlternateStream<std::ostream, std::ofstream> AOStream;
  typedef AlternateStream<std::istream, std::ifstream> AIStream;
  
} // namespace CoSupport

#endif // _INCLUDED_COSUPPORT_ALTERNATESTREAM_HPP
