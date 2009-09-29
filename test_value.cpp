/*
 * Copyright (c) 2004-2009 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
 * 
 *   This library is free software; you can redistribute it and/or modify it under
 *   the terms of the GNU Lesser General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option) any
 *   later version.
 * 
 *   This library is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 *   details.
 * 
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software Foundation, Inc.,
 *   59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 * 
 * --- This software and any associated documentation is provided "as is" 
 * 
 * IN NO EVENT SHALL HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG
 * BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN
 * NUREMBERG HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG, SPECIFICALLY
 * DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
 * HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF
 * ERLANGEN NUREMBERG HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 */

#include <iostream>
#include <cassert>
#include <cmath>

#include <CoSupport/DataTypes/Value.hpp>

#define CHECK_OP_VW(X,OP,V) do { \
    std::cout << #X " " #OP " " << #V << ": " << (X OP V) << std::endl; \
    assert((X OP V) == (X OP V.get())); \
  } while (0)

#define CHECK_OP_WV(X,OP,V) do { \
    std::cout << #X " " #OP " " << #V << ": " << (X OP V) << std::endl; \
    assert((X OP V) == (X.get() OP V)); \
  } while (0)

#define CHECK_OP_WW(X,OP,V) do { \
    std::cout << #X " " #OP " " << #V << ": " << (X OP V) << std::endl; \
    assert((X OP V) == (X.get() OP V.get())); \
  } while (0)

using namespace CoSupport::DataTypes;

