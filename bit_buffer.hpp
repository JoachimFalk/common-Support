// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2004-2006 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
 * 
 *   This library is free software; you can redistribute it and/or modify it under
 *   the terms of the GNU Lesser General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option) any
 *   later version.
 * 
 *   This library is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 *   details.
 * 
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software Foundation, Inc.,
 *   59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 * 
 * --- This software and any associated documentation is provided "as is" 
 * 
 * IN NO EVENT SHALL HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG
 * BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN
 * NUREMBERG HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG, SPECIFICALLY
 * DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
 * HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF
 * ERLANGEN NUREMBERG HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 */

#ifndef _INCLUDED_COSUPPORT_BIT_BUFFER_HPP
#define _INCLUDED_COSUPPORT_BIT_BUFFER_HPP

#include <cassert>
#include <string>
#include <iostream>

#include <limits.h>

namespace CoSupport {

/**
 * \brief Bit Buffer
 *
 * the bit_buffer class manages a (std::string based) buffer
 */
class bit_buffer 
{
private:
  typedef std::string   storage_type;
  typedef unsigned char primitive_type;
  
  /// size of storage primitive in bits
  static const size_t primitive_bits =
    sizeof(primitive_type) * CHAR_BIT;
  
public:
  /// storage for octet stream
  storage_type buffer;
public:
    /// read string from buffer
    std::string get_string()
    { return buffer; }

    /// get length of buffer
    size_t get_size()
    { return buffer.size(); }
    
    /// constructor for empty bit_buffer
    bit_buffer(size_t length) : 
      buffer(length / sizeof(primitive_type), 0)
    {}
    
    /// constructur with initial buffer value
    bit_buffer(std::string data) :
      buffer(data)
    {}
    
    /// generate bit mask (mask(3) --> 00000111)
    static size_t mask(size_t n)
    { return (1 << n) - 1; }
    
    /// get some bits out of x
    static primitive_type get_range(primitive_type x, size_t start, size_t length)
    { return (x >> start) & mask(length); }
    
    /// get least significant bits out of x
    static primitive_type get_lsb(primitive_type x, size_t length)
    { return x & mask(length); }
    
    /// get most significant bits out of x
    static primitive_type get_msb(primitive_type x, size_t length)
    { return x >> (primitive_bits - length); }
    
    /// read specified bits out of the buffer
    template<class T>
    T get_range(const size_t start, const size_t length) const {
      // read value from this bitbuffer
      // with offset start and length length
      //
      // not_prefix|prefix  middle  postfix|not_postfix
      //      ------XX     XXXXXXXX      XX------
      //      --XXXXX-     --------      --------
      // BIT  76543210     76543210      76543210
      // BYTE     0            1             2
      
      // this is the index of the last postfix bit
      const size_t end = start + length - 1;
      assert(end < buffer.size() * primitive_bits);
      
      // calculate character which contains the prefix bits
      const size_t start_byte = start / primitive_bits;
      
      // calculate character which contains the postfix bits
      const size_t end_byte = end / primitive_bits;
 
      // prefix bits
      const size_t prefix_bits = primitive_bits - start % primitive_bits;
      
      // postfix bits
      const size_t postfix_bits = end % primitive_bits + 1;
      
      /*std::cout << "start: " << start << "; length: " << length << "; end: " << end
                << "; start_byte: " << start_byte << "; end_byte: " << end_byte
                << "; prefix_bits: " << prefix_bits << "; postfix_bits: " << postfix_bits
                << std::endl;*/
      
      if(start_byte == end_byte)
        return T(get_range(buffer[start_byte], 8 - postfix_bits, length));
      
      // get the character which contains the prefix bits
      // and copy those into the return value
      size_t byte_index = start_byte;
      T ret(get_lsb(buffer[byte_index++], prefix_bits));
      size_t bits = length - prefix_bits;
      
      // copy whole bytes
      for(; bits >= primitive_bits; bits -= primitive_bits) {
        ret <<= primitive_bits;
        ret |= static_cast<primitive_type>(buffer[byte_index++]);
      }
      
      // assert remaining bits equals postfix_bits (which may already
      // be copied in the above for-loop)
      assert(bits == postfix_bits % 8);
      
      // copy remaining bits
      if(bits) {
        ret <<= bits;
        ret |= get_msb(buffer[byte_index], bits);
      }
      
      return ret;
    }
    
