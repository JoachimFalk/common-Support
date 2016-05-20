/*
 * Copyright (c) 2004-2013 Hardware-Software-CoDesign, University of
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
#include <string>
#include <cassert>
#include <cmath>

#include <CoSupport/compatibility-glue/nullptr.h>

#include <CoSupport/DataTypes/ValueInterface.hpp>
#include <CoSupport/DataTypes/ValueVirtual.hpp>
#include <CoSupport/DataTypes/ValueFacade.hpp>

/// This class implements the interface for a storage which contains a value of type T.
/// \example test_value.cpp
template <
  class T,
  class CR = typename boost::add_reference<typename boost::add_const<T>::type>::type
>
class Value: public CoSupport::DataTypes::ValueInterface<Value<T,CR>,T,CR> {
  typedef Value<T,CR>                                           this_type;
  typedef CoSupport::DataTypes::ValueInterface<this_type,T,CR>  base_type;

  friend class CoSupport::DataTypes::ValueInterface<this_type,T,CR>;
private:
  T value;
protected:
  void implSet(const T &val)
    { value = val; }
  T const &implGet() const
    { return value; }
public:
  Value() {}
  Value(T const &val)
    : value(val) {}
  template <class DD, typename TT, typename CRCR>
  Value(CoSupport::DataTypes::ValueInterface<DD,TT,CRCR> const &val)
    : value(val.get()) {}

  using base_type::operator =;
#ifdef _MSC_VER
  // Visual Studio can't pull the following template assignment operator from base_type.
  // Hence, we replicate it here.
  template <class DD, typename TT, typename CRCR>
  this_type &operator = (const ValueInterface<DD, TT, CRCR> &val)
    { this->set(val); return *this; }
#endif //_MSC_VER
};

#define CHECK_OP_VW(X,OP,V) do { \
    std::cout << #X " " #OP " " << #V << ": " << (X OP V) << std::endl; \
    assert((X OP V) == (X OP (V).get())); \
  } while (0)

#define CHECK_OP_WV(X,OP,V) do { \
    std::cout << #X " " #OP " " << #V << ": " << (X OP V) << std::endl; \
    assert((X OP V) == ((X).get() OP V)); \
  } while (0)

#define CHECK_OP_WW(X,OP,V) do { \
    std::cout << #X " " #OP " " << #V << ": " << (X OP V) << std::endl; \
    assert((X OP V) == ((X).get() OP (V).get())); \
  } while (0)

struct A {
  int a;
};

struct B: public A {
  int b;
};

struct C: public A {
  int c;
};

volatile int sink;

using namespace CoSupport::DataTypes;

int main(int argc, char *argv[]) {
  {
    Value<unsigned long>        x(0);
    Value<unsigned short>       y(10);
    ValueVirtual<unsigned int>  z(y);
    
    ValueVirtual<double> xx;
    Value<double>        yy(3.14159f);
    Value<double>        zz(y);
    
    std::cout << "x(0): " << x << std::endl;
    
    assert(0ul == x);
    assert(0u == x);
    assert(x == 0ul);
    assert(x == 0u);
    
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
    assert(x == 4u);

    x = 4711;
    std::cout << "x = 4711: " << x << std::endl;
    CHECK_OP_WV(x,==,4710u); CHECK_OP_WV(x,==,4711ul); CHECK_OP_WV(x,==,4712u);
    CHECK_OP_WV(x,!=,4710u); CHECK_OP_WV(x,!=,4711ul); CHECK_OP_WV(x,!=,4712u);
    CHECK_OP_WV(x,< ,4710u); CHECK_OP_WV(x,< ,4711ul); CHECK_OP_WV(x,< ,4712u);
    CHECK_OP_WV(x,<=,4710u); CHECK_OP_WV(x,<=,4711ul); CHECK_OP_WV(x,<=,4712u);
    CHECK_OP_WV(x,> ,4710u); CHECK_OP_WV(x,> ,4711ul); CHECK_OP_WV(x,> ,4712u);
    CHECK_OP_WV(x,>=,4710u); CHECK_OP_WV(x,>=,4711ul); CHECK_OP_WV(x,>=,4712u);

    CHECK_OP_VW(4710ul,==,x); CHECK_OP_VW(4711u,==,x); CHECK_OP_VW(4712ul,==,x);
    CHECK_OP_VW(4710ul,!=,x); CHECK_OP_VW(4711u,!=,x); CHECK_OP_VW(4712ul,!=,x);
    CHECK_OP_VW(4710ul,< ,x); CHECK_OP_VW(4711u,< ,x); CHECK_OP_VW(4712ul,< ,x);
    CHECK_OP_VW(4710ul,<=,x); CHECK_OP_VW(4711u,<=,x); CHECK_OP_VW(4712ul,<=,x);
    CHECK_OP_VW(4710ul,> ,x); CHECK_OP_VW(4711u,> ,x); CHECK_OP_VW(4712ul,> ,x);
    CHECK_OP_VW(4710ul,>=,x); CHECK_OP_VW(4711u,>=,x); CHECK_OP_VW(4712ul,>=,x);

    y = 4710; z = 4712;

    CHECK_OP_VW(y,==,x); CHECK_OP_VW(x,==,x); CHECK_OP_VW(z,==,x);
    CHECK_OP_VW(y,!=,x); CHECK_OP_VW(x,!=,x); CHECK_OP_VW(z,!=,x);
    CHECK_OP_VW(y,< ,x); CHECK_OP_VW(x,< ,x); CHECK_OP_VW(z,< ,x);
    CHECK_OP_VW(y,<=,x); CHECK_OP_VW(x,<=,x); CHECK_OP_VW(z,<=,x);
    CHECK_OP_VW(y,> ,x); CHECK_OP_VW(x,> ,x); CHECK_OP_VW(z,> ,x);
    CHECK_OP_VW(y,>=,x); CHECK_OP_VW(x,>=,x); CHECK_OP_VW(z,>=,x);

    CHECK_OP_VW(x,==,y); CHECK_OP_VW(x,==,z); CHECK_OP_VW(z,==,y);
    CHECK_OP_VW(x,!=,y); CHECK_OP_VW(x,!=,z); CHECK_OP_VW(z,!=,y);
    CHECK_OP_VW(x,< ,y); CHECK_OP_VW(x,< ,z); CHECK_OP_VW(z,< ,y);
    CHECK_OP_VW(x,<=,y); CHECK_OP_VW(x,<=,z); CHECK_OP_VW(z,<=,y);
    CHECK_OP_VW(x,> ,y); CHECK_OP_VW(x,> ,z); CHECK_OP_VW(z,> ,y);
    CHECK_OP_VW(x,>=,y); CHECK_OP_VW(x,>=,z); CHECK_OP_VW(z,>=,y);
    
    xx = 2.71828f;
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
    char       foo[] = "foo";
    char const bar[] = "bar";
    char       zzz[] = "zzz";

    Value<char *>       cp(foo);
    Value<const char *> ccp(foo);
    Value<std::string>  str1(ccp);
    Value<std::string>  str2("bar");
    Value<std::string>  str3(std::string("zzz"));

    ccp = cp;
    
    std::cout << "cp(\"foo\"):                \"" << cp << "\"" << std::endl;
    std::cout << "ccp(\"foo\"):               \"" << ccp << "\"" << std::endl;
    std::cout << "str1(ccp):                  \"" << str1 << "\"" << std::endl;
    std::cout << "str2(\"bar\"):              \"" << str2 << "\"" << std::endl;
    std::cout << "str3(std::string(\"zzz\")): \"" << str3 << "\"" << std::endl;
    
    CHECK_OP_WV(cp,==,std::string("foo")); CHECK_OP_WV(cp,==,std::string("bar"));
    CHECK_OP_WV(cp,!=,std::string("foo")); CHECK_OP_WV(cp,!=,std::string("bar"));
    CHECK_OP_WV(cp,< ,std::string("foo")); CHECK_OP_WV(cp,< ,std::string("bar")); CHECK_OP_WV(cp,< ,std::string("zzz"));
    CHECK_OP_WV(cp,<=,std::string("foo")); CHECK_OP_WV(cp,<=,std::string("bar")); CHECK_OP_WV(cp,<=,std::string("zzz"));
    CHECK_OP_WV(cp,> ,std::string("foo")); CHECK_OP_WV(cp,> ,std::string("bar")); CHECK_OP_WV(cp,> ,std::string("zzz"));
    CHECK_OP_WV(cp,>=,std::string("foo")); CHECK_OP_WV(cp,>=,std::string("bar")); CHECK_OP_WV(cp,>=,std::string("zzz"));

    CHECK_OP_VW(std::string("foo"),==,cp); CHECK_OP_VW(std::string("bar"),==,cp);
    CHECK_OP_VW(std::string("foo"),!=,cp); CHECK_OP_VW(std::string("bar"),!=,cp);
    CHECK_OP_VW(std::string("foo"),< ,cp); CHECK_OP_VW(std::string("bar"),< ,cp); CHECK_OP_VW(std::string("zzz"),< ,cp);
    CHECK_OP_VW(std::string("foo"),<=,cp); CHECK_OP_VW(std::string("bar"),<=,cp); CHECK_OP_VW(std::string("zzz"),<=,cp);
    CHECK_OP_VW(std::string("foo"),> ,cp); CHECK_OP_VW(std::string("bar"),> ,cp); CHECK_OP_VW(std::string("zzz"),> ,cp);
    CHECK_OP_VW(std::string("foo"),>=,cp); CHECK_OP_VW(std::string("bar"),>=,cp); CHECK_OP_VW(std::string("zzz"),>=,cp);

    CHECK_OP_WV(cp,==,foo); CHECK_OP_WV(cp,==,bar);
    CHECK_OP_WV(cp,!=,foo); CHECK_OP_WV(cp,!=,bar);
    CHECK_OP_WV(cp,< ,foo); CHECK_OP_WV(cp,< ,bar); CHECK_OP_WV(cp,< ,zzz);
    CHECK_OP_WV(cp,<=,foo); CHECK_OP_WV(cp,<=,bar); CHECK_OP_WV(cp,<=,zzz);
    CHECK_OP_WV(cp,> ,foo); CHECK_OP_WV(cp,> ,bar); CHECK_OP_WV(cp,> ,zzz);
    CHECK_OP_WV(cp,>=,foo); CHECK_OP_WV(cp,>=,bar); CHECK_OP_WV(cp,>=,zzz);

    CHECK_OP_VW(foo,==,cp); CHECK_OP_VW(bar,==,cp);
    CHECK_OP_VW(foo,!=,cp); CHECK_OP_VW(bar,!=,cp);
    CHECK_OP_VW(foo,< ,cp); CHECK_OP_VW(bar,< ,cp); CHECK_OP_VW(zzz,< ,cp);
    CHECK_OP_VW(foo,<=,cp); CHECK_OP_VW(bar,<=,cp); CHECK_OP_VW(zzz,<=,cp);
    CHECK_OP_VW(foo,> ,cp); CHECK_OP_VW(bar,> ,cp); CHECK_OP_VW(zzz,> ,cp);
    CHECK_OP_VW(foo,>=,cp); CHECK_OP_VW(bar,>=,cp); CHECK_OP_VW(zzz,>=,cp);

    CHECK_OP_WV(ccp,==,std::string("foo")); CHECK_OP_WV(ccp,==,std::string("bar"));
    CHECK_OP_WV(ccp,!=,std::string("foo")); CHECK_OP_WV(ccp,!=,std::string("bar"));
    CHECK_OP_WV(ccp,< ,std::string("foo")); CHECK_OP_WV(ccp,< ,std::string("bar")); CHECK_OP_WV(ccp,< ,std::string("zzz"));
    CHECK_OP_WV(ccp,<=,std::string("foo")); CHECK_OP_WV(ccp,<=,std::string("bar")); CHECK_OP_WV(ccp,<=,std::string("zzz"));
    CHECK_OP_WV(ccp,> ,std::string("foo")); CHECK_OP_WV(ccp,> ,std::string("bar")); CHECK_OP_WV(ccp,> ,std::string("zzz"));
    CHECK_OP_WV(ccp,>=,std::string("foo")); CHECK_OP_WV(ccp,>=,std::string("bar")); CHECK_OP_WV(ccp,>=,std::string("zzz"));

    CHECK_OP_VW(std::string("foo"),==,ccp); CHECK_OP_VW(std::string("bar"),==,ccp);
    CHECK_OP_VW(std::string("foo"),!=,ccp); CHECK_OP_VW(std::string("bar"),!=,ccp);
    CHECK_OP_VW(std::string("foo"),< ,ccp); CHECK_OP_VW(std::string("bar"),< ,ccp); CHECK_OP_VW(std::string("zzz"),< ,ccp);
    CHECK_OP_VW(std::string("foo"),<=,ccp); CHECK_OP_VW(std::string("bar"),<=,ccp); CHECK_OP_VW(std::string("zzz"),<=,ccp);
    CHECK_OP_VW(std::string("foo"),> ,ccp); CHECK_OP_VW(std::string("bar"),> ,ccp); CHECK_OP_VW(std::string("zzz"),> ,ccp);
    CHECK_OP_VW(std::string("foo"),>=,ccp); CHECK_OP_VW(std::string("bar"),>=,ccp); CHECK_OP_VW(std::string("zzz"),>=,ccp);

    CHECK_OP_WV(ccp,==,foo); CHECK_OP_WV(ccp,==,bar);
    CHECK_OP_WV(ccp,!=,foo); CHECK_OP_WV(ccp,!=,bar);
    CHECK_OP_WV(ccp,< ,foo); CHECK_OP_WV(ccp,< ,bar); CHECK_OP_WV(ccp,< ,zzz);
    CHECK_OP_WV(ccp,<=,foo); CHECK_OP_WV(ccp,<=,bar); CHECK_OP_WV(ccp,<=,zzz);
    CHECK_OP_WV(ccp,> ,foo); CHECK_OP_WV(ccp,> ,bar); CHECK_OP_WV(ccp,> ,zzz);
    CHECK_OP_WV(ccp,>=,foo); CHECK_OP_WV(ccp,>=,bar); CHECK_OP_WV(ccp,>=,zzz);

    CHECK_OP_VW(foo,==,ccp); CHECK_OP_VW(bar,==,ccp);
    CHECK_OP_VW(foo,!=,ccp); CHECK_OP_VW(bar,!=,ccp);
    CHECK_OP_VW(foo,< ,ccp); CHECK_OP_VW(bar,< ,ccp); CHECK_OP_VW(zzz,< ,ccp);
    CHECK_OP_VW(foo,<=,ccp); CHECK_OP_VW(bar,<=,ccp); CHECK_OP_VW(zzz,<=,ccp);
    CHECK_OP_VW(foo,> ,ccp); CHECK_OP_VW(bar,> ,ccp); CHECK_OP_VW(zzz,> ,ccp);
    CHECK_OP_VW(foo,>=,ccp); CHECK_OP_VW(bar,>=,ccp); CHECK_OP_VW(zzz,>=,ccp);

    CHECK_OP_WV(str1,==,std::string("foo")); CHECK_OP_WV(str1,==,std::string("bar"));
    CHECK_OP_WV(str1,!=,std::string("foo")); CHECK_OP_WV(str1,!=,std::string("bar"));
    CHECK_OP_WV(str1,< ,std::string("foo")); CHECK_OP_WV(str1,< ,std::string("bar")); CHECK_OP_WV(str1,< ,std::string("zzz"));
    CHECK_OP_WV(str1,<=,std::string("foo")); CHECK_OP_WV(str1,<=,std::string("bar")); CHECK_OP_WV(str1,<=,std::string("zzz"));
    CHECK_OP_WV(str1,> ,std::string("foo")); CHECK_OP_WV(str1,> ,std::string("bar")); CHECK_OP_WV(str1,> ,std::string("zzz"));
    CHECK_OP_WV(str1,>=,std::string("foo")); CHECK_OP_WV(str1,>=,std::string("bar")); CHECK_OP_WV(str1,>=,std::string("zzz"));

    CHECK_OP_VW(std::string("foo"),==,str1); CHECK_OP_VW(std::string("bar"),==,str1);
    CHECK_OP_VW(std::string("foo"),!=,str1); CHECK_OP_VW(std::string("bar"),!=,str1);
    CHECK_OP_VW(std::string("foo"),< ,str1); CHECK_OP_VW(std::string("bar"),< ,str1); CHECK_OP_VW(std::string("zzz"),< ,str1);
    CHECK_OP_VW(std::string("foo"),<=,str1); CHECK_OP_VW(std::string("bar"),<=,str1); CHECK_OP_VW(std::string("zzz"),<=,str1);
    CHECK_OP_VW(std::string("foo"),> ,str1); CHECK_OP_VW(std::string("bar"),> ,str1); CHECK_OP_VW(std::string("zzz"),> ,str1);
    CHECK_OP_VW(std::string("foo"),>=,str1); CHECK_OP_VW(std::string("bar"),>=,str1); CHECK_OP_VW(std::string("zzz"),>=,str1);

    CHECK_OP_WV(str1,==,"foo"); CHECK_OP_WV(str1,==,"bar");
    CHECK_OP_WV(str1,!=,"foo"); CHECK_OP_WV(str1,!=,"bar");
    CHECK_OP_WV(str1,< ,"foo"); CHECK_OP_WV(str1,< ,"bar"); CHECK_OP_WV(str1,< ,"zzz");
    CHECK_OP_WV(str1,<=,"foo"); CHECK_OP_WV(str1,<=,"bar"); CHECK_OP_WV(str1,<=,"zzz");
    CHECK_OP_WV(str1,> ,"foo"); CHECK_OP_WV(str1,> ,"bar"); CHECK_OP_WV(str1,> ,"zzz");
    CHECK_OP_WV(str1,>=,"foo"); CHECK_OP_WV(str1,>=,"bar"); CHECK_OP_WV(str1,>=,"zzz");

    CHECK_OP_VW("foo",==,str1); CHECK_OP_VW("bar",==,str1);
    CHECK_OP_VW("foo",!=,str1); CHECK_OP_VW("bar",!=,str1);
    CHECK_OP_VW("foo",< ,str1); CHECK_OP_VW("bar",< ,str1); CHECK_OP_VW("zzz",< ,str1);
    CHECK_OP_VW("foo",<=,str1); CHECK_OP_VW("bar",<=,str1); CHECK_OP_VW("zzz",<=,str1);
    CHECK_OP_VW("foo",> ,str1); CHECK_OP_VW("bar",> ,str1); CHECK_OP_VW("zzz",> ,str1);
    CHECK_OP_VW("foo",>=,str1); CHECK_OP_VW("bar",>=,str1); CHECK_OP_VW("zzz",>=,str1);

    CHECK_OP_WW(str2,==,cp); CHECK_OP_WW(str3,==,ccp); CHECK_OP_WW(str1,==,str2);
    CHECK_OP_WW(str2,!=,cp); CHECK_OP_WW(str3,!=,ccp); CHECK_OP_WW(str1,!=,str2);
    CHECK_OP_WW(str2,< ,cp); CHECK_OP_WW(str3,< ,ccp); CHECK_OP_WW(str1,< ,str2);
    CHECK_OP_WW(str2,<=,cp); CHECK_OP_WW(str3,<=,ccp); CHECK_OP_WW(str1,<=,str2);
    CHECK_OP_WW(str2,> ,cp); CHECK_OP_WW(str3,> ,ccp); CHECK_OP_WW(str1,> ,str2);
    CHECK_OP_WW(str2,>=,cp); CHECK_OP_WW(str3,>=,ccp); CHECK_OP_WW(str1,>=,str2);

    CHECK_OP_WW(cp,==,str2); CHECK_OP_WW(ccp,==,str3);
    CHECK_OP_WW(cp,!=,str2); CHECK_OP_WW(ccp,!=,str3);
    CHECK_OP_WW(cp,< ,str2); CHECK_OP_WW(ccp,< ,str3);
    CHECK_OP_WW(cp,<=,str2); CHECK_OP_WW(ccp,<=,str3);
    CHECK_OP_WW(cp,> ,str2); CHECK_OP_WW(ccp,> ,str3);
    CHECK_OP_WW(cp,>=,str2); CHECK_OP_WW(ccp,>=,str3);
    
    str1 = "aaa"; str2 = "foo"; str3 = "zzz";
    std::cout << "str1 = \"aaa\": \"" << str1 << "\"" << std::endl;
    std::cout << "str2 = \"foo\": \"" << str2 << "\"" << std::endl;
    std::cout << "str3 = \"zzz\": \"" << str3 << "\"" << std::endl;

    char       batz[] = "batz";
  
    cp  = batz;
    ccp = batz;

    assert(*ccp++ == 'b');
    assert(*ccp-- == 'a');
    assert(*++ccp == 'a');
    assert(*--ccp == 'b');
    assert(*(ccp += 2) == 't');
    assert(*(ccp -= 1) == 'a');
    assert(ccp-ccp == 0);
    assert(ccp-cp == 1);
    assert(cp[0] == 'b');
    assert(cp[3] == 'z');
    assert(ccp[-1] == 'b');

    str1 = "hix";
    str2 = std::string("hax");
    
    std::cout << "ccp = \"batz\":               \"" << ccp << "\"" << std::endl;
    std::cout << "str1 = \"hix\":              \"" << str1 << "\"" << std::endl;
    std::cout << "str2 = std::string(\"hax\"): \"" << str2 << "\"" << std::endl;
    
    str1 = ccp;
    str2 = str1.get() + ccp.get();
  }
  {
    char aaa[] = "aaa";
    char foo[] = "foo";
    char caaa[] = "aaa";
    char cfoo[] = "foo";
    
    Value<const char *> ccp(cfoo);
    Value<char *>       cp(foo);
    
    // This is not really string comparison put pointer comparison.
    CHECK_OP_WV(ccp,==,aaa); CHECK_OP_VW(foo,==,ccp);
    CHECK_OP_WV(ccp,!=,aaa); CHECK_OP_VW(foo,!=,ccp);
    CHECK_OP_WV(ccp,< ,aaa); CHECK_OP_VW(foo,< ,ccp);
    CHECK_OP_WV(ccp,<=,aaa); CHECK_OP_VW(foo,<=,ccp);
    CHECK_OP_WV(ccp,> ,aaa); CHECK_OP_VW(foo,> ,ccp);
    CHECK_OP_WV(ccp,>=,aaa); CHECK_OP_VW(foo,>=,ccp);

    CHECK_OP_WV(ccp,==,caaa); CHECK_OP_VW(cfoo,==,ccp);
    CHECK_OP_WV(ccp,!=,caaa); CHECK_OP_VW(cfoo,!=,ccp);
    CHECK_OP_WV(ccp,< ,caaa); CHECK_OP_VW(cfoo,< ,ccp);
    CHECK_OP_WV(ccp,<=,caaa); CHECK_OP_VW(cfoo,<=,ccp);
    CHECK_OP_WV(ccp,> ,caaa); CHECK_OP_VW(cfoo,> ,ccp);
    CHECK_OP_WV(ccp,>=,caaa); CHECK_OP_VW(cfoo,>=,ccp);

    CHECK_OP_WV(cp,==,aaa); CHECK_OP_VW(foo,==,cp);
    CHECK_OP_WV(cp,!=,aaa); CHECK_OP_VW(foo,!=,cp);
    CHECK_OP_WV(cp,< ,aaa); CHECK_OP_VW(foo,< ,cp);
    CHECK_OP_WV(cp,<=,aaa); CHECK_OP_VW(foo,<=,cp);
    CHECK_OP_WV(cp,> ,aaa); CHECK_OP_VW(foo,> ,cp);
    CHECK_OP_WV(cp,>=,aaa); CHECK_OP_VW(foo,>=,cp);

    CHECK_OP_WV(cp,==,caaa); CHECK_OP_VW(cfoo,==,cp);
    CHECK_OP_WV(cp,!=,caaa); CHECK_OP_VW(cfoo,!=,cp);
    CHECK_OP_WV(cp,< ,caaa); CHECK_OP_VW(cfoo,< ,cp);
    CHECK_OP_WV(cp,<=,caaa); CHECK_OP_VW(cfoo,<=,cp);
    CHECK_OP_WV(cp,> ,caaa); CHECK_OP_VW(cfoo,> ,cp);
    CHECK_OP_WV(cp,>=,caaa); CHECK_OP_VW(cfoo,>=,cp);
  }
  {
    A a;
    B b;
    C c;

    Value<A *>                  pan(nullptr);
    ValueVirtual<A *>           pa1(&a);
    Value<B *>                  pb1(&b);
    ValueVirtualInterface<C *> *pc1Impl(new Detail::ValueVirtualImpl<C *>(&c));
    ValueVirtual<C *>           pc1(pc1Impl);
    
    sink = pa1->a + pb1->a + pb1->b + pc1->a + pc1->c;
    sink = (*pa1).a + (*pb1).a + (*pb1).b + (*pc1).a + (*pc1).c;

    assert(pc1Impl->implGet() == pc1);

    CHECK_OP_VW(nullptr,==,pan); CHECK_OP_WV(pan,==,nullptr);
    CHECK_OP_VW(nullptr,!=,pan); CHECK_OP_WV(pan,!=,nullptr);
    CHECK_OP_VW(nullptr,==,pa1); CHECK_OP_WV(pa1,==,nullptr);
    CHECK_OP_VW(nullptr,!=,pa1); CHECK_OP_WV(pa1,!=,nullptr);
    CHECK_OP_VW(nullptr,==,pb1); CHECK_OP_WV(pb1,==,nullptr);
    CHECK_OP_VW(nullptr,!=,pb1); CHECK_OP_WV(pb1,!=,nullptr);
    CHECK_OP_VW(nullptr,==,pc1); CHECK_OP_WV(pc1,==,nullptr);
    CHECK_OP_VW(nullptr,!=,pc1); CHECK_OP_WV(pc1,!=,nullptr);


    CHECK_OP_WW(pa1,==,pa1); CHECK_OP_WW(pb1,==,pb1); CHECK_OP_WW(pc1,==,pc1);
    CHECK_OP_WW(pa1,!=,pa1); CHECK_OP_WW(pb1,!=,pb1); CHECK_OP_WW(pc1,!=,pc1);
    CHECK_OP_WW(pa1,< ,pa1); CHECK_OP_WW(pb1,< ,pb1); CHECK_OP_WW(pc1,< ,pc1);
    CHECK_OP_WW(pa1,<=,pa1); CHECK_OP_WW(pb1,<=,pb1); CHECK_OP_WW(pc1,<=,pc1);
    CHECK_OP_WW(pa1,> ,pa1); CHECK_OP_WW(pb1,> ,pb1); CHECK_OP_WW(pc1,> ,pc1);
    CHECK_OP_WW(pa1,>=,pa1); CHECK_OP_WW(pb1,>=,pb1); CHECK_OP_WW(pc1,>=,pc1);

    
    CHECK_OP_WW(pa1,==,pan); CHECK_OP_WW(pa1,==,pb1); CHECK_OP_WW(pa1,==,pc1);
    CHECK_OP_WW(pa1,!=,pan); CHECK_OP_WW(pa1,!=,pb1); CHECK_OP_WW(pa1,!=,pc1);
    CHECK_OP_WW(pa1,< ,pan); CHECK_OP_WW(pa1,< ,pb1); CHECK_OP_WW(pa1,< ,pc1);
    CHECK_OP_WW(pa1,<=,pan); CHECK_OP_WW(pa1,<=,pb1); CHECK_OP_WW(pa1,<=,pc1);
    CHECK_OP_WW(pa1,> ,pan); CHECK_OP_WW(pa1,> ,pb1); CHECK_OP_WW(pa1,> ,pc1);
    CHECK_OP_WW(pa1,>=,pan); CHECK_OP_WW(pa1,>=,pb1); CHECK_OP_WW(pa1,>=,pc1);

    CHECK_OP_WW(pa1,==,pa1); CHECK_OP_WW(pb1,==,pb1); CHECK_OP_WW(pc1,==,pc1);
    CHECK_OP_WW(pa1,!=,pa1); CHECK_OP_WW(pb1,!=,pb1); CHECK_OP_WW(pc1,!=,pc1);
    CHECK_OP_WW(pa1,< ,pa1); CHECK_OP_WW(pb1,< ,pb1); CHECK_OP_WW(pc1,< ,pc1);
    CHECK_OP_WW(pa1,<=,pa1); CHECK_OP_WW(pb1,<=,pb1); CHECK_OP_WW(pc1,<=,pc1);
    CHECK_OP_WW(pa1,> ,pa1); CHECK_OP_WW(pb1,> ,pb1); CHECK_OP_WW(pc1,> ,pc1);
    CHECK_OP_WW(pa1,>=,pa1); CHECK_OP_WW(pb1,>=,pb1); CHECK_OP_WW(pc1,>=,pc1);
  }
  {
    ValueFacade<int>            a(12);
    ValueFacade<int>::Ref       ra(a);
    ValueFacade<int>::ConstRef  cra1(a);
    ValueFacade<int>::ConstRef  cra2(ra);
    ValueFacade<int>::Ptr       pa(a.toPtr());
    ValueFacade<int>::ConstPtr  cpa1(pa);
    ValueFacade<int>::ConstPtr  cpa2(a.toPtr());
    
    ValueFacadeInterface<int>  *bImpl = new Detail::ValueFacadeImpl<int>(15);
    ValueFacade<int>            b(bImpl);
    ValueFacade<int>::Ref       rb(b);
    ValueFacade<int>::ConstRef  crb1(b);
    ValueFacade<int>::ConstRef  crb2(rb);
    ValueFacade<int>::Ptr       pb(b.toPtr());
    ValueFacade<int>::ConstPtr  cpb1(pb);
    ValueFacade<int>::ConstPtr  cpb2(b.toPtr());
    
    Value<int>        c(14);
    ValueVirtual<int> d(13);
    
    assert(a == 12); sassert(a++ == 12);
    assert(a == 13); assert(ra == 13); assert(cra2 == 13); assert(*cpa1 == 13); assert(*cpa2 == 13);
    
    assert(bImpl->implGet() == 15);
    assert(b == 15); sassert(b-- == 15);
    assert(b == 14); assert(rb == 14); assert(crb2 == 14); assert(*cpb1 == 14); assert(*cpb2 == 14);
    assert(bImpl->implGet() == 14);
    
    sassert(++c == 15); sassert(d++ == 13); assert(c == 15); assert(d == 14);
    
    CHECK_OP_WW(a,==,ra); CHECK_OP_WW(a,==,cra1); CHECK_OP_WW(a,==,*pa); CHECK_OP_WW(a,==,*cpa1);
    CHECK_OP_WW(a,!=,ra); CHECK_OP_WW(a,!=,cra1); CHECK_OP_WW(a,!=,*pa); CHECK_OP_WW(a,!=,*cpa1);
    CHECK_OP_WW(a,< ,ra); CHECK_OP_WW(a,< ,cra1); CHECK_OP_WW(a,< ,*pa); CHECK_OP_WW(a,< ,*cpa1);
    CHECK_OP_WW(a,<=,ra); CHECK_OP_WW(a,<=,cra1); CHECK_OP_WW(a,<=,*pa); CHECK_OP_WW(a,<=,*cpa1);
    CHECK_OP_WW(a,> ,ra); CHECK_OP_WW(a,> ,cra1); CHECK_OP_WW(a,> ,*pa); CHECK_OP_WW(a,> ,*cpa1);
    CHECK_OP_WW(a,>=,ra); CHECK_OP_WW(a,>=,cra1); CHECK_OP_WW(a,>=,*pa); CHECK_OP_WW(a,>=,*cpa1);
    
    CHECK_OP_WW(c,==,ra); CHECK_OP_WW(c,==,cra1); CHECK_OP_WW(c,==,*pa); CHECK_OP_WW(c,==,*cpa1);
    CHECK_OP_WW(c,!=,ra); CHECK_OP_WW(c,!=,cra1); CHECK_OP_WW(c,!=,*pa); CHECK_OP_WW(c,!=,*cpa1);
    CHECK_OP_WW(c,< ,ra); CHECK_OP_WW(c,< ,cra1); CHECK_OP_WW(c,< ,*pa); CHECK_OP_WW(c,< ,*cpa1);
    CHECK_OP_WW(c,<=,ra); CHECK_OP_WW(c,<=,cra1); CHECK_OP_WW(c,<=,*pa); CHECK_OP_WW(c,<=,*cpa1);
    CHECK_OP_WW(c,> ,ra); CHECK_OP_WW(c,> ,cra1); CHECK_OP_WW(c,> ,*pa); CHECK_OP_WW(c,> ,*cpa1);
    CHECK_OP_WW(c,>=,ra); CHECK_OP_WW(c,>=,cra1); CHECK_OP_WW(c,>=,*pa); CHECK_OP_WW(c,>=,*cpa1);
    
    CHECK_OP_WW(d,==,ra); CHECK_OP_WW(d,==,cra1); CHECK_OP_WW(d,==,*pa); CHECK_OP_WW(d,==,*cpa1);
    CHECK_OP_WW(d,!=,ra); CHECK_OP_WW(d,!=,cra1); CHECK_OP_WW(d,!=,*pa); CHECK_OP_WW(d,!=,*cpa1);
    CHECK_OP_WW(d,< ,ra); CHECK_OP_WW(d,< ,cra1); CHECK_OP_WW(d,< ,*pa); CHECK_OP_WW(d,< ,*cpa1);
    CHECK_OP_WW(d,<=,ra); CHECK_OP_WW(d,<=,cra1); CHECK_OP_WW(d,<=,*pa); CHECK_OP_WW(d,<=,*cpa1);
    CHECK_OP_WW(d,> ,ra); CHECK_OP_WW(d,> ,cra1); CHECK_OP_WW(d,> ,*pa); CHECK_OP_WW(d,> ,*cpa1);
    CHECK_OP_WW(d,>=,ra); CHECK_OP_WW(d,>=,cra1); CHECK_OP_WW(d,>=,*pa); CHECK_OP_WW(d,>=,*cpa1);
    
    CHECK_OP_WW(ra,==,a); CHECK_OP_WW(cra1,==,a); CHECK_OP_WW(*pa,==,a); CHECK_OP_WW(*cpa1,==,a);
    CHECK_OP_WW(ra,!=,a); CHECK_OP_WW(cra1,!=,a); CHECK_OP_WW(*pa,!=,a); CHECK_OP_WW(*cpa1,!=,a);
    CHECK_OP_WW(ra,< ,a); CHECK_OP_WW(cra1,< ,a); CHECK_OP_WW(*pa,< ,a); CHECK_OP_WW(*cpa1,< ,a);
    CHECK_OP_WW(ra,<=,a); CHECK_OP_WW(cra1,<=,a); CHECK_OP_WW(*pa,<=,a); CHECK_OP_WW(*cpa1,<=,a);
    CHECK_OP_WW(ra,> ,a); CHECK_OP_WW(cra1,> ,a); CHECK_OP_WW(*pa,> ,a); CHECK_OP_WW(*cpa1,> ,a);
    CHECK_OP_WW(ra,>=,a); CHECK_OP_WW(cra1,>=,a); CHECK_OP_WW(*pa,>=,a); CHECK_OP_WW(*cpa1,>=,a);
    
    CHECK_OP_WW(ra,==,c); CHECK_OP_WW(cra1,==,c); CHECK_OP_WW(*pa,==,c); CHECK_OP_WW(*cpa1,==,c);
    CHECK_OP_WW(ra,!=,c); CHECK_OP_WW(cra1,!=,c); CHECK_OP_WW(*pa,!=,c); CHECK_OP_WW(*cpa1,!=,c);
    CHECK_OP_WW(ra,< ,c); CHECK_OP_WW(cra1,< ,c); CHECK_OP_WW(*pa,< ,c); CHECK_OP_WW(*cpa1,< ,c);
    CHECK_OP_WW(ra,<=,c); CHECK_OP_WW(cra1,<=,c); CHECK_OP_WW(*pa,<=,c); CHECK_OP_WW(*cpa1,<=,c);
    CHECK_OP_WW(ra,> ,c); CHECK_OP_WW(cra1,> ,c); CHECK_OP_WW(*pa,> ,c); CHECK_OP_WW(*cpa1,> ,c);
    CHECK_OP_WW(ra,>=,c); CHECK_OP_WW(cra1,>=,c); CHECK_OP_WW(*pa,>=,c); CHECK_OP_WW(*cpa1,>=,c);
    
    CHECK_OP_WW(ra,==,d); CHECK_OP_WW(cra1,==,d); CHECK_OP_WW(*pa,==,d); CHECK_OP_WW(*cpa1,==,d);
    CHECK_OP_WW(ra,!=,d); CHECK_OP_WW(cra1,!=,d); CHECK_OP_WW(*pa,!=,d); CHECK_OP_WW(*cpa1,!=,d);
    CHECK_OP_WW(ra,< ,d); CHECK_OP_WW(cra1,< ,d); CHECK_OP_WW(*pa,< ,d); CHECK_OP_WW(*cpa1,< ,d);
    CHECK_OP_WW(ra,<=,d); CHECK_OP_WW(cra1,<=,d); CHECK_OP_WW(*pa,<=,d); CHECK_OP_WW(*cpa1,<=,d);
    CHECK_OP_WW(ra,> ,d); CHECK_OP_WW(cra1,> ,d); CHECK_OP_WW(*pa,> ,d); CHECK_OP_WW(*cpa1,> ,d);
    CHECK_OP_WW(ra,>=,d); CHECK_OP_WW(cra1,>=,d); CHECK_OP_WW(*pa,>=,d); CHECK_OP_WW(*cpa1,>=,d);
    
    CHECK_OP_VW(13,==,ra); CHECK_OP_VW(13,==,cra1); CHECK_OP_VW(13,==,*pa); CHECK_OP_VW(13,==,*cpa1);
    CHECK_OP_VW(13,!=,ra); CHECK_OP_VW(13,!=,cra1); CHECK_OP_VW(13,!=,*pa); CHECK_OP_VW(13,!=,*cpa1);
    CHECK_OP_VW(13,< ,ra); CHECK_OP_VW(13,< ,cra1); CHECK_OP_VW(13,< ,*pa); CHECK_OP_VW(13,< ,*cpa1);
    CHECK_OP_VW(13,<=,ra); CHECK_OP_VW(13,<=,cra1); CHECK_OP_VW(13,<=,*pa); CHECK_OP_VW(13,<=,*cpa1);
    CHECK_OP_VW(13,> ,ra); CHECK_OP_VW(13,> ,cra1); CHECK_OP_VW(13,> ,*pa); CHECK_OP_VW(13,> ,*cpa1);
    CHECK_OP_VW(13,>=,ra); CHECK_OP_VW(13,>=,cra1); CHECK_OP_VW(13,>=,*pa); CHECK_OP_VW(13,>=,*cpa1);
    
    CHECK_OP_WV(ra,==,13); CHECK_OP_WV(cra1,==,13); CHECK_OP_WV(*pa,==,13); CHECK_OP_WV(*cpa1,==,13);
    CHECK_OP_WV(ra,!=,13); CHECK_OP_WV(cra1,!=,13); CHECK_OP_WV(*pa,!=,13); CHECK_OP_WV(*cpa1,!=,13);
    CHECK_OP_WV(ra,< ,13); CHECK_OP_WV(cra1,< ,13); CHECK_OP_WV(*pa,< ,13); CHECK_OP_WV(*cpa1,< ,13);
    CHECK_OP_WV(ra,<=,13); CHECK_OP_WV(cra1,<=,13); CHECK_OP_WV(*pa,<=,13); CHECK_OP_WV(*cpa1,<=,13);
    CHECK_OP_WV(ra,> ,13); CHECK_OP_WV(cra1,> ,13); CHECK_OP_WV(*pa,> ,13); CHECK_OP_WV(*cpa1,> ,13);
    CHECK_OP_WV(ra,>=,13); CHECK_OP_WV(cra1,>=,13); CHECK_OP_WV(*pa,>=,13); CHECK_OP_WV(*cpa1,>=,13);
    
    CHECK_OP_WW(rb,==,ra); CHECK_OP_WW(rb,==,cra1); CHECK_OP_WW(rb,==,*pa); CHECK_OP_WW(rb,==,*cpa1);
    CHECK_OP_WW(rb,!=,ra); CHECK_OP_WW(rb,!=,cra1); CHECK_OP_WW(rb,!=,*pa); CHECK_OP_WW(rb,!=,*cpa1);
    CHECK_OP_WW(rb,< ,ra); CHECK_OP_WW(rb,< ,cra1); CHECK_OP_WW(rb,< ,*pa); CHECK_OP_WW(rb,< ,*cpa1);
    CHECK_OP_WW(rb,<=,ra); CHECK_OP_WW(rb,<=,cra1); CHECK_OP_WW(rb,<=,*pa); CHECK_OP_WW(rb,<=,*cpa1);
    CHECK_OP_WW(rb,> ,ra); CHECK_OP_WW(rb,> ,cra1); CHECK_OP_WW(rb,> ,*pa); CHECK_OP_WW(rb,> ,*cpa1);
    CHECK_OP_WW(rb,>=,ra); CHECK_OP_WW(rb,>=,cra1); CHECK_OP_WW(rb,>=,*pa); CHECK_OP_WW(rb,>=,*cpa1);
    
    CHECK_OP_WW(*pb,==,ra); CHECK_OP_WW(*pb,==,cra1); CHECK_OP_WW(*pb,==,*pa); CHECK_OP_WW(*pb,==,*cpa1);
    CHECK_OP_WW(*pb,!=,ra); CHECK_OP_WW(*pb,!=,cra1); CHECK_OP_WW(*pb,!=,*pa); CHECK_OP_WW(*pb,!=,*cpa1);
    CHECK_OP_WW(*pb,< ,ra); CHECK_OP_WW(*pb,< ,cra1); CHECK_OP_WW(*pb,< ,*pa); CHECK_OP_WW(*pb,< ,*cpa1);
    CHECK_OP_WW(*pb,<=,ra); CHECK_OP_WW(*pb,<=,cra1); CHECK_OP_WW(*pb,<=,*pa); CHECK_OP_WW(*pb,<=,*cpa1);
    CHECK_OP_WW(*pb,> ,ra); CHECK_OP_WW(*pb,> ,cra1); CHECK_OP_WW(*pb,> ,*pa); CHECK_OP_WW(*pb,> ,*cpa1);
    CHECK_OP_WW(*pb,>=,ra); CHECK_OP_WW(*pb,>=,cra1); CHECK_OP_WW(*pb,>=,*pa); CHECK_OP_WW(*pb,>=,*cpa1);
    
    CHECK_OP_WW(*cpb2,==,ra); CHECK_OP_WW(*cpb2,==,cra1); CHECK_OP_WW(*cpb2,==,*pa); CHECK_OP_WW(*cpb2,==,*cpa1);
    CHECK_OP_WW(*cpb2,!=,ra); CHECK_OP_WW(*cpb2,!=,cra1); CHECK_OP_WW(*cpb2,!=,*pa); CHECK_OP_WW(*cpb2,!=,*cpa1);
    CHECK_OP_WW(*cpb2,< ,ra); CHECK_OP_WW(*cpb2,< ,cra1); CHECK_OP_WW(*cpb2,< ,*pa); CHECK_OP_WW(*cpb2,< ,*cpa1);
    CHECK_OP_WW(*cpb2,<=,ra); CHECK_OP_WW(*cpb2,<=,cra1); CHECK_OP_WW(*cpb2,<=,*pa); CHECK_OP_WW(*cpb2,<=,*cpa1);
    CHECK_OP_WW(*cpb2,> ,ra); CHECK_OP_WW(*cpb2,> ,cra1); CHECK_OP_WW(*cpb2,> ,*pa); CHECK_OP_WW(*cpb2,> ,*cpa1);
    CHECK_OP_WW(*cpb2,>=,ra); CHECK_OP_WW(*cpb2,>=,cra1); CHECK_OP_WW(*cpb2,>=,*pa); CHECK_OP_WW(*cpb2,>=,*cpa1);
  }
  return 0;
}
