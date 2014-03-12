//#include <CoSupport/Math/modulo.hpp>
#include <CoSupport/Math/ModuloGroup.hpp>

#include <stdint.h>

template <typename T1, typename T2, typename T3>
void testModuloGroup(T1 const &m1 = T1(), T2 const &m2 = T2(), T3 const &m3 = T3()) {
  CoSupport::Math::ModuloGroup<T1> a1(2, m1);
  CoSupport::Math::ModuloGroup<T2> a2(3, m2);
  CoSupport::Math::ModuloGroup<T3> a3(-1, m3);
  assert(a1 == 2);
  assert(a2 == 3);
  assert(a3 == 4);
  // + - constant test
  assert(a1 -  1 == 1);
  assert(a1 -  2 == 0);
  assert(a1 -  3 == 4);
  assert(a1 -  4 == 3);
  assert(a1 + -1 == 1);
  assert(a1 + -2 == 0);
  assert(a1 + -3 == 4);
  assert(a1 + -4 == 3);
  assert(a1 +  1 == 3);
  assert(a1 +  2 == 4);
  assert(a1 +  3 == 0);
  assert(a1 +  4 == 1);
  assert(a1 - -1 == 3);
  assert(a1 - -2 == 4);
  assert(a1 - -3 == 0);
  assert(a1 - -4 == 1);
  // between test
  assert(a1.between(a1, a1));
  assert(a1.between(a1, a2));
  assert(a1.between(a1, a3));
  assert(a1.between(a1, a1));
  assert(a1.between(a2, a1));
  assert(a1.between(a3, a1));
  assert(a2.between(a2, a1));
  assert(a2.between(a2, a2));
  assert(a2.between(a2, a3));
  assert(a2.between(a1, a2));
  assert(a2.between(a2, a2));
  assert(a2.between(a3, a2));
  assert(a3.between(a3, a1));
  assert(a3.between(a3, a2));
  assert(a3.between(a3, a3));
  assert(a3.between(a1, a3));
  assert(a3.between(a2, a3));
  assert(a3.between(a3, a3));
  // assert(a1 == 2);
  // assert(a2 == 3);
  // assert(a3 == 4);
  assert(!a1.between(a2, a3));
  assert(a1.between(a3, a2));
  assert(a2.between(a1, a3));
  assert(!a2.between(a3, a1));
  assert(!a3.between(a1, a2));
  assert(a3.between(a2, a1));
  // a1 ++ -- test
  assert(a1++ == 2);
  assert(a1 == 3);
  assert(a1-- == 3);
  assert(a1 == 2);
  assert(++a1 == 3);
  assert(a1 == 3);
  assert(--a1 == 2);
  assert(a1 == 2);
  // a2 ++ -- test
  assert(a2++ == 3);
  assert(a2 == 4);
  assert(a2-- == 4);
  assert(a2 == 3);
  assert(++a2 == 4);
  assert(a2 == 4);
  assert(--a2 == 3);
  assert(a2 == 3);
  // a3 ++ -- test
  assert(a3++ == 4);
  assert(a3 == 0);
  assert(a3-- == 0);
  assert(a3 == 4);
  assert(++a3 == 0);
  assert(a3 == 0);
  assert(--a3 == 4);
  assert(a3 == 4);
  // + - test
  assert(a1 + a2 == a3 - a3);
  assert(a1 + a3 == a3 - a2);
  assert(a2 + a3 == a1);
  assert(a1 - a2 == a3);
  assert(a1 - a3 == a2);
  assert(a2 - a3 == a3);
  // += test
  // assert(a1 == 2);
  // assert(a2 == 3);
  // assert(a3 == 4);
  assert(a1 += 3 == 0);
  assert(a2 += 4 == 2);
  assert(a3 += -1 == 3);
  assert(a1 -= a3 == 2);
  assert(a3 -= 4 == 4);
  assert(a2 -= a3 == 3);
}

