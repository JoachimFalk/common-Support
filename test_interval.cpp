#include <CoSupport/Math/Interval.hpp>

using namespace CoSupport::Math;

typedef Detail::IntervalLower<int8_t, true, true> SCLower;
typedef Detail::IntervalLower<uint8_t, true, false> UCLower;
typedef Detail::IntervalLower<signed int, true, true> SILower;
typedef Detail::IntervalLower<unsigned int, true, false> UILower;

#define EXPECT_PASS(T, I, C) \
  try { \
    typedef T IT; \
    T x I; C; \
  } catch (...) { \
    assert(!"WTF?! " #T " x " #I "; " #C "; failed!"); \
  }
#define EXPECT_FAIL(T, I, C, E) \
  try { \
    typedef T IT; \
    T x I; C; \
    assert(!"WTF?! " #T " x " #I "; " #C "; passed!"); \
  } catch (Exception::E) { \
  } catch (...) { \
    assert(!"WTF?! " #T " x " #I "; " #C "; failed with unexpected exception!"); \
  }

int main(int argc, char *argv[]) {

  EXPECT_PASS(SCLower, (IT::EMPTY), x += SCLower::INF-1);
  EXPECT_PASS(SCLower, (IT::ALL), x += (int16_t) 32767);
  EXPECT_PASS(SCLower, (IT::NINF), x += (int16_t) 32767);
  EXPECT_PASS(SCLower, (IT::NINF), x += (int16_t) -32768);
  EXPECT_PASS(SCLower, (IT::NINF+2), x += -1);
  EXPECT_FAIL(SCLower, (IT::NINF+1), x += -1, IntervalBoundUnderflow);
  EXPECT_PASS(SCLower, (IT::INF-1), x += -253);
  EXPECT_FAIL(SCLower, (IT::INF-1), x += -254, IntervalBoundUnderflow);
  EXPECT_PASS(SCLower, (IT::NINF+1), x += 253);
  EXPECT_FAIL(SCLower, (IT::NINF+1), x += 254, IntervalBoundOverflow);

  EXPECT_PASS(SCLower, (IT::INF-2), x += 1);
  EXPECT_FAIL(SCLower, (IT::INF-2), x += 2, IntervalBoundOverflow);

  EXPECT_PASS(SCLower, (IT::NINF+2), x -= (uint8_t) 1);
  EXPECT_FAIL(SCLower, (IT::NINF+1), x -= (uint8_t) 1, IntervalBoundUnderflow);
  EXPECT_PASS(SCLower, (IT::INF-1), x -= (uint8_t) 253);
  EXPECT_FAIL(SCLower, (IT::INF-1), x -= (uint8_t) 254, IntervalBoundUnderflow);
  EXPECT_PASS(SCLower, (IT::NINF+1), x -= (int16_t) -253);
  EXPECT_FAIL(SCLower, (IT::NINF+1), x -= (int16_t) -254, IntervalBoundOverflow);

  EXPECT_PASS(UCLower, (IT::INF-1), x += -254);
  EXPECT_FAIL(UCLower, (IT::INF-1), x += -255, IntervalBoundUnderflow);
  EXPECT_PASS(UCLower, (IT::INF-2), x += 1);
  EXPECT_FAIL(UCLower, (IT::INF-2), x += 2, IntervalBoundOverflow);
  EXPECT_PASS(UCLower, (IT::INF-1), x -= (uint8_t) 254);
  EXPECT_FAIL(UCLower, (IT::INF-1), x -= (uint8_t) 255, IntervalBoundUnderflow);

  EXPECT_PASS(SILower, (IT::NINF+1), x += std::numeric_limits<unsigned int>::max()-3);
  EXPECT_PASS(SILower, (IT::NINF+1), x += std::numeric_limits<unsigned int>::max()-2);
  EXPECT_FAIL(SILower, (IT::NINF+1), x += std::numeric_limits<unsigned int>::max()-1, IntervalBoundOverflow);
  EXPECT_PASS(SILower, (IT::INF-1), x -= std::numeric_limits<unsigned int>::max()-3);
  EXPECT_PASS(SILower, (IT::INF-1), x -= std::numeric_limits<unsigned int>::max()-2);
  EXPECT_FAIL(SILower, (IT::INF-1), x -= std::numeric_limits<unsigned int>::max()-1, IntervalBoundUnderflow);

  EXPECT_PASS(SILower, (IT::NINF+1), x += (long long) (std::numeric_limits<unsigned int>::max()-2));
  EXPECT_FAIL(SILower, (IT::NINF+1), x += (long long) (std::numeric_limits<unsigned int>::max()-1), IntervalBoundOverflow);
  EXPECT_PASS(SILower, (IT::NINF+1), x += (unsigned long) (std::numeric_limits<unsigned int>::max()-2));
  EXPECT_FAIL(SILower, (IT::NINF+1), x += (unsigned long) (std::numeric_limits<unsigned int>::max()-1), IntervalBoundOverflow);
  EXPECT_PASS(SILower, (IT::INF-std::numeric_limits<short>::max()), x += (short) (std::numeric_limits<short>::max()-1));
  EXPECT_FAIL(SILower, (IT::INF-std::numeric_limits<short>::max()), x += (short) (std::numeric_limits<short>::max()), IntervalBoundOverflow);
  EXPECT_PASS(SILower, (IT::INF-std::numeric_limits<unsigned short>::max()), x += (unsigned short) (std::numeric_limits<unsigned short>::max()-1));
  EXPECT_FAIL(SILower, (IT::INF-std::numeric_limits<unsigned short>::max()), x += (unsigned short) (std::numeric_limits<unsigned short>::max()), IntervalBoundOverflow);

  EXPECT_PASS(SILower, (IT::INF-1), x += -(long long) (std::numeric_limits<unsigned int>::max()-2));
  EXPECT_FAIL(SILower, (IT::INF-1), x += -(long long) (std::numeric_limits<unsigned int>::max()-1), IntervalBoundUnderflow);
  EXPECT_PASS(SILower, (IT::INF-1), x -= (unsigned long) (std::numeric_limits<unsigned int>::max()-2));
  EXPECT_FAIL(SILower, (IT::INF-1), x -= (unsigned long) (std::numeric_limits<unsigned int>::max()-1), IntervalBoundUnderflow);
  EXPECT_PASS(SILower, (IT::NINF+std::numeric_limits<short>::max()), x += -(short) (std::numeric_limits<short>::max()-1));
  EXPECT_FAIL(SILower, (IT::NINF+std::numeric_limits<short>::max()), x += -(short) (std::numeric_limits<short>::max()), IntervalBoundUnderflow);
  EXPECT_PASS(SILower, (IT::NINF+std::numeric_limits<unsigned short>::max()), x -= (unsigned short) (std::numeric_limits<unsigned short>::max()-1));
  EXPECT_FAIL(SILower, (IT::NINF+std::numeric_limits<unsigned short>::max()), x -= (unsigned short) (std::numeric_limits<unsigned short>::max()), IntervalBoundUnderflow);

  EXPECT_PASS(UILower, (IT::INF-1), x -= std::numeric_limits<unsigned int>::max()-2);
  EXPECT_PASS(UILower, (IT::INF-1), x -= std::numeric_limits<unsigned int>::max()-1);
  EXPECT_FAIL(UILower, (IT::INF-1), x -= std::numeric_limits<unsigned int>::max(), IntervalBoundUnderflow);

  EXPECT_PASS(UILower, (IT::INF-std::numeric_limits<short>::max()), x += (short) (std::numeric_limits<short>::max()-1));
  EXPECT_FAIL(UILower, (IT::INF-std::numeric_limits<short>::max()), x += (short) (std::numeric_limits<short>::max()), IntervalBoundOverflow);
  EXPECT_PASS(UILower, (IT::INF-std::numeric_limits<unsigned short>::max()), x += (unsigned short) (std::numeric_limits<unsigned short>::max()-1));
  EXPECT_FAIL(UILower, (IT::INF-std::numeric_limits<unsigned short>::max()), x += (unsigned short) (std::numeric_limits<unsigned short>::max()), IntervalBoundOverflow);
  EXPECT_PASS(UILower, (IT::INF-std::numeric_limits<int>::max()), x += (unsigned int) (std::numeric_limits<int>::max()-1));
  EXPECT_FAIL(UILower, (IT::INF-std::numeric_limits<int>::max()), x += (unsigned int) (std::numeric_limits<int>::max()), IntervalBoundOverflow);
  EXPECT_PASS(UILower, (IT::INF-std::numeric_limits<int>::max()), x += (int) (std::numeric_limits<int>::max()-1));
  EXPECT_FAIL(UILower, (IT::INF-std::numeric_limits<int>::max()), x += (int) (std::numeric_limits<int>::max()), IntervalBoundOverflow);
  EXPECT_PASS(UILower, (IT::INF-std::numeric_limits<int>::max()), x += (long long) (std::numeric_limits<int>::max()-1));
  EXPECT_FAIL(UILower, (IT::INF-std::numeric_limits<int>::max()), x += (long long) (std::numeric_limits<int>::max()), IntervalBoundOverflow);
  EXPECT_PASS(UILower, (IT::INF-std::numeric_limits<int>::max()), x += (unsigned long long) (std::numeric_limits<int>::max()-1));
  EXPECT_FAIL(UILower, (IT::INF-std::numeric_limits<int>::max()), x += (unsigned long long) (std::numeric_limits<int>::max()), IntervalBoundOverflow);
  EXPECT_PASS(UILower, (IT::INF-std::numeric_limits<int>::max()-1), x += (unsigned int) std::numeric_limits<int>::max()+0);
  EXPECT_FAIL(UILower, (IT::INF-std::numeric_limits<int>::max()-1), x += (unsigned int) std::numeric_limits<int>::max()+1, IntervalBoundOverflow);
  EXPECT_PASS(UILower, (IT::INF-std::numeric_limits<int>::max()-1), x += (long long) std::numeric_limits<int>::max()+0);
  EXPECT_FAIL(UILower, (IT::INF-std::numeric_limits<int>::max()-1), x += (long long) std::numeric_limits<int>::max()+1, IntervalBoundOverflow);
  EXPECT_PASS(UILower, (IT::INF-std::numeric_limits<int>::max()-1), x += (unsigned long long) std::numeric_limits<int>::max()+0);
  EXPECT_FAIL(UILower, (IT::INF-std::numeric_limits<int>::max()-1), x += (unsigned long long) std::numeric_limits<int>::max()+1, IntervalBoundOverflow);
  EXPECT_PASS(UILower, (IT::INF-std::numeric_limits<int>::max()+1), x += (unsigned int) (std::numeric_limits<int>::max()-2));
  EXPECT_FAIL(UILower, (IT::INF-std::numeric_limits<int>::max()+1), x += (unsigned int) (std::numeric_limits<int>::max()-1), IntervalBoundOverflow);
  EXPECT_PASS(UILower, (IT::INF-std::numeric_limits<int>::max()+1), x += (int) (std::numeric_limits<int>::max()-2));
  EXPECT_FAIL(UILower, (IT::INF-std::numeric_limits<int>::max()+1), x += (int) (std::numeric_limits<int>::max()-1), IntervalBoundOverflow);
  EXPECT_PASS(UILower, (IT::INF-std::numeric_limits<int>::max()+1), x += (long long) (std::numeric_limits<int>::max()-2));
  EXPECT_FAIL(UILower, (IT::INF-std::numeric_limits<int>::max()+1), x += (long long) (std::numeric_limits<int>::max()-1), IntervalBoundOverflow);
  EXPECT_PASS(UILower, (IT::INF-std::numeric_limits<int>::max()+1), x += (unsigned long long) (std::numeric_limits<int>::max()-2));
  EXPECT_FAIL(UILower, (IT::INF-std::numeric_limits<int>::max()+1), x += (unsigned long long) (std::numeric_limits<int>::max()-1), IntervalBoundOverflow);

  EXPECT_PASS(UILower, (std::numeric_limits<int>::max()-1), x -= (unsigned int) (std::numeric_limits<int>::max()-1));
  EXPECT_FAIL(UILower, (std::numeric_limits<int>::max()-1), x -= (unsigned int) (std::numeric_limits<int>::max()), IntervalBoundUnderflow);
  EXPECT_PASS(UILower, (std::numeric_limits<int>::max()-1), x -= (int) (std::numeric_limits<int>::max()-1));
  EXPECT_FAIL(UILower, (std::numeric_limits<int>::max()-1), x -= (int) (std::numeric_limits<int>::max()), IntervalBoundUnderflow);
  EXPECT_PASS(UILower, (std::numeric_limits<int>::max()-1), x -= (long long) (std::numeric_limits<int>::max()-1));
  EXPECT_FAIL(UILower, (std::numeric_limits<int>::max()-1), x -= (long long) (std::numeric_limits<int>::max()), IntervalBoundUnderflow);
  EXPECT_PASS(UILower, (std::numeric_limits<int>::max()-1), x -= (unsigned long long) (std::numeric_limits<int>::max()-1));
  EXPECT_FAIL(UILower, (std::numeric_limits<int>::max()-1), x -= (unsigned long long) (std::numeric_limits<int>::max()), IntervalBoundUnderflow);
  EXPECT_PASS(UILower, (std::numeric_limits<int>::max()+0), x -= (unsigned int) std::numeric_limits<int>::max()+0);
  EXPECT_FAIL(UILower, (std::numeric_limits<int>::max()+0), x -= (unsigned int) std::numeric_limits<int>::max()+1, IntervalBoundUnderflow);
  EXPECT_PASS(UILower, (std::numeric_limits<int>::max()+0), x -= (long long) std::numeric_limits<int>::max()+0);
  EXPECT_FAIL(UILower, (std::numeric_limits<int>::max()+0), x -= (long long) std::numeric_limits<int>::max()+1, IntervalBoundUnderflow);
  EXPECT_PASS(UILower, (std::numeric_limits<int>::max()+0), x -= (unsigned long long) std::numeric_limits<int>::max()+0);
  EXPECT_FAIL(UILower, (std::numeric_limits<int>::max()+0), x -= (unsigned long long) std::numeric_limits<int>::max()+1, IntervalBoundUnderflow);
  EXPECT_PASS(UILower, (std::numeric_limits<int>::max()-2), x -= (unsigned int) (std::numeric_limits<int>::max()-2));
  EXPECT_FAIL(UILower, (std::numeric_limits<int>::max()-2), x -= (unsigned int) (std::numeric_limits<int>::max()-1), IntervalBoundUnderflow);
  EXPECT_PASS(UILower, (std::numeric_limits<int>::max()-2), x -= (int) (std::numeric_limits<int>::max()-2));
  EXPECT_FAIL(UILower, (std::numeric_limits<int>::max()-2), x -= (int) (std::numeric_limits<int>::max()-1), IntervalBoundUnderflow);
  EXPECT_PASS(UILower, (std::numeric_limits<int>::max()-2), x -= (long long) (std::numeric_limits<int>::max()-2));
  EXPECT_FAIL(UILower, (std::numeric_limits<int>::max()-2), x -= (long long) (std::numeric_limits<int>::max()-1), IntervalBoundUnderflow);
  EXPECT_PASS(UILower, (std::numeric_limits<int>::max()-2), x -= (unsigned long long) (std::numeric_limits<int>::max()-2));
  EXPECT_FAIL(UILower, (std::numeric_limits<int>::max()-2), x -= (unsigned long long) (std::numeric_limits<int>::max()-1), IntervalBoundUnderflow);

  EXPECT_PASS(UILower, (IT::INF-1), x += -(long long) (std::numeric_limits<unsigned int>::max()-1));
  EXPECT_FAIL(UILower, (IT::INF-1), x += -(long long) (std::numeric_limits<unsigned int>::max()), IntervalBoundUnderflow);
  EXPECT_PASS(UILower, (IT::INF-1), x -= (unsigned long) (std::numeric_limits<unsigned int>::max()-1));
  EXPECT_FAIL(UILower, (IT::INF-1), x -= (unsigned long) (std::numeric_limits<unsigned int>::max()), IntervalBoundUnderflow);

  Interval<int, true> foo(12);
  foo = 13; foo += 1; foo -= 3;

  Interval<unsigned int, true> bar(12);
  bar = 13; bar += 1; bar -= 7;

  Interval<unsigned int, true> batz(Interval<unsigned int, true>::EMPTY);

  std::cout << "batz: " << batz << std::endl;

  Interval<int, true> hix(Interval<int, true>::ALL);

  Interval<long long, false> aa(bar);

  std::cout << "foo: " << foo << std::endl;
  std::cout << "hix: " << hix << std::endl;
  std::cout << "intersect(foo, hix): " << intersect(foo, hix) << std::endl;

  hix = 12;

  std::cout << "foo: " << foo << std::endl;
  std::cout << "hix: " << hix << std::endl;
  std::cout << "intersect(foo, hix): " << intersect(foo, hix) << std::endl;

  foo = Interval<int>(-13, 17);
  hix = Interval<int>(3, 31);

  std::cout << "foo: " << foo << std::endl;
  std::cout << "hix: " << hix << std::endl;
  std::cout << "intersect(foo, hix): " << intersect(foo, hix) << std::endl;
  std::cout << "intersect(hix, foo): " << intersect(hix, foo) << std::endl;

  return 0;
}
