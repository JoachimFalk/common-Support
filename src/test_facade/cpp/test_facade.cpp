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
 *   FOR AFacade PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
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
 * OF MERCHANTABILITY AND FITNESS FOR AFacade PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
 * HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF
 * ERLANGEN NUREMBERG HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 */

#include <iostream>

#include <CoSupport/compatibility-glue/nullptr.h>

#include <CoSupport/sassert.h>
#include <CoSupport/String/TypeName.hpp>

#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/remove_reference.hpp>

#include <boost/type_traits/add_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>

#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/static_assert.hpp>

#include "test_facade_lib.hpp"

#include <list>
#include <set>

using namespace Something;

class X {};

COSUPPORT_REGISTER_TYPENAME(X)
COSUPPORT_REGISTER_TYPENAME(AFacade)
COSUPPORT_REGISTER_TYPENAME(AFacade::Ref)
COSUPPORT_REGISTER_TYPENAME(AFacade::ConstRef)
COSUPPORT_REGISTER_TYPENAME(AFacade::Ptr)
COSUPPORT_REGISTER_TYPENAME(AFacade::ConstPtr)

int main(int argc, char *argv[]) {
  AFacade a;
  BFacade b;
  CFacade c;
  const AFacade ca;
  const BFacade cb;
  const CFacade cc;
  
  assert(a.toPtr());
  assert(b.toPtr());
  assert(c.toPtr());
  assert(&a);
  assert(&b);
  assert(&c);

  AFacade::Ref ra = a;
  BFacade::Ref rb = b;
  CFacade::Ref rc = c;

  assert(ra.toPtr());
  assert(rb.toPtr());
  assert(rc.toPtr());
  assert(&ra);
  assert(&rb);
  assert(&rc);

  assert(a.toPtr() == ra.toPtr());
  assert(a.toPtr() == &ra);
  assert(&a == ra.toPtr());
  assert(&a == &ra);

  AFacade::Ptr pa = &b;
  BFacade::Ptr pb = b.toPtr();
  CFacade::Ptr pc = &rc;
  
  assert(pa != &a);
  assert(pa != a.toPtr());
  assert(&a        != pa);
  assert(a.toPtr() != pa);

  assert(pb != &a);
  assert(pb != a.toPtr());
  assert(&a        != pb);
  assert(a.toPtr() != pb);
  assert(pb != &c);
  assert(pb != c.toPtr());
  assert(&c        != pb);
  assert(c.toPtr() != pb);
  assert(pb == &rb);
  assert(pb == rb.toPtr());
  assert(&b        == pb);
  assert(b.toPtr() == pb);

  AFacade::ConstPtr cpa;
  BFacade::ConstPtr cpb;
  CFacade::ConstPtr cpc;
  
  assert(!cpa);
  assert(!cpb);
  assert(!cpc);

  AFacade::ConstRef cra = c;
  BFacade::ConstRef crb = *cpb;
  CFacade::ConstRef crc = rc;

  assert(cra.toPtr());
  assert(&cra);
  assert(!crb.toPtr());
  assert(!&crb);
  assert(crc.toPtr());
  assert(&crc);

  assert(b.toPtr());
  assert(c.toPtr());
  assert(&a);
  assert(&b);
  assert(&c);

  AFacade::Ptr pa1 = pa;
  AFacade::Ptr pa2 = pb;
  AFacade::Ptr pa3 = pc;
  AFacade::Ptr pa4 = &a;
  AFacade::Ptr pa5 = &b;
  AFacade::Ptr pa6 = &c;
  AFacade::Ptr pa7 = a.toPtr();
  AFacade::Ptr pa8 = b.toPtr();
  AFacade::Ptr pa9 = c.toPtr();
  AFacade::Ptr pa10= &ra;
  AFacade::Ptr pa11= &rb;
  AFacade::Ptr pa12= &rc;
  AFacade::Ptr pa13= ra.toPtr();
  AFacade::Ptr pa14= rb.toPtr();
  AFacade::Ptr pa15= rc.toPtr();
  AFacade::Ptr pa16(pa);
  AFacade::Ptr pa17(pb);
  AFacade::Ptr pa18(pc);
  AFacade::Ptr pa19(&a);
  AFacade::Ptr pa20(&b);
  AFacade::Ptr pa21(&c);
  AFacade::Ptr pa22(a.toPtr());
  AFacade::Ptr pa23(b.toPtr());
  AFacade::Ptr pa24(c.toPtr());
  AFacade::Ptr pa25(&ra);
  AFacade::Ptr pa26(&rb);
  AFacade::Ptr pa27(&rc);
  AFacade::Ptr pa28(ra.toPtr());
  AFacade::Ptr pa29(rb.toPtr());
  AFacade::Ptr pa30(rc.toPtr());
  
  pa = pa1;
  pa = pb;
  pa = pc;
  pa = &a;
  pa = &b;
  pa = &c;
  pa = a.toPtr();
  pa = b.toPtr();
  pa = c.toPtr();
  pa = &ra;
  pa = &rb;
  pa = &rc;
  pa = ra.toPtr();
  pa = rb.toPtr();
  pa = rc.toPtr();
  
  AFacade::ConstPtr cpa1 = pa;
  AFacade::ConstPtr cpa2 = pb;
  AFacade::ConstPtr cpa3 = pc;
  AFacade::ConstPtr cpa4 = cpa;
  AFacade::ConstPtr cpa5 = cpb;
  AFacade::ConstPtr cpa6 = cpc;
  AFacade::ConstPtr cpa7 = &a;
  AFacade::ConstPtr cpa8 = &b;
  AFacade::ConstPtr cpa9 = &c;
  AFacade::ConstPtr cpa10= &ca;
  AFacade::ConstPtr cpa11= &cb;
  AFacade::ConstPtr cpa12= &cc;
  AFacade::ConstPtr cpa13= a.toPtr();
  AFacade::ConstPtr cpa14= b.toPtr();
  AFacade::ConstPtr cpa15= c.toPtr();
  AFacade::ConstPtr cpa16= ca.toPtr();
  AFacade::ConstPtr cpa17= cb.toPtr();
  AFacade::ConstPtr cpa18= cc.toPtr();
  AFacade::ConstPtr cpa19= &ra;
  AFacade::ConstPtr cpa20= &rb;
  AFacade::ConstPtr cpa21= &rc;
  AFacade::ConstPtr cpa22= ra.toPtr();
  AFacade::ConstPtr cpa23= rb.toPtr();
  AFacade::ConstPtr cpa24= rc.toPtr();
  AFacade::ConstPtr cpa25= &cra;
  AFacade::ConstPtr cpa26= &crb;
  AFacade::ConstPtr cpa27= &crc;
  AFacade::ConstPtr cpa28= cra.toPtr();
  AFacade::ConstPtr cpa29= crb.toPtr();
  AFacade::ConstPtr cpa30= crc.toPtr();
  AFacade::ConstPtr cpa31(pa);
  AFacade::ConstPtr cpa32(pb);
  AFacade::ConstPtr cpa33(pc);
  AFacade::ConstPtr cpa34(cpa);
  AFacade::ConstPtr cpa35(cpb);
  AFacade::ConstPtr cpa36(cpc);
  AFacade::ConstPtr cpa37(&a);
  AFacade::ConstPtr cpa38(&b);
  AFacade::ConstPtr cpa39(&c);
  AFacade::ConstPtr cpa40(&ca);
  AFacade::ConstPtr cpa41(&cb);
  AFacade::ConstPtr cpa42(&cc);
  AFacade::ConstPtr cpa43(a.toPtr());
  AFacade::ConstPtr cpa44(b.toPtr());
  AFacade::ConstPtr cpa45(c.toPtr());
  AFacade::ConstPtr cpa46(ca.toPtr());
  AFacade::ConstPtr cpa47(cb.toPtr());
  AFacade::ConstPtr cpa48(cc.toPtr());
  AFacade::ConstPtr cpa49(&ra);
  AFacade::ConstPtr cpa50(&rb);
  AFacade::ConstPtr cpa51(&rc);
  AFacade::ConstPtr cpa52(ra.toPtr());
  AFacade::ConstPtr cpa53(rb.toPtr());
  AFacade::ConstPtr cpa54(rc.toPtr());
  AFacade::ConstPtr cpa55(&cra);
  AFacade::ConstPtr cpa56(&crb);
  AFacade::ConstPtr cpa57(&crc);
  AFacade::ConstPtr cpa58(cra.toPtr());
  AFacade::ConstPtr cpa59(crb.toPtr());
  AFacade::ConstPtr cpa60(crc.toPtr());
  
  cpa = &a;
  cpa = &b;
  cpa = &c;
  cpa = &ca;
  cpa = &cb;
  cpa = &cc;
  cpa = a.toPtr();
  cpa = b.toPtr();
  cpa = c.toPtr();
  cpa = ca.toPtr();
  cpa = cb.toPtr();
  cpa = cc.toPtr();
  cpa = &ra;
  cpa = &rb;
  cpa = &rc;
  cpa = ra.toPtr();
  cpa = rb.toPtr();
  cpa = rc.toPtr();
  cpa = &cra;
  cpa = &crb;
  cpa = &crc;
  cpa = cra.toPtr();
  cpa = crb.toPtr();
  cpa = crc.toPtr();
  
  AFacade::Ref ra1 = a;
  AFacade::Ref ra2 = b;
  AFacade::Ref ra3 = c;
  AFacade::Ref ra4 = ra;
  AFacade::Ref ra5 = rb;
  AFacade::Ref ra6 = rc;
  AFacade::Ref ra7 (a);
  AFacade::Ref ra8 (b);
  AFacade::Ref ra9 (c);
  AFacade::Ref ra10(ra);
  AFacade::Ref ra11(rb);
  AFacade::Ref ra12(rc);
  
  AFacade::ConstRef cra1 = a;
  AFacade::ConstRef cra2 = b;
  AFacade::ConstRef cra3 = c;
  AFacade::ConstRef cra4 = ca;
  AFacade::ConstRef cra5 = cb;
  AFacade::ConstRef cra6 = cc;
  AFacade::ConstRef cra7 = ra;
  AFacade::ConstRef cra8 = rb;
  AFacade::ConstRef cra9 = rc;
  AFacade::ConstRef cra10= cra;
  AFacade::ConstRef cra11= crb;
  AFacade::ConstRef cra12= crc;
  AFacade::ConstRef cra13(a);
  AFacade::ConstRef cra14(b);
  AFacade::ConstRef cra15(c);
  AFacade::ConstRef cra16(ca);
  AFacade::ConstRef cra17(cb);
  AFacade::ConstRef cra18(cc);
  AFacade::ConstRef cra19(ra);
  AFacade::ConstRef cra20(rb);
  AFacade::ConstRef cra21(rc);
  AFacade::ConstRef cra22(cra);
  AFacade::ConstRef cra23(crb);
  AFacade::ConstRef cra24(crc);

  bool result = true;
  do { result = false; } while (result);

  if ((pa || cpa) && !pa && cpa)
    ;

#define TESTCOMPOPS(R) \
  result = pa  == R; \
  result = pa  != R; \
  result = pa  <= R; \
  result = pa  <  R; \
  result = pa  >= R; \
  result = pa  >  R; \
  result = cpa == R; \
  result = cpa != R; \
  result = cpa <= R; \
  result = cpa <  R; \
  result = cpa >= R; \
  result = cpa >  R; \
  result = R == pa;  \
  result = R != pa;  \
  result = R <= pa;  \
  result = R <  pa;  \
  result = R >= pa;  \
  result = R >  pa;  \
  result = R == cpa; \
  result = R != cpa; \
  result = R <= cpa; \
  result = R <  cpa; \
  result = R >= cpa; \
  result = R >  cpa; \
  result = pb  == R; \
  result = pb  != R; \
  result = pb  <= R; \
  result = pb  <  R; \
  result = pb  >= R; \
  result = pb  >  R; \
  result = cpb == R; \
  result = cpb != R; \
  result = cpb <= R; \
  result = cpb <  R; \
  result = cpb >= R; \
  result = cpb >  R; \
  result = R == pb;  \
  result = R != pb;  \
  result = R <= pb;  \
  result = R <  pb;  \
  result = R >= pb;  \
  result = R >  pb;  \
  result = R == cpb; \
  result = R != cpb; \
  result = R <= cpb; \
  result = R <  cpb; \
  result = R >= cpb; \
  result = R >  cpb; \
  result = pc  == R; \
  result = pc  != R; \
  result = pc  <= R; \
  result = pc  <  R; \
  result = pc  >= R; \
  result = pc  >  R; \
  result = cpc == R; \
  result = cpc != R; \
  result = cpc <= R; \
  result = cpc <  R; \
  result = cpc >= R; \
  result = cpc >  R; \
  result = R == pc;  \
  result = R != pc;  \
  result = R <= pc;  \
  result = R <  pc;  \
  result = R >= pc;  \
  result = R >  pc;  \
  result = R == cpc; \
  result = R != cpc; \
  result = R <= cpc; \
  result = R <  cpc; \
  result = R >= cpc; \
  result = R >  cpc; \
  result = &a  == R; \
  result = &a  != R; \
  result = &a  <= R; \
  result = &a  <  R; \
  result = &a  >= R; \
  result = &a  >  R; \
  result = &ca == R; \
  result = &ca != R; \
  result = &ca <= R; \
  result = &ca <  R; \
  result = &ca >= R; \
  result = &ca >  R; \
  result = R == &a;  \
  result = R != &a;  \
  result = R <= &a;  \
  result = R <  &a;  \
  result = R >= &a;  \
  result = R >  &a;  \
  result = R == &ca; \
  result = R != &ca; \
  result = R <= &ca; \
  result = R <  &ca; \
  result = R >= &ca; \
  result = R >  &ca; \
  result = &b  == R; \
  result = &b  != R; \
  result = &b  <= R; \
  result = &b  <  R; \
  result = &b  >= R; \
  result = &b  >  R; \
  result = &cb == R; \
  result = &cb != R; \
  result = &cb <= R; \
  result = &cb <  R; \
  result = &cb >= R; \
  result = &cb >  R; \
  result = R == &b;  \
  result = R != &b;  \
  result = R <= &b;  \
  result = R <  &b;  \
  result = R >= &b;  \
  result = R >  &b;  \
  result = R == &cb; \
  result = R != &cb; \
  result = R <= &cb; \
  result = R <  &cb; \
  result = R >= &cb; \
  result = R >  &cb; \
  result = &c  == R; \
  result = &c  != R; \
  result = &c  <= R; \
  result = &c  <  R; \
  result = &c  >= R; \
  result = &c  >  R; \
  result = &cc == R; \
  result = &cc != R; \
  result = &cc <= R; \
  result = &cc <  R; \
  result = &cc >= R; \
  result = &cc >  R; \
  result = R == &c;  \
  result = R != &c;  \
  result = R <= &c;  \
  result = R <  &c;  \
  result = R >= &c;  \
  result = R >  &c;  \
  result = R == &cc; \
  result = R != &cc; \
  result = R <= &cc; \
  result = R <  &cc; \
  result = R >= &cc; \
  result = R >  &cc; \
  result = &ra  == R; \
  result = &ra  != R; \
  result = &ra  <= R; \
  result = &ra  <  R; \
  result = &ra  >= R; \
  result = &ra  >  R; \
  result = &cra == R; \
  result = &cra != R; \
  result = &cra <= R; \
  result = &cra <  R; \
  result = &cra >= R; \
  result = &cra >  R; \
  result = R == &ra;  \
  result = R != &ra;  \
  result = R <= &ra;  \
  result = R <  &ra;  \
  result = R >= &ra;  \
  result = R >  &ra;  \
  result = R == &cra; \
  result = R != &cra; \
  result = R <= &cra; \
  result = R <  &cra; \
  result = R >= &cra; \
  result = R >  &cra; \
  result = &rb  == R; \
  result = &rb  != R; \
  result = &rb  <= R; \
  result = &rb  <  R; \
  result = &rb  >= R; \
  result = &rb  >  R; \
  result = &crb == R; \
  result = &crb != R; \
  result = &crb <= R; \
  result = &crb <  R; \
  result = &crb >= R; \
  result = &crb >  R; \
  result = R == &rb;  \
  result = R != &rb;  \
  result = R <= &rb;  \
  result = R <  &rb;  \
  result = R >= &rb;  \
  result = R >  &rb;  \
  result = R == &crb; \
  result = R != &crb; \
  result = R <= &crb; \
  result = R <  &crb; \
  result = R >= &crb; \
  result = R >  &crb; \
  result = &rc  == R; \
  result = &rc  != R; \
  result = &rc  <= R; \
  result = &rc  <  R; \
  result = &rc  >= R; \
  result = &rc  >  R; \
  result = &crc == R; \
  result = &crc != R; \
  result = &crc <= R; \
  result = &crc <  R; \
  result = &crc >= R; \
  result = &crc >  R; \
  result = R == &rc;  \
  result = R != &rc;  \
  result = R <= &rc;  \
  result = R <  &rc;  \
  result = R >= &rc;  \
  result = R >  &rc;  \
  result = R == &crc; \
  result = R != &crc; \
  result = R <= &crc; \
  result = R <  &crc; \
  result = R >= &crc; \
  result = R >  &crc; \
  result = R != nullptr; \
  result = R == nullptr; \
  result = nullptr != R; \
  result = nullptr == R; \
  result = R;

  TESTCOMPOPS(pa)
  TESTCOMPOPS(pb)
  TESTCOMPOPS(pc)
  TESTCOMPOPS(cpa)
  TESTCOMPOPS(cpb)
  TESTCOMPOPS(cpc)
  TESTCOMPOPS(a.toPtr())
  TESTCOMPOPS(b.toPtr())
  TESTCOMPOPS(c.toPtr())
  TESTCOMPOPS(ca.toPtr())
  TESTCOMPOPS(cb.toPtr())
  TESTCOMPOPS(cc.toPtr())
  TESTCOMPOPS(ra.toPtr())
  TESTCOMPOPS(rb.toPtr())
  TESTCOMPOPS(rc.toPtr())
  TESTCOMPOPS(cra.toPtr())
  TESTCOMPOPS(crb.toPtr())
  TESTCOMPOPS(crc.toPtr())

  pa = &a; cpa = &a;
  sassert(pa == cpa);
  pa = &a; cpa = &ca;
  sassert(pa != cpa);
  pa = nullptr;
  sassert(pa == nullptr);
  cpa = nullptr;
  sassert(cpa == nullptr);
  pa = &a;
  sassert(pa != nullptr);
  cpa = &a;
  sassert(cpa != nullptr);

#define TYPENAME(x) #x << ": " << CoSupport::String::TypeName<x>::name()

  std::cout << TYPENAME(X) << std::endl;
  std::cout << TYPENAME(X const) << std::endl;
  std::cout << TYPENAME(X *) << std::endl;
  std::cout << TYPENAME(X const *) << std::endl;
  std::cout << TYPENAME(X &) << std::endl;
  std::cout << TYPENAME(X const &) << std::endl;
  std::cout << TYPENAME(boost::add_reference<X>::type) << std::endl;
  std::cout << TYPENAME(boost::add_reference<X const>::type) << std::endl;
  std::cout << TYPENAME(boost::add_reference<X *>::type) << std::endl;
  std::cout << TYPENAME(boost::add_reference<X const *>::type) << std::endl;
  std::cout << TYPENAME(boost::add_reference<X &>::type) << std::endl;
  std::cout << TYPENAME(boost::add_reference<X const &>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_reference<X>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_reference<X const>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_reference<X *>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_reference<X const *>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_reference<X &>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_reference<X const &>::type) << std::endl;
  std::cout << TYPENAME(boost::add_pointer<X>::type) << std::endl;
  std::cout << TYPENAME(boost::add_pointer<X const>::type) << std::endl;
  std::cout << TYPENAME(boost::add_pointer<X *>::type) << std::endl;
  std::cout << TYPENAME(boost::add_pointer<X const *>::type) << std::endl;
  std::cout << TYPENAME(boost::add_pointer<X &>::type) << std::endl;
  std::cout << TYPENAME(boost::add_pointer<X const &>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_pointer<X>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_pointer<X const>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_pointer<X *>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_pointer<X const *>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_pointer<X &>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_pointer<X const &>::type) << std::endl;
  std::cout << TYPENAME(boost::add_const<X>::type) << std::endl;
  std::cout << TYPENAME(boost::add_const<X const>::type) << std::endl;
  std::cout << TYPENAME(boost::add_const<X *>::type) << std::endl;
  std::cout << TYPENAME(boost::add_const<X const *>::type) << std::endl;
  std::cout << TYPENAME(boost::add_const<X &>::type) << std::endl;
  std::cout << TYPENAME(boost::add_const<X const &>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_const<X>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_const<X const>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_const<X *>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_const<X const *>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_const<X &>::type) << std::endl;
  std::cout << TYPENAME(boost::remove_const<X const &>::type) << std::endl;

  std::cout << TYPENAME(AFacade) << std::endl;
  std::cout << TYPENAME(AFacade const) << std::endl;
  std::cout << TYPENAME(AFacade::Ptr) << std::endl;
  std::cout << TYPENAME(AFacade::ConstPtr) << std::endl;
  std::cout << TYPENAME(AFacade::Ref) << std::endl;
  std::cout << TYPENAME(AFacade::ConstRef) << std::endl;
  std::cout << TYPENAME(boost::add_reference<AFacade>::type) << std::endl;
//BOOST_STATIC_ASSERT((boost::is_same<boost::add_reference<AFacade>::type, AFacade::Ref>::value)); cf. FIXME in Facade.hpp
  std::cout << TYPENAME(boost::add_reference<AFacade const>::type) << std::endl;
//BOOST_STATIC_ASSERT((boost::is_same<boost::add_reference<AFacade const>::type, AFacade::ConstRef>::value)); cf. FIXME in Facade.hpp
  std::cout << TYPENAME(boost::remove_reference<AFacade>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_reference<AFacade>::type, AFacade>::value));
  std::cout << TYPENAME(boost::remove_reference<AFacade const>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_reference<AFacade const>::type, AFacade const>::value));
  std::cout << TYPENAME(boost::remove_reference<AFacade::Ptr>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_reference<AFacade::Ptr>::type, AFacade::Ptr>::value));
  std::cout << TYPENAME(boost::remove_reference<AFacade::ConstPtr>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_reference<AFacade::ConstPtr>::type, AFacade::ConstPtr>::value));
  std::cout << TYPENAME(boost::remove_reference<AFacade::Ref>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_reference<AFacade::Ref>::type, AFacade>::value));
  std::cout << TYPENAME(boost::remove_reference<AFacade::ConstRef>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_reference<AFacade::ConstRef>::type, AFacade const>::value));
  std::cout << TYPENAME(boost::add_pointer<AFacade>::type) << std::endl;
