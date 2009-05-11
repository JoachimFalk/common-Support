#include <iostream>
#include <cmath>

#include <CoSupport/DataTypes/Value.hpp>

using namespace CoSupport::DataTypes;

int main(int argc, char *argv[]) {
  {
    Value<int> x;
    Value<int> y(10);
    Value<int> z(y);
    
    Value<double> xx;
    Value<double> yy(M_PI);
    Value<double> zz(y);
    
    xx = M_E;
    x = -1;
    
    std::cout << "x == " << x << std::endl;
    std::cout << "y == " << y << std::endl;
    std::cout << "z == " << z << std::endl;
    
    z += 13; x = y + z; y = z; z = 14;
    
    std::cout << "x == " << x << std::endl;
    std::cout << "y == " << y << std::endl;
    std::cout << "z == " << z << std::endl;
    
    std::cout << "xx == " << xx << std::endl;
    std::cout << "yy == " << yy << std::endl;
    std::cout << "zz == " << zz << std::endl;
    
    zz = 13.5; xx = y + zz; yy = zz; zz = 14.5;
    
    std::cout << "xx == " << xx << std::endl;
    std::cout << "yy == " << yy << std::endl;
    std::cout << "zz == " << zz << std::endl;
    
    for (int i = 0; i < 6; ++i) {
      x = y; y = z; z = xx; xx = yy; yy = zz; zz = zz *2;
      std::cout << "===" << std::endl;
      std::cout << "x == " << x << std::endl;
      std::cout << "y == " << y << std::endl;
      std::cout << "z == " << z << std::endl;
      std::cout << "xx == " << xx << std::endl;
      std::cout << "yy == " << yy << std::endl;
      std::cout << "zz == " << zz << std::endl;
    }
  }
  {
    Value<const char *> cp("foo");
    Value<std::string>  str1(cp);
    Value<std::string>  str2("bar");
    
    std::cout << "cp   == \"" << cp << "\"" << std::endl;
    std::cout << "str1 == \"" << str1 << "\"" << std::endl;
    std::cout << "str2 == \"" << str2 << "\"" << std::endl;
    
    cp   = "batz";
    str1 = "hix";
    str2 = std::string("hax");
    
    std::cout << "cp   == \"" << cp << "\"" << std::endl;
    std::cout << "str1 == \"" << str1 << "\"" << std::endl;
    std::cout << "str2 == \"" << str2 << "\"" << std::endl;
    
    str1 = cp;
    str2 = str1.get() + cp.get();
  }





  return 0;
}