    /// write specified bits into the buffer
    template<class T>
    void set_range(T t, const size_t start, const size_t length) {
      // set value t into this bitbuffer
      // with offset start and length length
      //
      // not_prefix|prefix  middle  postfix|not_postfix
      //      ------XX     XXXXXXXX      XX------
      // BIT  76543210     76543210      76543210
      // BYTE     0            1             2
      
      // this is the index of the last postfix bit
      const size_t end = start + length - 1;
      assert(end < buffer.size() * primitive_bits);
      
      // calculate character which contains the prefix bits
      const size_t start_byte = start / primitive_bits;
      
      // calculate character which contains the postfix bits
      const size_t end_byte = end / primitive_bits;
 
      // prefix bits
      const size_t prefix_bits = primitive_bits - start % primitive_bits;
      
      // postfix bits
      const size_t postfix_bits = end % primitive_bits + 1;

      /*std::cout << "start: " << start << "; length: " << length << "; end: " << end
                << "; start_byte: " << start_byte << "; end_byte: " << end_byte
                << "; prefix_bits: " << prefix_bits << "; postfix_bits: " << postfix_bits
                << std::endl;*/
      
      // get the character which contains the postfix bits;
      // set postfix bits to zero and overwrite them with their
      // new value
      size_t byte_index = end_byte;
      primitive_type c = buffer[byte_index];
      c &= mask(primitive_bits - postfix_bits);
      c |= (t << (primitive_bits - postfix_bits));
      t >>= postfix_bits;
      int bits = length - postfix_bits;
      
      // copy whole bytes until we have copied all bits
      // (or copied too much)
      for(; bits > 0; bits -= primitive_bits) {
        buffer[byte_index--] = c;
        c = t;
        t >>= primitive_bits;
      }

      // fix prefix bytes
      if(bits < 0) {
        // assert -length == not_prefix_bits
        assert(primitive_bits + bits == prefix_bits);
        c &= mask(prefix_bits);
        c |= static_cast<primitive_type>(buffer[byte_index]) & ~mask(prefix_bits);
      }

      buffer[byte_index] = c;
    }
};

/**
 * \brief Bit Field
 *
 * a bit field represents a bit range within a
 * bit_buffer. this range can be read/written.
 *
 * a bit_field is initialized with a reference
 * to the buffer, a bit offset and a bit range
 * of the field
 */
template<typename T> class bit_field
{
public:
  bit_buffer &bb;
  size_t bo;
  size_t bl;

public:
  /// constructor
  bit_field(bit_buffer &bb, size_t bo, size_t bl) :
    bb(bb), bo(bo), bl(bl)
  {}
  
  /// write bit_field with value x
  void set(T x)
  { bb.template set_range<T>(x, bo, bl); }
  
  /// convenience method for setting this
  /// bit_field to specified value
  bit_field<T> &operator=(T x)
  { set(x); return *this; }
      
  /// read value from bit_field
  T get() const
  { return bb.template get_range<T>(bo, bl); }
  
  /// convenience method for reading the
  /// appropriate value out of this bit_field
  operator T() const
  { return get(); }
};


/**
 * \brief Bit Field (for enums)
 *
 * this class can be used to access enum values
 * (via static_cast)
 */
template<typename T> class bit_field_enum
{
public:
  bit_buffer &bb;
  size_t bo;
  size_t bl;

public:
  /// constructor
  bit_field_enum(bit_buffer &bb, size_t bo, size_t bl) :
    bb(bb), bo(bo), bl(bl)
  {}
  
  /// write bit_field with value x
  void set(T x)
  { bb.template set_range<unsigned int>(static_cast<unsigned int>(x), bo, bl); }
  
  /// convenience method for setting this
  /// bit_field to specified value
  bit_field_enum<T> &operator=(T x)
  { set(x); return *this; }
      
  /// read value from bit_field
  T get() const
  { return static_cast<T>(bb.template get_range<unsigned int>(bo, bl)); }
  
  /// convenience method for reading the
  /// appropriate value out of this bit_field
  operator T() const
  { return get(); }
};

} // namespace CoSupport

#endif // _INCLUDED_COSUPPORT_BIT_BUFFER_HPP