//BOOST_STATIC_ASSERT((boost::is_same<boost::add_pointer<AFacade>::type, AFacade::Ptr>::value)); cf. FIXME in Facade.hpp
  std::cout << TYPENAME(boost::add_pointer<AFacade const>::type) << std::endl;
//BOOST_STATIC_ASSERT((boost::is_same<boost::add_pointer<AFacade const>::type, AFacade::ConstPtr>::value)); cf. FIXME in Facade.hpp
  std::cout << TYPENAME(boost::remove_pointer<AFacade>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_pointer<AFacade>::type, AFacade>::value));
  std::cout << TYPENAME(boost::remove_pointer<AFacade const>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_pointer<AFacade const>::type, AFacade const>::value));
  std::cout << TYPENAME(boost::remove_pointer<AFacade::Ptr>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_pointer<AFacade::Ptr>::type, AFacade>::value));
  std::cout << TYPENAME(boost::remove_pointer<AFacade::ConstPtr>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_pointer<AFacade::ConstPtr>::type, AFacade const>::value));
  std::cout << TYPENAME(boost::remove_pointer<AFacade::Ref>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_pointer<AFacade::Ref>::type, AFacade::Ref>::value));
  std::cout << TYPENAME(boost::remove_pointer<AFacade::ConstRef>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_pointer<AFacade::ConstRef>::type, AFacade::ConstRef>::value));
  std::cout << TYPENAME(boost::add_const<AFacade>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::add_const<AFacade>::type, AFacade const>::value));
  std::cout << TYPENAME(boost::add_const<AFacade const>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::add_const<AFacade const>::type, AFacade const>::value));
  std::cout << TYPENAME(boost::add_const<AFacade::Ptr>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::add_const<AFacade::Ptr>::type, AFacade::ConstPtr>::value));
  std::cout << TYPENAME(boost::add_const<AFacade::ConstPtr>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::add_const<AFacade::ConstPtr>::type, AFacade::ConstPtr>::value));
  std::cout << TYPENAME(boost::add_const<AFacade::Ref>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::add_const<AFacade::Ref>::type, AFacade::ConstRef>::value));
  std::cout << TYPENAME(boost::add_const<AFacade::ConstRef>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::add_const<AFacade::ConstRef>::type, AFacade::ConstRef>::value));
  std::cout << TYPENAME(boost::remove_const<AFacade>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_const<AFacade>::type, AFacade>::value));
  std::cout << TYPENAME(boost::remove_const<AFacade const>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_const<AFacade const>::type, AFacade>::value));
  std::cout << TYPENAME(boost::remove_const<AFacade::Ptr>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_const<AFacade::Ptr>::type, AFacade::Ptr>::value));
  std::cout << TYPENAME(boost::remove_const<AFacade::ConstPtr>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_const<AFacade::ConstPtr>::type, AFacade::Ptr>::value));
  std::cout << TYPENAME(boost::remove_const<AFacade::Ref>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_const<AFacade::Ref>::type, AFacade::Ref>::value));
  std::cout << TYPENAME(boost::remove_const<AFacade::ConstRef>::type) << std::endl;
  BOOST_STATIC_ASSERT((boost::is_same<boost::remove_const<AFacade::ConstRef>::type, AFacade::Ref>::value));

  {
    std::list<AFacade> al;
    al.push_back(a);
    std::list<AFacade>::iterator iter1 = al.begin();
    iter1->aNonConst();
    iter1->aConst();
    std::list<AFacade>::const_iterator iter2 = al.begin();
    iter2->aConst();
  }
  {
    std::list<AFacade::Ref> arl;
    arl.push_back(a);
    std::list<AFacade::Ref>::iterator iter1 = arl.begin();
    iter1->aNonConst();
    iter1->aConst();
    std::list<AFacade::Ref>::const_iterator iter2 = arl.begin();
    iter2->aConst();
  }
/*{
    std::list<AFacade::ConstRef> acrl;
    acrl.push_back(a);
    std::list<AFacade::ConstRef>::iterator iter1 = acrl.begin();
    iter1->aConst();
    std::list<AFacade::ConstRef>::const_iterator iter2 = acrl.begin();
    iter2->aConst();
  }*/
}
