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

#ifndef _INCLUDED_COSUPPORT_FILTER_OSTREAM_HPP
#define _INCLUDED_COSUPPORT_FILTER_OSTREAM_HPP

#include <streambuf>
#include <ostream>

namespace CoSupport {

/// forward declarations 
class FilterOstream;

/**
 * the base class for all custom streambuffers
 */  
class FilterStreambuf :
  public std::streambuf
{
protected:
  /// pointer to the next streambuffer in the buffer chain
  std::streambuf *next;
  
  /// must be able to set next
  friend class FilterOstream;
  
public:  
  /// constructs a new object, optionally with a target
  /// streambuffer
  FilterStreambuf(std::streambuf *next = 0);
  
public:
  /// should be reimplemented in derived classes: return true
  /// if stream manipulators are available
  virtual bool hasManip() const;
  
  /// returns the index for the ostream extensible array
  /// (obtained with std::ostream::xalloc())
  virtual int getIndex() const;
};


/**
 * prefixes each line with the current line number
 */
class LineNumberStreambuf :
  public FilterStreambuf
{
private:
  /// current line number (starting with 1)
  size_t line;
  
  /// indicator if newline was encountered
  bool newline;

public:
  /// constructs a new object
  LineNumberStreambuf(std::streambuf *next = 0);
  
protected:
  int overflow(int c);
};


/**
 * stream manipulator for the HeaderFooterStreambuf
 * streambuffer (set a new header)
 */
struct Header
{
  /// new header
  const char *value;
  
  /// constructs a new Header object
  Header(const char *value);
};


/**
 * stream manipulator for the HeaderFooterStreambuf
 * streambuffer (set a new footer)
 */
struct Footer
{
  // new footer
  const char *value;
  
  /// constructs a new Footer object
  Footer(const char *value);
};


/**
 * prefixes each line with a custom header string and
 * appends to each line a custom footer string
 */
class HeaderFooterStreambuf :
  public FilterStreambuf
{
private:
  /// currently used header string
  std::string header;

  /// currently used footer string
  std::string footer;
  
  /// indicator if newline was encountered 
  bool newline;
  
public:
  /// constructs a new object with the specified header
  /// and footer
  HeaderFooterStreambuf(
      const std::string &header = "",
      const std::string &footer = "",
      std::streambuf *next = 0);
  
  /// sets a new header
  void setHeader(const std::string &value);
  
  /// sets a new footer
  void setFooter(const std::string &value);
  
protected:
  int overflow(int c);
  
public:
  /// index obtained with std::ostream::xalloc
  static const int index;
   
  /// see Header and Footer
  bool hasManip() const;
  
  /// returns the (static) index
  int getIndex() const;
};

/// output operator for the Header manipulator
std::ostream& operator<<(std::ostream &os, const Header &p);

/// output operator for the Footer manipulator
std::ostream& operator<<(std::ostream &os, const Footer &p);


/**
 * stream manipulator for the ColorStreambuf custom
 * streambuffer
 */
struct Color
{
  /// console escape code for this color
  const char *escape;
  
  /// constructs a new object with the specified escape
  /// code
  Color(const char *escape);

  /// predefined colors 
  static const Color Auto;
  static const Color Black;
  static const Color Blue;
  static const Color Brown;
  static const Color Cyan;
  static const Color Gray;
  static const Color Green;
  static const Color Purple;
  static const Color Red;
  static const Color White;
  static const Color Yellow;
};

/**
 * colors each line with the specified color
 */
class ColorStreambuf :
  public FilterStreambuf
{
private:
  /// color of the printed chars
  Color color;

  /// color to set after newline
  Color reset;
  
  /// indicator if newline was encountered
  bool newline;
  
public:
  /// constructs a new object with the specified
  /// colors
  ColorStreambuf(
      const Color &color = Color::Auto,
      const Color &reset = Color::Auto,
      std::streambuf *next = 0);
  
  /// set a new line color
  void setColor(const Color &c);
  
  /// set a new reset color
  void setReset(const Color &c);
  
protected:
  int overflow(int c);
  
public:
  /// index obtained with std::ostream::xalloc
  static const int index;
 
  /// see Color 
  bool hasManip() const;
  
  /// returns the (static) index
  int getIndex() const;
};

/// output operator for the Color manipulator
std::ostream& operator<<(std::ostream &os, const Color &c);


/**
 * stream manipulator for the IndentStreambuf custom stream
 * buffer
 */
struct Indent
{
  /// how many levels to indent
  int delta_level;
  
  /// constructs a new object with the specified value
  Indent(int delta_level);
  
  /// predefined indentation values
  static const Indent Up;
  static const Indent Down;
};

/**
 * prefixes each line with spaces
 */
class IndentStreambuf :
  public FilterStreambuf
{
private:
  /// number of spaces to add for each indentation
  /// level
  size_t delta;

  /// current number of spaces to print for each line
  size_t indent;

  /// indicator if newline was encountered
  bool newline;

public:
  /// constructs a new object with the specified delta and
  /// initial indentation
  IndentStreambuf(
      size_t delta = 2,
      size_t indent = 0,
      std::streambuf *next = 0);
  
  /// modify the indentation (absolute value)
  void setIndentation(int value);
  
  /// modify the indentation (delta)
  void setDeltaLevel(int value);
  
protected:
  int overflow(int c);
  
public:
  /// index obtained with std::ostream::xalloc
  static const int index;
  
  /// see Indent
  bool hasManip() const;
  
  /// returns the (static) index
  int getIndex() const;
};

/// output operator for the Indent manipulator
std::ostream& operator<<(std::ostream &os, const Indent &i);

/**
 * stream manipulator for the DebugStreambuf custom
 * streambuffer
 */
struct Debug
{
  /// new debug level
  size_t level;
  
  /// constructs a new object with the specified debug
  /// level
  Debug(size_t level);
  
  /// predefined debug levels
  static const Debug Low;
  static const Debug Medium;
  static const Debug High;
  static const Debug None;
};

/**
 * prints or discards output according to the current
 * debug level
 */
class DebugStreambuf :
  public FilterStreambuf
{
private:
  /// current debug level
  size_t level;

  /// flag if output will be printend or not
  bool visible;
  
public:
  /// constructs a new object with the specified debug
  /// level
  DebugStreambuf(
      const Debug &dbg = Debug::None,
      std::streambuf *next = 0);
  
  /// set a new debug level
  void setLevel(const Debug &dbg);
  
  /// set the visibility based on the current debug
  /// level
  void setVisibility(const Debug &dbg);
  
protected:
  int overflow(int c);
  
public:
  /// index obtained with std::ostream::xalloc
  static const int index;
 
  /// see Debug 
  bool hasManip() const;
  
  /// returns the (static) index
  int getIndex() const;
};

/// output operator for the Debug manipulator
std::ostream& operator<<(std::ostream &os, const Debug &d);


/**
 * convenience class for managing a custom streambuffer
 * chain
 */
class FilterOstream :
  public std::ostream
{
public:
  /// construct a new object which uses the streambuffer
  /// of the specified stream as initial target
  FilterOstream(std::ostream &os);
  
  /// inserts a custom streambuffer (as first buffer in
  /// the chain)
  void insert(FilterStreambuf &head);
};

} // namespace CoSupport

#endif // _INCLUDED_COSUPPORT_FILTER_OSTREAM_HPP

