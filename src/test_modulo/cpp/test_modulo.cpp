#include <CoSupport/Math/modulo.hpp>
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
  assert((a1 += 3) == 0);
  assert((a2 += 4) == 2);
  assert((a3 += -1) == 3);
  assert((a1 -= a3) == 2);
  assert((a3 -= 4) == 4);
  assert((a2 -= a3) == 3);
}

template <typename T>
void testModDiv(T const n, T const m, T const d) {
  using CoSupport::Math::mod;
  using CoSupport::Math::div;
  assert(div(n,m) == d); assert(mod(n,m) == n-m*d);
}

int main(int argc, char *argv[]) {
  // ModuloGroup tests
  {
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
    {
      CoSupport::Math::ModuloGroup<CoSupport::Math::CTModulus<uint32_t, (1ULL<<32)-3> > dummy((1ULL<<32)-7);
      assert(dummy + 16 == 12);
      assert(dummy + 5 == 1);
      assert(dummy + 4 == 0);
      assert(dummy + 3 == (1ULL<<32)-4);
      assert(dummy - ((1ULL<<32)-4) == (1ULL<<32)-6);
      assert(dummy - ((1ULL<<32)-5) == (1ULL<<32)-5);
      assert(dummy - ((1ULL<<32)-6) == (1ULL<<32)-4);
      assert(dummy - ((1ULL<<32)-7) == 0);
      assert(dummy - ((1ULL<<32)-8) == 1);
    }
    {
      CoSupport::Math::ModuloGroup<CoSupport::Math::CTModulus<uint64_t, static_cast<uint64_t>(~0ULL)-2> > dummy(static_cast<uint64_t>(~0ULL)-6);
      assert(dummy + 16 == 12);
      assert(dummy + 5 == 1);
      assert(dummy + 4 == 0);
      assert(dummy + 3 == static_cast<uint64_t>(~0ULL)-3);
      assert(dummy - (static_cast<uint64_t>(~0ULL)-3) == static_cast<uint64_t>(~0ULL)-5);
      assert(dummy - (static_cast<uint64_t>(~0ULL)-4) == static_cast<uint64_t>(~0ULL)-4);
      assert(dummy - (static_cast<uint64_t>(~0ULL)-5) == static_cast<uint64_t>(~0ULL)-3);
      assert(dummy - (static_cast<uint64_t>(~0ULL)-6) == 0);
      assert(dummy - (static_cast<uint64_t>(~0ULL)-7) == 1);
    }
  }
  // mod and div test
  {
    using CoSupport::Math::mod;
    using CoSupport::Math::div;
    testModDiv<int>(1,2,0);
    testModDiv<unsigned char>(1,2,0);
    testModDiv<int>(-1,2,-1);
    testModDiv<int>(0,7,0);
    testModDiv<unsigned int>(0,7,0);
    testModDiv<int>(3,7,0);
    testModDiv<unsigned int>(3,7,0);
    testModDiv<int>(4,7,0);
    testModDiv<unsigned >(4,7,0);
    testModDiv<int>(6,7,0);
    testModDiv<unsigned short>(6,7,0);
    testModDiv<int>(7,7,1);
    testModDiv<unsigned long>(7,7,1);
    testModDiv<int>(8,7,1);
    testModDiv<signed char>(8,7,1);
    testModDiv<int>(-1,7,-1);
    testModDiv<int>(-6,7,-1);
    testModDiv<int>(-7,7,-1);
    testModDiv<int>(-8,7,-2);

    assert(div(-4, 2) == -2);
    assert(div(-3, 2) == -2);
    assert(div(-2, 2) == -1);
    assert(div(-1, 2) == -1);
    assert(div( 0, 2) ==  0);
    assert(div( 1, 2) ==  0);
    assert(div( 2, 2) ==  1);
    assert(div( 3, 2) ==  1);
    assert(div( 4, 2) ==  2);
    assert(div( 5, 2) ==  2);
    assert(div(-5,-2) ==  2);
    assert(div(-4,-2) ==  2);
    assert(div(-3,-2) ==  1);
    assert(div(-2,-2) ==  1);
    assert(div(-1,-2) ==  0);
    assert(div( 0,-2) ==  0);
    assert(div( 1,-2) == -1);
    assert(div( 2,-2) == -1);
    assert(div( 3,-2) == -2);
    assert(div( 4,-2) == -2);

    assert(mod(-4, 2) ==  0);
    assert(mod(-3, 2) ==  1);
    assert(mod(-2, 2) ==  0);
    assert(mod(-1, 2) ==  1);
    assert(mod( 0, 2) ==  0);
    assert(mod( 1, 2) ==  1);
    assert(mod( 2, 2) ==  0);
    assert(mod( 3, 2) ==  1);
    assert(mod( 4, 2) ==  0);
    assert(mod( 5, 2) ==  1);
    assert(mod(-5,-2) == -1);
    assert(mod(-4,-2) ==  0);
    assert(mod(-3,-2) == -1);
    assert(mod(-2,-2) ==  0);
    assert(mod(-1,-2) == -1);
    assert(mod( 0,-2) ==  0);
    assert(mod( 1,-2) == -1);
    assert(mod( 2,-2) ==  0);
    assert(mod( 3,-2) == -1);
    assert(mod( 4,-2) ==  0);

    testModDiv(-3,-7,0);
    testModDiv(-4,-7,0);
    testModDiv(-6,-7,0);
    testModDiv(-7,-7,1);
    testModDiv(-8,-7,1);
    testModDiv(1,-7,-1);
    testModDiv(6,-7,-1);
    testModDiv(7,-7,-1);
    testModDiv(8,-7,-2);

    // Now test in overflow region
    testModDiv<int8_t>(127, -13, -10);
    testModDiv<int8_t>(127, -14, -10);
    testModDiv<int8_t>(127, -15, -9);
    testModDiv<int8_t>(127, -16, -8);
    testModDiv<int8_t>(127, -17, -8);
    testModDiv<int8_t>(-128, 13, -10);
    testModDiv<int8_t>(-128, -13, 9);
    testModDiv<int8_t>(-128, 16, -8);
    testModDiv<int8_t>(-128, 17, -8);
    testModDiv<int8_t>(-128, 18, -8);
    testModDiv<int8_t>(-128, 19, -7);

    // Now test in overflow region
    testModDiv<int16_t>(32767, -13, -2521);
    testModDiv<int16_t>(32767, -14, -2341);
    testModDiv<int16_t>(32767, -15, -2185);
    testModDiv<int16_t>(32767, -16, -2048);
    testModDiv<int16_t>(32767, -17, -1928);
    testModDiv<int16_t>(-32768, 13, -2521);
    testModDiv<int16_t>(-32768, -13, 2520);
    testModDiv<int16_t>(-32768, 16, -2048);
    testModDiv<int16_t>(-32768, 17, -1928);
    testModDiv<int16_t>(-32768, 18, -1821);
    testModDiv<int16_t>(-32768, 19, -1725);

    // Now test in overflow region
    testModDiv<int32_t>(2147483647, -13, -165191050);
    testModDiv<int32_t>(2147483647, -14, -153391690);
    testModDiv<int32_t>(2147483647, -15, -143165577);
    testModDiv<int32_t>(2147483647, -16, -134217728);
    testModDiv<int32_t>(2147483647, -17, -126322568);
    testModDiv<int32_t>(-2147483648, 13, -165191050);
    testModDiv<int32_t>(-2147483648, -13, 165191049);
    testModDiv<int32_t>(-2147483648, 16, -134217728);
    testModDiv<int32_t>(-2147483648, 17, -126322568);
    testModDiv<int32_t>(-2147483648, 18, -119304648);
    testModDiv<int32_t>(-2147483648, 19, -113025456);

    // Now test in overflow region
    testModDiv<int64_t>(9223372036854775807LL, -1651961050LL, -5583286626LL);
    testModDiv<int64_t>(9223372036854775807LL, -1533910690LL, -6012978525LL);
    testModDiv<int64_t>(9223372036854775807LL, -2143165577LL, -4303620839LL);
    testModDiv<int64_t>(9223372036854775807LL, -1349217728LL, -6836088680LL);
    testModDiv<int64_t>(9223372036854775807LL, -1263292568LL, -7301057784LL);
    testModDiv<int64_t>(-9223372036854775808ULL, 3165191050LL, -2914001680LL);
    testModDiv<int64_t>(-9223372036854775808ULL,-3165191050LL,  2914001679LL);
    testModDiv<int64_t>(-9223372036854775808ULL, 1374217728LL, -6711725405LL);
    testModDiv<int64_t>(-9223372036854775808ULL, 1263229568LL, -7301421904LL);
    testModDiv<int64_t>(-9223372036854775808ULL, 1199304648LL, -7690599759LL);
    testModDiv<int64_t>(-9223372036854775808ULL, 9113025456LL, -1012108666LL);
  }
  return 0;
}
