#ifndef INCLUDED_COSUPPORT_FLOG2_HPP
#define INCLUDED_COSUPPORT_FLOG2_HPP

#include <stddef.h>
#include <stdint.h>

namespace CoSupport {

// Returns number of bits up to highest bit set in a number
// e.g. flog2(7) == 3 ("00...000111")
//      flog2(8) == 4 ("00...001000")
// (To obtain log2(m), one must calculate n = m - 1,
//  i.e. log2(m) == flog2(m - 1) !!!)

size_t flog2(uint32_t n);
size_t flog2(uint64_t n);

} // namespace CoSupport

#endif // INCLUDED_COSUPPORT_FLOG2_HPP
