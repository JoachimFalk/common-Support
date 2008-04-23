#ifndef INCLUDED_COSUPPORT_STRING_HASH_HPP
#define INCLUDED_COSUPPORT_STRING_HASH_HPP

namespace CoSupport {

namespace Detail {

// calculates smallest x = 2^k; x > N  (e.g. 29 -> 32, 32 -> 64)
template<uint64_t N, uint64_t S = 1>
struct NextPower2
{ static const size_t value = NextPower2<N >> 1, S << 1>::value; };

template<uint64_t S>
struct NextPower2<0, S>
{ static const uint64_t value = S; };

} // namespace Detail

// http://isthe.com/chongo/tech/comp/fnv/
// (BITS <= 64)
template<size_t BITS>
class FNV {
public:
  typedef uint64_t result_type;

  template<class N>
  result_type operator()(N n) const
  { return xorFold(fnv(n)); }

private:
  static const result_type MASK = (1ull << BITS) - 1ull;

  FNV<Detail::NextPower2<BITS>::value> fnv;

  result_type xorFold(result_type h) const
  { return (h >> BITS) ^ (h & MASK); }
};

// http://isthe.com/chongo/tech/comp/fnv/
template<class T, T INIT, T PRIME>
class FNVBase {
public:
  typedef T result_type;

  template<class N>
  result_type operator()(N n) const {
    result_type h = INIT;
    while(*n) {
      h *= PRIME;
      h ^= *n++;
    }
    return h;
  }
};

// http://isthe.com/chongo/tech/comp/fnv/
template<>
class FNV<32> : public FNVBase<
  uint32_t,
  0x811C9DC5ul,
  0x01000193ul>
{};

// http://isthe.com/chongo/tech/comp/fnv/
template<>
class FNV<64> : public FNVBase<
  uint64_t,
  0xCBF29CE484222325ull,
  0x00000100000001B3ull>
{};

} // namespace CoSupport

#endif // INCLUDED_COSUPPORT_STRING_HASH_HPP
