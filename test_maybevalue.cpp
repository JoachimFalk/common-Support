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
#include <climits>

#include <CoSupport/DataTypes/MaybeValue.hpp>

#define CHECK_OP_VW(MIN,X,OP,V) do { \
    std::cout << #X " " #OP " " << #V << ": " << (X OP V) << std::endl; \
    assert((X OP V) == (X OP (V.isDefined() ? V.get() : MIN))); \
  } while (0)

#define CHECK_OP_WV(MIN,X,OP,V) do { \
    std::cout << #X " " #OP " " << #V << ": " << (X OP V) << std::endl; \
    assert((X OP V) == ((X.isDefined() ? X.get() : MIN) OP V)); \
  } while (0)

#define CHECK_OP_WW(MIN,X,OP,V) do { \
    std::cout << #X " " #OP " " << #V << ": " << (X OP V) << std::endl; \
    assert((X OP V) == ((X.isDefined() ? X.get() : MIN) OP (V.isDefined() ? V.get() : MIN))); \
  } while (0)

using namespace CoSupport::DataTypes;

int main(int argc, char *argv[]) {
  {
    MaybeValue<int> foo;
    MaybeValue<int> foo2(foo);
    
    std::cout << "foo(): " << foo << std::endl;
    
    assert(!foo.isDefined());
    
    { MaybeValue<int> &check = (foo = 13); assert(&check == &foo); }
    std::cout << "foo = 13: " << foo << std::endl;
    
    { MaybeValue<int> &check = (foo += 1); assert(&check == &foo); }
    std::cout << "foo += 1: " << foo << std::endl;
    std::cout << "++foo:    " << ++foo << std::endl;
    
    { MaybeValue<int> &check = (foo -= 1); assert(&check == &foo); }
    std::cout << "foo -= 1: " << foo << std::endl;
    std::cout << "--foo:    " << --foo << std::endl;
    
    std::cout << "foo++:    " << foo++ << std::endl;
    std::cout << "foo:      " << foo << std::endl;
    
    std::cout << "foo--:    " << foo-- << std::endl;
    std::cout << "foo:      " << foo << std::endl;
    
    { MaybeValue<int> &check = (foo *= 3); assert(&check == &foo); }
    std::cout << "foo *= 3: " << foo << std::endl;
    
    { MaybeValue<int> &check = (foo /= 4); assert(&check == &foo); }
    std::cout << "foo /= 4: " << foo << std::endl;
    
    { MaybeValue<int> &check = (foo %= 5); assert(&check == &foo); }
    std::cout << "foo %= 5: " << foo << std::endl;
    assert(foo.isDefined());
    assert(foo.get() == 4);
    
    foo = 4711;
    std::cout << "foo = 4711: " << foo << std::endl;
    CHECK_OP_WV(INT_MIN,foo,==,4710); CHECK_OP_WV(INT_MIN,foo,==,4711LL); CHECK_OP_WV(INT_MIN,foo,==,4712);
    CHECK_OP_WV(INT_MIN,foo,!=,4710); CHECK_OP_WV(INT_MIN,foo,!=,4711LL); CHECK_OP_WV(INT_MIN,foo,!=,4712);
    CHECK_OP_WV(INT_MIN,foo,< ,4710); CHECK_OP_WV(INT_MIN,foo,< ,4711LL); CHECK_OP_WV(INT_MIN,foo,< ,4712);
    CHECK_OP_WV(INT_MIN,foo,<=,4710); CHECK_OP_WV(INT_MIN,foo,<=,4711LL); CHECK_OP_WV(INT_MIN,foo,<=,4712);
    CHECK_OP_WV(INT_MIN,foo,> ,4710); CHECK_OP_WV(INT_MIN,foo,> ,4711LL); CHECK_OP_WV(INT_MIN,foo,> ,4712);
    CHECK_OP_WV(INT_MIN,foo,>=,4710); CHECK_OP_WV(INT_MIN,foo,>=,4711LL); CHECK_OP_WV(INT_MIN,foo,>=,4712);
    
    { MaybeValue<int> &check = (foo = boost::blank()); assert(&check == &foo); }
    std::cout << "foo = boost::blank(): " << foo << std::endl;
    assert(!foo.isDefined());
    
    MaybeValue<int> bar;
    std::cout << "bar: " << bar << std::endl;
    
    CHECK_OP_WV(INT_MIN,foo,==,13); CHECK_OP_WW(INT_MIN,foo,==,bar);
    CHECK_OP_WV(INT_MIN,foo,!=,13); CHECK_OP_WW(INT_MIN,foo,!=,bar);
    CHECK_OP_WV(INT_MIN,foo,< ,13); CHECK_OP_WW(INT_MIN,foo,< ,bar);
    CHECK_OP_WV(INT_MIN,foo,<=,13); CHECK_OP_WW(INT_MIN,foo,<=,bar);
    CHECK_OP_WV(INT_MIN,foo,> ,13); CHECK_OP_WW(INT_MIN,foo,> ,bar);
    CHECK_OP_WV(INT_MIN,foo,>=,13); CHECK_OP_WW(INT_MIN,foo,>=,bar);
    
    MaybeValue<int> batz13(13);
    MaybeValue<int> batz14(14);
    MaybeValue<int> batz15(15);
    std::cout << "batz13(13): " << batz13 << std::endl;
    std::cout << "batz14(14): " << batz14 << std::endl;
    std::cout << "batz15(15): " << batz15 << std::endl;
    
    bar = 14U;
    std::cout << "bar = 14: " << bar << std::endl;
    
    assert(bar.isDefined());
    assert(bar.get() == 14);
    
    CHECK_OP_WW(INT_MIN,bar,==,batz13); CHECK_OP_WW(INT_MIN,bar,==,batz14); CHECK_OP_WW(INT_MIN,bar,==,batz15);
    CHECK_OP_WW(INT_MIN,bar,!=,batz13); CHECK_OP_WW(INT_MIN,bar,!=,batz14); CHECK_OP_WW(INT_MIN,bar,!=,batz15);
    CHECK_OP_WW(INT_MIN,bar,< ,batz13); CHECK_OP_WW(INT_MIN,bar,< ,batz14); CHECK_OP_WW(INT_MIN,bar,< ,batz15);
    CHECK_OP_WW(INT_MIN,bar,<=,batz13); CHECK_OP_WW(INT_MIN,bar,<=,batz14); CHECK_OP_WW(INT_MIN,bar,<=,batz15);
    CHECK_OP_WW(INT_MIN,bar,> ,batz13); CHECK_OP_WW(INT_MIN,bar,> ,batz14); CHECK_OP_WW(INT_MIN,bar,> ,batz15);
    CHECK_OP_WW(INT_MIN,bar,>=,batz13); CHECK_OP_WW(INT_MIN,bar,>=,batz14); CHECK_OP_WW(INT_MIN,bar,>=,batz15);
    
    bar.set(foo);
    std::cout << "bar: " << bar << std::endl;
    
    assert(!bar.isDefined());
    
    bar.set(33);
    std::cout << "bar: " << bar << std::endl;
    
    MaybeValue<long> batz(bar);
    
    std::cout << "batz: " << batz << std::endl;
    
    assert(batz.isDefined());
    assert(batz.get() == 33);
    
    batz.undef();
    std::cout << "batz: " << batz << std::endl;
    
    foo.set(batz);
    std::cout << "foo: " << foo << std::endl;

    { MaybeValue<int> &check = (foo = bar); assert(&check == &foo); }
    { MaybeValue<int> &check = (foo = batz); assert(&check == &foo); }
    { MaybeValue<int> &check = (foo = MaybeValue<int>(12)); assert(&check == &foo); }
    { MaybeValue<int> &check = (foo = MaybeValue<long>(12)); assert(&check == &foo); }
  }
  {
    MaybeValue<const char *> cp("foo");
    MaybeValue<std::string>  str1(cp);
    MaybeValue<std::string>  str2("bar");
    MaybeValue<std::string>  str3(std::string("zzz"));
    
    std::cout << "cp(\"foo\"):                \"" << cp << "\"" << std::endl;
    std::cout << "str1(cp):                 \"" << str1 << "\"" << std::endl;
    std::cout << "str2(\"bar\"):              \"" << str2 << "\"" << std::endl;
    std::cout << "str3(std::string(\"zzz\")): \"" << str3 << "\"" << std::endl;
    
    CHECK_OP_WV(std::string(),str1,==,std::string("foo")); CHECK_OP_WV(std::string(),str1,==,std::string("bar"));
    CHECK_OP_WV(std::string(),str1,!=,std::string("foo")); CHECK_OP_WV(std::string(),str1,!=,std::string("bar"));
    CHECK_OP_WV(std::string(),str1,< ,std::string("foo")); CHECK_OP_WV(std::string(),str1,< ,std::string("bar")); CHECK_OP_WV(std::string(),str1,< ,std::string("zzz"));
    CHECK_OP_WV(std::string(),str1,<=,std::string("foo")); CHECK_OP_WV(std::string(),str1,<=,std::string("bar")); CHECK_OP_WV(std::string(),str1,<=,std::string("zzz"));
    CHECK_OP_WV(std::string(),str1,> ,std::string("foo")); CHECK_OP_WV(std::string(),str1,> ,std::string("bar")); CHECK_OP_WV(std::string(),str1,> ,std::string("zzz"));
    CHECK_OP_WV(std::string(),str1,>=,std::string("foo")); CHECK_OP_WV(std::string(),str1,>=,std::string("bar")); CHECK_OP_WV(std::string(),str1,>=,std::string("zzz"));

    CHECK_OP_WV(std::string(),str1,==,"foo"); CHECK_OP_WV(std::string(),str1,==,"bar");
    CHECK_OP_WV(std::string(),str1,!=,"foo"); CHECK_OP_WV(std::string(),str1,!=,"bar");
    CHECK_OP_WV(std::string(),str1,< ,"foo"); CHECK_OP_WV(std::string(),str1,< ,"bar"); CHECK_OP_WV(std::string(),str1,< ,"zzz");
    CHECK_OP_WV(std::string(),str1,<=,"foo"); CHECK_OP_WV(std::string(),str1,<=,"bar"); CHECK_OP_WV(std::string(),str1,<=,"zzz");
    CHECK_OP_WV(std::string(),str1,> ,"foo"); CHECK_OP_WV(std::string(),str1,> ,"bar"); CHECK_OP_WV(std::string(),str1,> ,"zzz");
    CHECK_OP_WV(std::string(),str1,>=,"foo"); CHECK_OP_WV(std::string(),str1,>=,"bar"); CHECK_OP_WV(std::string(),str1,>=,"zzz");

    CHECK_OP_WW(std::string(),str1,==,cp); CHECK_OP_WW(std::string(),str1,==,str2);
    CHECK_OP_WW(std::string(),str1,!=,cp); CHECK_OP_WW(std::string(),str1,!=,str2);
    CHECK_OP_WW(std::string(),str1,< ,cp); CHECK_OP_WW(std::string(),str1,< ,str2); CHECK_OP_WW(std::string(),str1,< ,str3);
    CHECK_OP_WW(std::string(),str1,<=,cp); CHECK_OP_WW(std::string(),str1,<=,str2); CHECK_OP_WW(std::string(),str1,<=,str3);
    CHECK_OP_WW(std::string(),str1,> ,cp); CHECK_OP_WW(std::string(),str1,> ,str2); CHECK_OP_WW(std::string(),str1,> ,str3);
    CHECK_OP_WW(std::string(),str1,>=,cp); CHECK_OP_WW(std::string(),str1,>=,str2); CHECK_OP_WW(std::string(),str1,>=,str3);

    CHECK_OP_WV(std::string(),cp,==,std::string("aaa")); CHECK_OP_WV(std::string(),cp,==,std::string("foo"));
    CHECK_OP_WV(std::string(),cp,!=,std::string("aaa")); CHECK_OP_WV(std::string(),cp,!=,std::string("foo"));
    CHECK_OP_WV(std::string(),cp,< ,std::string("aaa")); CHECK_OP_WV(std::string(),cp,< ,std::string("foo")); CHECK_OP_WV(std::string(),cp,< ,std::string("zzz"));
    CHECK_OP_WV(std::string(),cp,<=,std::string("aaa")); CHECK_OP_WV(std::string(),cp,<=,std::string("foo")); CHECK_OP_WV(std::string(),cp,<=,std::string("zzz"));
    CHECK_OP_WV(std::string(),cp,> ,std::string("aaa")); CHECK_OP_WV(std::string(),cp,> ,std::string("foo")); CHECK_OP_WV(std::string(),cp,> ,std::string("zzz"));
    CHECK_OP_WV(std::string(),cp,>=,std::string("aaa")); CHECK_OP_WV(std::string(),cp,>=,std::string("foo")); CHECK_OP_WV(std::string(),cp,>=,std::string("zzz"));
    
    str1 = "aaa"; str2 = "foo"; str3 = "zzz";
    std::cout << "str1 = \"aaa\": \"" << str1 << "\"" << std::endl;
    std::cout << "str2 = \"foo\": \"" << str2 << "\"" << std::endl;
    std::cout << "str3 = \"zzz\": \"" << str3 << "\"" << std::endl;
    
    CHECK_OP_WW(std::string(),cp,==,str1); CHECK_OP_WW(std::string(),cp,==,str2);
    CHECK_OP_WW(std::string(),cp,!=,str1); CHECK_OP_WW(std::string(),cp,!=,str2);
    CHECK_OP_WW(std::string(),cp,< ,str1); CHECK_OP_WW(std::string(),cp,< ,str2); CHECK_OP_WW(std::string(),cp,< ,str3);
    CHECK_OP_WW(std::string(),cp,<=,str1); CHECK_OP_WW(std::string(),cp,<=,str2); CHECK_OP_WW(std::string(),cp,<=,str3);
    CHECK_OP_WW(std::string(),cp,> ,str1); CHECK_OP_WW(std::string(),cp,> ,str2); CHECK_OP_WW(std::string(),cp,> ,str3);
    CHECK_OP_WW(std::string(),cp,>=,str1); CHECK_OP_WW(std::string(),cp,>=,str2); CHECK_OP_WW(std::string(),cp,>=,str3);
    
    str1 = boost::blank();
    std::cout << "str1 = boost::blank(): " << str1 << std::endl;
    assert(!str1.isDefined());
    str3 = boost::blank();
    std::cout << "str3 = boost::blank(): " << str3 << std::endl;
    assert(!str3.isDefined());
    
    CHECK_OP_WV(std::string(),str1,==,"foo"); CHECK_OP_WW(std::string(),str1,==,str2); CHECK_OP_WW(std::string(),str1,==,str3);
    CHECK_OP_WV(std::string(),str1,!=,"foo"); CHECK_OP_WW(std::string(),str1,!=,str2); CHECK_OP_WW(std::string(),str1,!=,str3);
    CHECK_OP_WV(std::string(),str1,< ,"foo"); CHECK_OP_WW(std::string(),str1,< ,str2); CHECK_OP_WW(std::string(),str1,< ,str3);
    CHECK_OP_WV(std::string(),str1,<=,"foo"); CHECK_OP_WW(std::string(),str1,<=,str2); CHECK_OP_WW(std::string(),str1,<=,str3);
    CHECK_OP_WV(std::string(),str1,> ,"foo"); CHECK_OP_WW(std::string(),str1,> ,str2); CHECK_OP_WW(std::string(),str1,> ,str3);
    CHECK_OP_WV(std::string(),str1,>=,"foo"); CHECK_OP_WW(std::string(),str1,>=,str2); CHECK_OP_WW(std::string(),str1,>=,str3);
    
    cp = boost::blank();
    std::cout << "cp = boost::blank(): " << cp << std::endl;
    
    CHECK_OP_WW(std::string(),cp,==,cp); CHECK_OP_WW(std::string(),cp,==,str2); CHECK_OP_WW(std::string(),cp,==,str3);
    CHECK_OP_WW(std::string(),cp,!=,cp); CHECK_OP_WW(std::string(),cp,!=,str2); CHECK_OP_WW(std::string(),cp,!=,str3);
    CHECK_OP_WW(std::string(),cp,< ,cp); CHECK_OP_WW(std::string(),cp,< ,str2); CHECK_OP_WW(std::string(),cp,< ,str3);
    CHECK_OP_WW(std::string(),cp,<=,cp); CHECK_OP_WW(std::string(),cp,<=,str2); CHECK_OP_WW(std::string(),cp,<=,str3);
    CHECK_OP_WW(std::string(),cp,> ,cp); CHECK_OP_WW(std::string(),cp,> ,str2); CHECK_OP_WW(std::string(),cp,> ,str3);
    CHECK_OP_WW(std::string(),cp,>=,cp); CHECK_OP_WW(std::string(),cp,>=,str2); CHECK_OP_WW(std::string(),cp,>=,str3);
    
    CHECK_OP_VW(std::string(),std::string("foo"),==,cp); CHECK_OP_WW(std::string(),str2,==,cp); CHECK_OP_WW(std::string(),str3,==,cp);
    CHECK_OP_VW(std::string(),std::string("foo"),!=,cp); CHECK_OP_WW(std::string(),str2,!=,cp); CHECK_OP_WW(std::string(),str3,!=,cp);
    CHECK_OP_VW(std::string(),std::string("foo"),< ,cp); CHECK_OP_WW(std::string(),str2,< ,cp); CHECK_OP_WW(std::string(),str3,< ,cp);
    CHECK_OP_VW(std::string(),std::string("foo"),<=,cp); CHECK_OP_WW(std::string(),str2,<=,cp); CHECK_OP_WW(std::string(),str3,<=,cp);
    CHECK_OP_VW(std::string(),std::string("foo"),> ,cp); CHECK_OP_WW(std::string(),str2,> ,cp); CHECK_OP_WW(std::string(),str3,> ,cp);
    CHECK_OP_VW(std::string(),std::string("foo"),>=,cp); CHECK_OP_WW(std::string(),str2,>=,cp); CHECK_OP_WW(std::string(),str3,>=,cp);
    
    cp   = "batz";
    str1 = "hix";
    str2 = std::string("hax");
    
    std::cout << "cp = \"batz\":               \"" << cp << "\"" << std::endl;
    std::cout << "str1 = \"hix\":              \"" << str1 << "\"" << std::endl;
    std::cout << "str2 = std::string(\"hax\"): \"" << str2 << "\"" << std::endl;
    
    str1 = cp;
    str2 = str1.get() + cp.get();

    { MaybeValue<std::string> &check = (str1 = str2); assert(&check == &str1); }
  }
  {
    char aaa[] = "aaa";
    char foo[] = "foo";
    char caaa[] = "aaa";
    char cfoo[] = "foo";
    
    MaybeValue<const char *> ccp(cfoo);
    MaybeValue<char *>       cp(foo);
    
    // This is not really string comparison put pointer comparison.
    CHECK_OP_WV(NULL,ccp,==,aaa); CHECK_OP_VW(NULL,foo,==,ccp);
    CHECK_OP_WV(NULL,ccp,!=,aaa); CHECK_OP_VW(NULL,foo,!=,ccp);
    CHECK_OP_WV(NULL,ccp,< ,aaa); CHECK_OP_VW(NULL,foo,< ,ccp);
    CHECK_OP_WV(NULL,ccp,<=,aaa); CHECK_OP_VW(NULL,foo,<=,ccp);
    CHECK_OP_WV(NULL,ccp,> ,aaa); CHECK_OP_VW(NULL,foo,> ,ccp);
    CHECK_OP_WV(NULL,ccp,>=,aaa); CHECK_OP_VW(NULL,foo,>=,ccp);

    CHECK_OP_WV(NULL,ccp,==,caaa); CHECK_OP_VW(NULL,cfoo,==,ccp);
    CHECK_OP_WV(NULL,ccp,!=,caaa); CHECK_OP_VW(NULL,cfoo,!=,ccp);
    CHECK_OP_WV(NULL,ccp,< ,caaa); CHECK_OP_VW(NULL,cfoo,< ,ccp);
    CHECK_OP_WV(NULL,ccp,<=,caaa); CHECK_OP_VW(NULL,cfoo,<=,ccp);
    CHECK_OP_WV(NULL,ccp,> ,caaa); CHECK_OP_VW(NULL,cfoo,> ,ccp);
    CHECK_OP_WV(NULL,ccp,>=,caaa); CHECK_OP_VW(NULL,cfoo,>=,ccp);

    CHECK_OP_WV(NULL,cp,==,aaa); CHECK_OP_VW(NULL,foo,==,cp);
    CHECK_OP_WV(NULL,cp,!=,aaa); CHECK_OP_VW(NULL,foo,!=,cp);
    CHECK_OP_WV(NULL,cp,< ,aaa); CHECK_OP_VW(NULL,foo,< ,cp);
    CHECK_OP_WV(NULL,cp,<=,aaa); CHECK_OP_VW(NULL,foo,<=,cp);
    CHECK_OP_WV(NULL,cp,> ,aaa); CHECK_OP_VW(NULL,foo,> ,cp);
    CHECK_OP_WV(NULL,cp,>=,aaa); CHECK_OP_VW(NULL,foo,>=,cp);

    CHECK_OP_WV(NULL,cp,==,caaa); CHECK_OP_VW(NULL,cfoo,==,cp);
    CHECK_OP_WV(NULL,cp,!=,caaa); CHECK_OP_VW(NULL,cfoo,!=,cp);
    CHECK_OP_WV(NULL,cp,< ,caaa); CHECK_OP_VW(NULL,cfoo,< ,cp);
    CHECK_OP_WV(NULL,cp,<=,caaa); CHECK_OP_VW(NULL,cfoo,<=,cp);
    CHECK_OP_WV(NULL,cp,> ,caaa); CHECK_OP_VW(NULL,cfoo,> ,cp);
    CHECK_OP_WV(NULL,cp,>=,caaa); CHECK_OP_VW(NULL,cfoo,>=,cp);
  }

  return 0;
}
