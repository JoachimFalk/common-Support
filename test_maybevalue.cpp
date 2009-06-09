
#include <iostream>
#include <cassert>

#include <CoSupport/DataTypes/MaybeValue.hpp>

using namespace CoSupport::DataTypes;

int main(int argc, char *argv[]) {
  MaybeValue<int> foo;
  MaybeValue<int> bar(17);

  std::cout << "foo: " << foo << std::endl;
  std::cout << "bar: " << bar << std::endl;

  assert(!foo.isDefined());

  foo = 13;
  std::cout << "foo: " << foo << std::endl;

  assert(foo.isDefined());
  assert(foo.get() == 13);

  foo = boost::blank();
  std::cout << "foo: " << foo << std::endl;

  assert(!foo.isDefined());
  assert(bar.isDefined());
  assert(bar.get() == 17);

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

  return 0;
}