int main(int argc, char *argv[]) {
  // ModuloGroup tests
  testModuloGroup<
    CoSupport::Math::CTModulus<int, 5>,
    CoSupport::Math::CTModulus<int, 5>,
    CoSupport::Math::CTModulus<int, 5> >();
  testModuloGroup<
    CoSupport::Math::CTModulus<int, 5>,
    CoSupport::Math::CTModulus<int, 5>,
    CoSupport::Math::Modulus<int>
  >(CoSupport::Math::CTModulus<int, 5>(),
    CoSupport::Math::CTModulus<int, 5>(),
    CoSupport::Math::Modulus<int>(5));
  testModuloGroup<
    CoSupport::Math::CTModulus<int, 5>,
    CoSupport::Math::Modulus<int>,
    CoSupport::Math::CTModulus<int, 5>
  >(CoSupport::Math::CTModulus<int, 5>(),
    CoSupport::Math::Modulus<int>(5),
    CoSupport::Math::CTModulus<int, 5>());
  testModuloGroup<
    CoSupport::Math::Modulus<int>,
    CoSupport::Math::CTModulus<int, 5>,
    CoSupport::Math::CTModulus<int, 5>
  >(CoSupport::Math::Modulus<int>(5),
    CoSupport::Math::CTModulus<int, 5>(),
    CoSupport::Math::CTModulus<int, 5>());

  testModuloGroup<
    CoSupport::Math::Modulus<int>,
    CoSupport::Math::Modulus<int>,
    CoSupport::Math::Modulus<int>
  >(CoSupport::Math::Modulus<int>(5),
    CoSupport::Math::Modulus<int>(5),
    CoSupport::Math::Modulus<int>(5));
  testModuloGroup<
    CoSupport::Math::Modulus<int>,
    CoSupport::Math::Modulus<int>,
    CoSupport::Math::CTModulus<int, 5>
  >(CoSupport::Math::Modulus<int>(5),
    CoSupport::Math::Modulus<int>(5),
    CoSupport::Math::CTModulus<int, 5>());
  testModuloGroup<
    CoSupport::Math::Modulus<int>,
    CoSupport::Math::CTModulus<int, 5>,
    CoSupport::Math::Modulus<int>
  >(CoSupport::Math::Modulus<int>(5),
    CoSupport::Math::CTModulus<int, 5>(),
    CoSupport::Math::Modulus<int>(5));
  testModuloGroup<
    CoSupport::Math::CTModulus<int, 5>,
    CoSupport::Math::Modulus<int>,
    CoSupport::Math::Modulus<int>
  >(CoSupport::Math::CTModulus<int, 5>(),
    CoSupport::Math::Modulus<int>(5),
    CoSupport::Math::Modulus<int>(5));

  testModuloGroup<
    CoSupport::Math::CTModulus<unsigned int, 5>,
    CoSupport::Math::CTModulus<unsigned int, 5>,
    CoSupport::Math::CTModulus<unsigned int, 5> >();
  testModuloGroup<
    CoSupport::Math::CTModulus<int, 5>,
    CoSupport::Math::CTModulus<unsigned int, 5>,
    CoSupport::Math::CTModulus<int, 5> >();
  {
    CoSupport::Math::ModuloGroup<CoSupport::Math::CTModulus<int16_t, (1<<15)-3> > dummy((1<<15)-7);
    assert(dummy + 16 == 12);
    assert(dummy + 5 == 1);
    assert(dummy + 4 == 0);
    assert(dummy + 3 == (1<<15)-4);
    assert(dummy - ((1<<15)-4) == (1<<15)-6);
    assert(dummy - ((1<<15)-5) == (1<<15)-5);
    assert(dummy - ((1<<15)-6) == (1<<15)-4);
    assert(dummy - ((1<<15)-7) == 0);
    assert(dummy - ((1<<15)-8) == 1);
  }
  {
    CoSupport::Math::ModuloGroup<CoSupport::Math::CTModulus<uint16_t, (1<<16)-3> > dummy((1<<16)-7);
    assert(dummy + 16 == 12);
    assert(dummy + 5 == 1);
    assert(dummy + 4 == 0);
    assert(dummy + 3 == (1<<16)-4);
    assert(dummy - ((1<<16)-4) == (1<<16)-6);
    assert(dummy - ((1<<16)-5) == (1<<16)-5);
    assert(dummy - ((1<<16)-6) == (1<<16)-4);
    assert(dummy - ((1<<16)-7) == 0);
    assert(dummy - ((1<<16)-8) == 1);
  }
  return 0;
}
