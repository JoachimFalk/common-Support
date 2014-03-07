//#include <CoSupport/Math/modulo.hpp>
#include <CoSupport/Math/ModuloGroup.hpp>

template <typename T1, typename T2, typename T3>
void testModuloGroup() {
  T1 a1(2);
  T2 a2(3);
  T3 a3(-1);
  assert(a1 == 2);
  assert(a2 == 3);
  assert(a3 == 4);
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
}

int main(int argc, char *argv[]) {
  // ModuloGroup tests
  testModuloGroup<
    CoSupport::Math::ModuloGroup<CoSupport::Math::CTModulus<int, 5> >,
    CoSupport::Math::ModuloGroup<CoSupport::Math::CTModulus<int, 5> >,
    CoSupport::Math::ModuloGroup<CoSupport::Math::CTModulus<int, 5> > >();
  testModuloGroup<
    CoSupport::Math::ModuloGroup<CoSupport::Math::CTModulus<unsigned int, 5> >,
    CoSupport::Math::ModuloGroup<CoSupport::Math::CTModulus<unsigned int, 5> >,
    CoSupport::Math::ModuloGroup<CoSupport::Math::CTModulus<unsigned int, 5> > >();
  testModuloGroup<
    CoSupport::Math::ModuloGroup<CoSupport::Math::CTModulus<int, 5> >,
    CoSupport::Math::ModuloGroup<CoSupport::Math::CTModulus<unsigned int, 5> >,
    CoSupport::Math::ModuloGroup<CoSupport::Math::CTModulus<int, 5> > >();
  return 0;
}
