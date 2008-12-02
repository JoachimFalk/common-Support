
#include <cassert>

#include <iostream>

#include <CoSupport/String/TypeName.hpp>

using namespace CoSupport::String;

template <typename T>
void check_typename_for(const std::string &expected) {
  std::string retval = TypeName<T>::name();
  std::cout << "TypeName<" << expected << ">::name(): \"" << retval << "\"" << std::endl;
  assert(retval == expected);
}

#define CHECK_TYPENAME_FOR(TYPE) \
  check_typename_for<TYPE>(#TYPE)

int main(int argc, char *argv[]) {
  CHECK_TYPENAME_FOR(char);
  CHECK_TYPENAME_FOR(const char);
  CHECK_TYPENAME_FOR(char *);
  CHECK_TYPENAME_FOR(const char *);
  CHECK_TYPENAME_FOR(char *const);
  CHECK_TYPENAME_FOR(const char *const);
  CHECK_TYPENAME_FOR(char **);
  CHECK_TYPENAME_FOR(const char **);
  CHECK_TYPENAME_FOR(char *const *);
  CHECK_TYPENAME_FOR(const char *const *);
  CHECK_TYPENAME_FOR(char **const);
  CHECK_TYPENAME_FOR(const char **const);
  CHECK_TYPENAME_FOR(char *const *const);
  CHECK_TYPENAME_FOR(const char *const *const);
  CHECK_TYPENAME_FOR(char &);
  CHECK_TYPENAME_FOR(const char &);
  CHECK_TYPENAME_FOR(char * &);
  CHECK_TYPENAME_FOR(const char * &);
  CHECK_TYPENAME_FOR(char *const &);
  CHECK_TYPENAME_FOR(const char *const &);
  CHECK_TYPENAME_FOR(char ** &);
  CHECK_TYPENAME_FOR(const char ** &);
  CHECK_TYPENAME_FOR(char *const * &);
  CHECK_TYPENAME_FOR(const char *const * &);
  CHECK_TYPENAME_FOR(char **const &);
  CHECK_TYPENAME_FOR(const char **const &);
  CHECK_TYPENAME_FOR(char *const *const &);
  CHECK_TYPENAME_FOR(const char *const *const &);
  return 0;
}
