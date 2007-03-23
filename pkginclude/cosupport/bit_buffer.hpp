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

namespace CoSupport {

/**
 * \brief Bit Buffer
 *
 * the bit_buffer class manages a (std::string based) buffer
 */
class bit_buffer 
{
private:
  std::string buffer;
  /// should be 'char'
  typedef std::string::value_type Char;
  /// size of char (in bytes)
  static const size_t char_bytes = sizeof(Char);
  /// size of byte (in bits)
  static const size_t byte_bits = 8;
  /// size of char (in bits)
  static const size_t char_bits = char_bytes * byte_bits;

public:
    /// read string from buffer
    std::string get_string()
    { return buffer; }

    /// get length of buffer
    size_t get_size()
    { return buffer.size(); }
    
    /// constructor for empty bit_buffer
    bit_buffer(size_t length) : 
      buffer(length, 0)
    {}
    
    /// constructur with initial buffer value
    bit_buffer(std::string data) :
      buffer(data)
    {}
   
    /// calculate byte mask 76543210
    /// e.g n=3:            00000111
    static size_t mask(size_t n)
    { return (1<<n)-1; }
    
    /// calculate total bits in this buffer
    size_t num_bits() const
    { return buffer.size() * char_bits; }

    /// read specified bits out of the buffer
    template<class T>
    T get_range(size_t start, size_t length) const {
      assert(start + length <= num_bits());
      assert(sizeof(T) * byte_bits >= length);

      // calculate bits before the first byte boundary
      size_t prefix_bits = char_bits - start % char_bits;

      // calculate character which contains the prefix bits
      start /= char_bits;

      // get the character which contains the prefix bits
      // and copy those into the return value (maybe too
      // much bits are copied; this is handled in the return
      // statement)
      T ret(buffer[start++] & mask(prefix_bits));

      // calculate remaining bits to be copied (this needs
      // to be signed; so we cannot reuse length)
      int len = length - prefix_bits;

      // copy whole bytes until we have copied all bits
      // (or copied too much)
      for(; len > 0; len -= char_bits) {
        ret <<= char_bits;
        ret |= buffer[start++];
      }

      // if we did copy too much bits, shift the unnecessary
      // bits out of the return value
      return ret >>= -len;
    }
    
    /// write specified bits into the buffer
    template<class T>
    void set_range(T t, size_t start, size_t length) {
      assert(start + length <= num_bits());
      assert(sizeof(T) * byte_bits >= length);

      // calculate end bit
      size_t end = start + length - 1;

      // calculate bits following the last byte boundary
      size_t postfix_bits = end % char_bits + 1;

      // calculate character which contains the postfix bits
      end /= char_bits;

      // get the character which contains the postfix bits;
      // set postfix bits to zero and overwrite them with their
      // new value
      Char c = buffer[end];
      c &= mask(char_bits - postfix_bits);
      c |= (t << (char_bits - postfix_bits));
      t >>= postfix_bits;

      // calculate remaining bits to be copied (this needs
      // to be signed; so we cannot reuse length)
      int len = length - postfix_bits;

      // copy whole bytes until we have copied all bits
      // (or copied too much)
      for(; len > 0; len -= char_bits) {
        buffer[end--] = c;
        c = t;
        t >>= char_bits;
      }

      // fix prefix bytes
      if(len < 0) {
        size_t prefix_bits = char_bits + len;
        c &= mask(prefix_bits);
        c |= buffer[end] & ~mask(prefix_bits);
      }

      buffer[end] = c;
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