int main(int argc, char *argv[]) {
  {
    Value<int> x(0);
    Value<int> y(10);
    Value<int> z(y);
    
    Value<double> xx;
    Value<double> yy(M_PI);
    Value<double> zz(y);
    
    std::cout << "x(0): " << x << std::endl;
    
    assert(x == 0);
    
    x = 13;
    std::cout << "x = 13: " << x << std::endl;
    
    x += 1;
    std::cout << "x += 1: " << x << std::endl;
    std::cout << "++x:    " << ++x << std::endl;
    
    x -= 1;
    std::cout << "x -= 1: " << x << std::endl;
    std::cout << "--x:    " << --x << std::endl;
    
    std::cout << "x++:    " << x++ << std::endl;
    std::cout << "x:      " << x << std::endl;
    
    std::cout << "x--:    " << x-- << std::endl;
    std::cout << "x:      " << x << std::endl;
    
    x *= 3;
    std::cout << "x *= 3: " << x << std::endl;
    
    x /= 4;
    std::cout << "x /= 4: " << x << std::endl;
    
    x %= 5;
    std::cout << "x %= 5: " << x << std::endl;
    assert(x == 4);

    x = 4711;
    std::cout << "x = 4711: " << x << std::endl;
    CHECK_OP_WV(x,==,4710); CHECK_OP_WV(x,==,4711); CHECK_OP_WV(x,==,4712);
    CHECK_OP_WV(x,!=,4710); CHECK_OP_WV(x,!=,4711); CHECK_OP_WV(x,!=,4712);
    CHECK_OP_WV(x,< ,4710); CHECK_OP_WV(x,< ,4711); CHECK_OP_WV(x,< ,4712);
    CHECK_OP_WV(x,<=,4710); CHECK_OP_WV(x,<=,4711); CHECK_OP_WV(x,<=,4712);
    CHECK_OP_WV(x,> ,4710); CHECK_OP_WV(x,> ,4711); CHECK_OP_WV(x,> ,4712);
    CHECK_OP_WV(x,>=,4710); CHECK_OP_WV(x,>=,4711); CHECK_OP_WV(x,>=,4712);

    CHECK_OP_VW(4710,==,x); CHECK_OP_VW(4711,==,x); CHECK_OP_VW(4712,==,x);
    CHECK_OP_VW(4710,!=,x); CHECK_OP_VW(4711,!=,x); CHECK_OP_VW(4712,!=,x);
    CHECK_OP_VW(4710,< ,x); CHECK_OP_VW(4711,< ,x); CHECK_OP_VW(4712,< ,x);
    CHECK_OP_VW(4710,<=,x); CHECK_OP_VW(4711,<=,x); CHECK_OP_VW(4712,<=,x);
    CHECK_OP_VW(4710,> ,x); CHECK_OP_VW(4711,> ,x); CHECK_OP_VW(4712,> ,x);
    CHECK_OP_VW(4710,>=,x); CHECK_OP_VW(4711,>=,x); CHECK_OP_VW(4712,>=,x);
    
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
    Value<std::string>  str3(std::string("zzz"));
    
    std::cout << "cp(\"foo\"):                \"" << cp << "\"" << std::endl;
    std::cout << "str1(cp):                 \"" << str1 << "\"" << std::endl;
    std::cout << "str2(\"bar\"):              \"" << str2 << "\"" << std::endl;
    std::cout << "str3(std::string(\"zzz\")): \"" << str3 << "\"" << std::endl;
    
    CHECK_OP_WV(str1,==,std::string("foo")); CHECK_OP_WV(str1,==,std::string("bar"));
    CHECK_OP_WV(str1,!=,std::string("foo")); CHECK_OP_WV(str1,!=,std::string("bar"));
    CHECK_OP_WV(str1,< ,std::string("foo")); CHECK_OP_WV(str1,< ,std::string("bar")); CHECK_OP_WV(str1,< ,std::string("zzz"));
    CHECK_OP_WV(str1,<=,std::string("foo")); CHECK_OP_WV(str1,<=,std::string("bar")); CHECK_OP_WV(str1,<=,std::string("zzz"));
    CHECK_OP_WV(str1,> ,std::string("foo")); CHECK_OP_WV(str1,> ,std::string("bar")); CHECK_OP_WV(str1,> ,std::string("zzz"));
    CHECK_OP_WV(str1,>=,std::string("foo")); CHECK_OP_WV(str1,>=,std::string("bar")); CHECK_OP_WV(str1,>=,std::string("zzz"));

    CHECK_OP_WV(str1,==,"foo"); CHECK_OP_WV(str1,==,"bar");
    CHECK_OP_WV(str1,!=,"foo"); CHECK_OP_WV(str1,!=,"bar");
    CHECK_OP_WV(str1,< ,"foo"); CHECK_OP_WV(str1,< ,"bar"); CHECK_OP_WV(str1,< ,"zzz");
    CHECK_OP_WV(str1,<=,"foo"); CHECK_OP_WV(str1,<=,"bar"); CHECK_OP_WV(str1,<=,"zzz");
    CHECK_OP_WV(str1,> ,"foo"); CHECK_OP_WV(str1,> ,"bar"); CHECK_OP_WV(str1,> ,"zzz");
    CHECK_OP_WV(str1,>=,"foo"); CHECK_OP_WV(str1,>=,"bar"); CHECK_OP_WV(str1,>=,"zzz");

    CHECK_OP_WW(str1,==,cp); CHECK_OP_WW(str1,==,str2);
    CHECK_OP_WW(str1,!=,cp); CHECK_OP_WW(str1,!=,str2);
    CHECK_OP_WW(str1,< ,cp); CHECK_OP_WW(str1,< ,str2); CHECK_OP_WW(str1,< ,str3);
    CHECK_OP_WW(str1,<=,cp); CHECK_OP_WW(str1,<=,str2); CHECK_OP_WW(str1,<=,str3);
    CHECK_OP_WW(str1,> ,cp); CHECK_OP_WW(str1,> ,str2); CHECK_OP_WW(str1,> ,str3);
    CHECK_OP_WW(str1,>=,cp); CHECK_OP_WW(str1,>=,str2); CHECK_OP_WW(str1,>=,str3);

    CHECK_OP_WV(cp,==,std::string("aaa")); CHECK_OP_WV(cp,==,std::string("foo"));
    CHECK_OP_WV(cp,!=,std::string("aaa")); CHECK_OP_WV(cp,!=,std::string("foo"));
    CHECK_OP_WV(cp,< ,std::string("aaa")); CHECK_OP_WV(cp,< ,std::string("foo")); CHECK_OP_WV(cp,< ,std::string("zzz"));
    CHECK_OP_WV(cp,<=,std::string("aaa")); CHECK_OP_WV(cp,<=,std::string("foo")); CHECK_OP_WV(cp,<=,std::string("zzz"));
    CHECK_OP_WV(cp,> ,std::string("aaa")); CHECK_OP_WV(cp,> ,std::string("foo")); CHECK_OP_WV(cp,> ,std::string("zzz"));
    CHECK_OP_WV(cp,>=,std::string("aaa")); CHECK_OP_WV(cp,>=,std::string("foo")); CHECK_OP_WV(cp,>=,std::string("zzz"));

//  This is not really string comparison put pointer comparison. Implement this later!
//  CHECK_OP_WV(cp,==,"aaa"); CHECK_OP_WV(cp,==,"foo");
//  CHECK_OP_WV(cp,!=,"aaa"); CHECK_OP_WV(cp,!=,"foo");
//  CHECK_OP_WV(cp,< ,"aaa"); CHECK_OP_WV(cp,< ,"foo"); CHECK_OP_WV(cp,< ,"zzz");
//  CHECK_OP_WV(cp,<=,"aaa"); CHECK_OP_WV(cp,<=,"foo"); CHECK_OP_WV(cp,<=,"zzz");
//  CHECK_OP_WV(cp,> ,"aaa"); CHECK_OP_WV(cp,> ,"foo"); CHECK_OP_WV(cp,> ,"zzz");
//  CHECK_OP_WV(cp,>=,"aaa"); CHECK_OP_WV(cp,>=,"foo"); CHECK_OP_WV(cp,>=,"zzz");
    
    str1 = "aaa"; str2 = "foo"; str3 = "zzz";
    std::cout << "str1 = \"aaa\": \"" << str1 << "\"" << std::endl;
    std::cout << "str2 = \"foo\": \"" << str2 << "\"" << std::endl;
    std::cout << "str3 = \"zzz\": \"" << str3 << "\"" << std::endl;
    
    CHECK_OP_WW(cp,==,str1); CHECK_OP_WW(cp,==,str2);
    CHECK_OP_WW(cp,!=,str1); CHECK_OP_WW(cp,!=,str2);
    CHECK_OP_WW(cp,< ,str1); CHECK_OP_WW(cp,< ,str2); CHECK_OP_WW(cp,< ,str3);
    CHECK_OP_WW(cp,<=,str1); CHECK_OP_WW(cp,<=,str2); CHECK_OP_WW(cp,<=,str3);
    CHECK_OP_WW(cp,> ,str1); CHECK_OP_WW(cp,> ,str2); CHECK_OP_WW(cp,> ,str3);
    CHECK_OP_WW(cp,>=,str1); CHECK_OP_WW(cp,>=,str2); CHECK_OP_WW(cp,>=,str3);
    
    cp   = "batz";
    str1 = "hix";
    str2 = std::string("hax");
    
    std::cout << "cp = \"batz\":               \"" << cp << "\"" << std::endl;
    std::cout << "str1 = \"hix\":              \"" << str1 << "\"" << std::endl;
    std::cout << "str2 = std::string(\"hax\"): \"" << str2 << "\"" << std::endl;
    
    str1 = cp;
    str2 = str1.get() + cp.get();
  }
  return 0;
}
