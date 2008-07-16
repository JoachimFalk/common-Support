
#include <CoSupport/sassert.h>

#include "test_facade_lib.hpp"

using namespace Something;

int main(int argc, char *argv[]) {
  A a;
  B b;
  C c;
  const A ca;
  const B cb;
  const C cc;
  
  A::Ref ra = a;
  A::Ref rb = b;
  A::Ref rc = c;

  A::Ptr pa;
  A::Ptr pb;
  A::Ptr pc;
  
  A::ConstRef cra = a;
  A::ConstRef crb = b;
  A::ConstRef crc = c;
  
  A::ConstPtr cpa;
  A::ConstPtr cpb;
  A::ConstPtr cpc;
  
  A::Ptr pa1 = pa;
  A::Ptr pa2 = pb;
  A::Ptr pa3 = pc;
  A::Ptr pa4 = &a;
  A::Ptr pa5 = &b;
  A::Ptr pa6 = &c;
  A::Ptr pa7 = a.toPtr();
  A::Ptr pa8 = b.toPtr();
  A::Ptr pa9 = c.toPtr();
  A::Ptr pa10= &ra;
  A::Ptr pa11= &rb;
  A::Ptr pa12= &rc;
  A::Ptr pa13= ra.toPtr();
  A::Ptr pa14= rb.toPtr();
  A::Ptr pa15= rc.toPtr();
  A::Ptr pa16(pa);
  A::Ptr pa17(pb);
  A::Ptr pa18(pc);
  A::Ptr pa19(&a);
  A::Ptr pa20(&b);
  A::Ptr pa21(&c);
  A::Ptr pa22(a.toPtr());
  A::Ptr pa23(b.toPtr());
  A::Ptr pa24(c.toPtr());
  A::Ptr pa25(&ra);
  A::Ptr pa26(&rb);
  A::Ptr pa27(&rc);
  A::Ptr pa28(ra.toPtr());
  A::Ptr pa29(rb.toPtr());
  A::Ptr pa30(rc.toPtr());
  
  pa = pa;
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
  
  A::ConstPtr cpa1 = pa;
  A::ConstPtr cpa2 = pb;
  A::ConstPtr cpa3 = pc;
  A::ConstPtr cpa4 = cpa;
  A::ConstPtr cpa5 = cpb;
  A::ConstPtr cpa6 = cpc;
  A::ConstPtr cpa7 = &a;
  A::ConstPtr cpa8 = &b;
  A::ConstPtr cpa9 = &c;
  A::ConstPtr cpa10= &ca;
  A::ConstPtr cpa11= &cb;
  A::ConstPtr cpa12= &cc;
  A::ConstPtr cpa13= a.toPtr();
  A::ConstPtr cpa14= b.toPtr();
  A::ConstPtr cpa15= c.toPtr();
  A::ConstPtr cpa16= ca.toPtr();
  A::ConstPtr cpa17= cb.toPtr();
  A::ConstPtr cpa18= cc.toPtr();
  A::ConstPtr cpa19= &ra;
  A::ConstPtr cpa20= &rb;
  A::ConstPtr cpa21= &rc;
  A::ConstPtr cpa22= ra.toPtr();
  A::ConstPtr cpa23= rb.toPtr();
  A::ConstPtr cpa24= rc.toPtr();
  A::ConstPtr cpa25= &cra;
  A::ConstPtr cpa26= &crb;
  A::ConstPtr cpa27= &crc;
  A::ConstPtr cpa28= cra.toPtr();
  A::ConstPtr cpa29= crb.toPtr();
  A::ConstPtr cpa30= crc.toPtr();
  A::ConstPtr cpa31(pa);
  A::ConstPtr cpa32(pb);
  A::ConstPtr cpa33(pc);
  A::ConstPtr cpa34(cpa);
  A::ConstPtr cpa35(cpb);
  A::ConstPtr cpa36(cpc);
  A::ConstPtr cpa37(&a);
  A::ConstPtr cpa38(&b);
  A::ConstPtr cpa39(&c);
  A::ConstPtr cpa40(&ca);
  A::ConstPtr cpa41(&cb);
  A::ConstPtr cpa42(&cc);
  A::ConstPtr cpa43(a.toPtr());
  A::ConstPtr cpa44(b.toPtr());
  A::ConstPtr cpa45(c.toPtr());
  A::ConstPtr cpa46(ca.toPtr());
  A::ConstPtr cpa47(cb.toPtr());
  A::ConstPtr cpa48(cc.toPtr());
  A::ConstPtr cpa49(&ra);
  A::ConstPtr cpa50(&rb);
  A::ConstPtr cpa51(&rc);
  A::ConstPtr cpa52(ra.toPtr());
  A::ConstPtr cpa53(rb.toPtr());
  A::ConstPtr cpa54(rc.toPtr());
  A::ConstPtr cpa55(&cra);
  A::ConstPtr cpa56(&crb);
  A::ConstPtr cpa57(&crc);
  A::ConstPtr cpa58(cra.toPtr());
  A::ConstPtr cpa59(crb.toPtr());
  A::ConstPtr cpa60(crc.toPtr());
  
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
  
  A::Ref ra1 = a;
  A::Ref ra2 = b;
  A::Ref ra3 = c;
  A::Ref ra4 = ra;
  A::Ref ra5 = rb;
  A::Ref ra6 = rc;
  A::Ref ra7 (a);
  A::Ref ra8 (b);
  A::Ref ra9 (c);
  A::Ref ra10(ra);
  A::Ref ra11(rb);
  A::Ref ra12(rc);
  
  A::ConstRef cra1 = a;
  A::ConstRef cra2 = b;
  A::ConstRef cra3 = c;
  A::ConstRef cra4 = ca;
  A::ConstRef cra5 = cb;
  A::ConstRef cra6 = cc;
  A::ConstRef cra7 = ra;
  A::ConstRef cra8 = rb;
  A::ConstRef cra9 = rc;
  A::ConstRef cra10= cra;
  A::ConstRef cra11= crb;
  A::ConstRef cra12= crc;
  A::ConstRef cra13(a);
  A::ConstRef cra14(b);
  A::ConstRef cra15(c);
  A::ConstRef cra16(ca);
  A::ConstRef cra17(cb);
  A::ConstRef cra18(cc);
  A::ConstRef cra19(ra);
  A::ConstRef cra20(rb);
  A::ConstRef cra21(rc);
  A::ConstRef cra22(cra);
  A::ConstRef cra23(crb);
  A::ConstRef cra24(crc);

  bool result;
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
  result = R != NULL; \
  result = R == NULL; \
  result = R;

  TESTCOMPOPS(pa)
  TESTCOMPOPS(pb)
  TESTCOMPOPS(pc)
  TESTCOMPOPS(cpa)
  TESTCOMPOPS(cpb)
  TESTCOMPOPS(cpc)
  TESTCOMPOPS(&a)
  TESTCOMPOPS(&b)
  TESTCOMPOPS(&c)
  TESTCOMPOPS(&ca)
  TESTCOMPOPS(&cb)
  TESTCOMPOPS(&cc)
  TESTCOMPOPS(a.toPtr())
  TESTCOMPOPS(b.toPtr())
  TESTCOMPOPS(c.toPtr())
  TESTCOMPOPS(ca.toPtr())
  TESTCOMPOPS(cb.toPtr())
  TESTCOMPOPS(cc.toPtr())
  TESTCOMPOPS(&ra)
  TESTCOMPOPS(&rb)
  TESTCOMPOPS(&rc)
  TESTCOMPOPS(ra.toPtr())
  TESTCOMPOPS(rb.toPtr())
  TESTCOMPOPS(rc.toPtr())
  TESTCOMPOPS(&cra)
  TESTCOMPOPS(&crb)
  TESTCOMPOPS(&crc)
  TESTCOMPOPS(cra.toPtr())
  TESTCOMPOPS(crb.toPtr())
  TESTCOMPOPS(crc.toPtr())

  pa = &a; cpa = &a;
  sassert(pa == cpa);
  pa = &a; cpa = &ca;
  sassert(pa != cpa);

}
