#include <CoSupport/String/convert.hpp>
#include <CoSupport/String/TypeName.hpp>

#include <cassert>
#include <cstdint>

#include <iostream>
#include <limits>

using CoSupport::String::strAs;
using CoSupport::String::asStr;
using CoSupport::String::TypeName;

class Dummy {

};

std::ostream &operator <<(std::ostream &out, Dummy const &) {
  return out << "DUMMY";
}

template <typename T>
struct Underflow {
  static
  std::string value() {
    return "-" + asStr(static_cast<uint64_t>(-1*(static_cast<int64_t>(std::numeric_limits<T>::min()+1)))+2);
  }
};

template <typename T>
struct Overflow {
  static
  std::string value() {
    std::string value = asStr(std::numeric_limits<T>::max());
    int i;
    // decimal addition
    for (i = value.size()-1; i >= 0; --i) {
      switch (value[i]) {
        case '0': value[i] = '1'; i = -10; break;
        case '1': value[i] = '2'; i = -10; break;
        case '2': value[i] = '3'; i = -10; break;
        case '3': value[i] = '4'; i = -10; break;
        case '4': value[i] = '5'; i = -10; break;
        case '5': value[i] = '6'; i = -10; break;
        case '6': value[i] = '7'; i = -10; break;
        case '7': value[i] = '8'; i = -10; break;
        case '8': value[i] = '9'; i = -10; break;
        case '9': value[i] = '0'; break;
        default: assert(!"WTF?!");
      }
    }
    return i == -1
      ? "1" + value
      : value;
  }
};

template <typename T>
void testInteger() {
  T x;

  {
    std::string value = asStr(static_cast<int64_t>(std::numeric_limits<T>::min()));
    std::cout << "Testing value " << value << " with type " << TypeName<T>::name() << std::endl;
    x = strAs<T>(value);
    std::cout << "Result " << asStr(x) << std::endl;
    assert(x == std::numeric_limits<T>::min());
    if (x != std::numeric_limits<T>::min())
      throw std::runtime_error("test error");
  }
  {
    std::string value = Underflow<T>::value();
    std::cout << "Testing value " << value << " with type " << TypeName<T>::name() << std::endl;
    bool error = false;
    bool errorUnderflow = false;
    try {
      x = strAs<T>(value);
      std::cout << "Result " << asStr(x) << std::endl;
    } catch (CoSupport::String::InvalidConversionUnderflow const &) {
      errorUnderflow = true;
      std::cout << "Underflow error for value " << value << " and type " << TypeName<T>::name() << std::endl;
    } catch (CoSupport::String::InvalidConversion const &) {
      std::cout << "Conversion error for value " << value << " and type " <<TypeName<T>::name() << std::endl;
      error = true;
    }
    assert(
        (std::numeric_limits<T>::min() < 0 && errorUnderflow) ||
        (std::numeric_limits<T>::min() == 0 && error));
    if (!(std::numeric_limits<T>::min() < 0 && errorUnderflow) &&
        !(std::numeric_limits<T>::min() == 0 && error))
      throw std::runtime_error("test error");
  }
  {
    std::string value = asStr(static_cast<uint64_t>(std::numeric_limits<T>::max()));
    std::cout << "Testing value " << value << " with type " << TypeName<T>::name() << std::endl;
    x = strAs<T>(value);
    std::cout << "Result " << asStr(x) << std::endl;
    assert(x == std::numeric_limits<T>::max());
    if (x != std::numeric_limits<T>::max())
      throw std::runtime_error("test error");
  }
  {
    std::string value = asStr(Overflow<T>::value());
    std::cout << "Testing value " << value << " with type " << TypeName<T>::name() << std::endl;
    bool errorOverflow = false;
    try {
      x = strAs<T>(value);
      std::cout << "Result " << asStr(x) << std::endl;
    } catch (CoSupport::String::InvalidConversionOverflow const &) {
      errorOverflow = true;
      std::cout << "Overflow error for value " << value << " and type " << TypeName<T>::name() << std::endl;
    }
    assert(errorOverflow);
    if (!errorOverflow)
      throw std::runtime_error("test error");
  }
}

int main(int argc, char *argv[]) {
  testInteger<int8_t>();
  testInteger<uint8_t>();
  testInteger<int16_t>();
  testInteger<uint16_t>();
  testInteger<int32_t>();
  testInteger<uint32_t>();
  testInteger<int64_t>();
  testInteger<uint64_t>();
  testInteger<short>();
  testInteger<signed short>();
  testInteger<unsigned short>();
  testInteger<int>();
  testInteger<signed int>();
  testInteger<unsigned int>();
  testInteger<long>();
  testInteger<signed long>();
  testInteger<unsigned long>();
  testInteger<long long>();
  testInteger<signed long long>();
  testInteger<unsigned long long>();
  return 0;
}
