#ifndef INCLUDED_COSUPPORT_HEX_OUTPUT_HPP
#define INCLUDED_COSUPPORT_HEX_OUTPUT_HPP

#include <ostream>

namespace CoSupport {

/**
 * @brief Conversion types for printHex function
 */
template<class T>
struct PrintHexTraits {
  /// @brief Type used to determine number of digits
  typedef T unsigned_type;
  /// @brief Type used to print number
  typedef T print_type;
};

/**
 * @brief Specialization of PrintHexTraits for uint8_t
 */
template<>
struct PrintHexTraits<uint8_t> {
  /// @brief Type used to determine number of digits
  typedef uint8_t unsigned_type;
  /// @brief Type used to print number
  typedef uint16_t print_type;
};

/**
 * @brief Specialization of PrintHexTraits for char
 */
template<>
struct PrintHexTraits<char> {
  /// @brief Type used to determine number of digits
  typedef uint8_t unsigned_type;
  /// @brief Type used to print number
  typedef uint16_t print_type;
};

/// @brief Prints integer as hex number
/// @param[in] os Target ostream
/// @param[in] t Number
template<class T>
void printHex(std::ostream &os, const T& t) {
  std::ios_base::fmtflags f = os.flags();
  os.setf(os.hex, os.basefield);
  os.setf(os.uppercase);

  std::streamsize w = os.width();
  os.width(std::numeric_limits<
      typename PrintHexTraits<T>::unsigned_type>::digits >> 2);

  char fl = os.fill();
  os.fill('0');

  os <<
    static_cast<typename PrintHexTraits<T>::print_type>(
    static_cast<typename PrintHexTraits<T>::unsigned_type>(t));

  os.flags(f);
  os.width(w);
  os.fill(fl);
}

/**
 * @brief Hex output manipulator class
 * @param T Integer type (short, long, etc.)
 */
template<class T>
struct hexManip {
  /// @brief Number
  T t;

  /// @brief True, if "0x" should be printed
  bool prefix;

  /// @brief Constructor
  /// @param[in] t Number
  /// @param[in] prefix True, if "0x" should be printed
  hexManip(const T& t, bool prefix = true) :
    t(t), prefix(prefix)
  {}
};

/// @brief Output operator for hexManip
/// @param[in] os Target ostream
/// @param[in] m Hex output manipulator
template<class T>
std::ostream& operator<<(std::ostream& os, const hexManip<T>& m)
{ if(m.prefix) os << "0x"; printHex<T>(os, m.t); return os; }

/// @brief Convenience function for creating an hex output manipulator
/// @param[in] t Number
/// @param[in] prefix True, if "0x" should be printed
/// @return Corresponding hex output manipulator object
template<class T>
hexManip<T> hex(T t, bool prefix = true)
{ return hexManip<T>(t, prefix); }

} // namespace CoSupport

#endif // INCLUDED_COSUPPORT_HEX_OUTPUT_HPP
