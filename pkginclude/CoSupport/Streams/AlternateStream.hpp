// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2004-2009 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_STREAMS_ALTERNATESTREAM_HPP
#define _INCLUDED_COSUPPORT_STREAMS_ALTERNATESTREAM_HPP

#include <ostream>
#include <istream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <memory>

namespace CoSupport { namespace Streams {

  struct FileNotOpenException : public std::runtime_error {
    FileNotOpenException(const char* file) 
      : std::runtime_error(std::string("Could not open '") + file + "'") {}
  };

  struct FileNotGoodException : public std::runtime_error {
    FileNotGoodException() 
      : std::runtime_error("AlternateStream opened is not good!") {}
  };

  template<class Base, class FStream>
  struct AlternateStream
  : public Base {
  private:
    std::string  cmp;
    Base        *obj;
    Base        *def;
  public:
    AlternateStream(Base &def, const std::string &cmp)
      : Base(NULL), cmp(cmp), obj(NULL), def(&def) {}

    AlternateStream(Base &def, const std::string &file, const std::string &cmp)
      : Base(NULL), cmp(cmp), obj(NULL), def(&def) { open(file); }

    void open(const std::string &file) {
      if (file == cmp)
        open(def);
      else {
        std::auto_ptr<FStream> fs(new FStream(file.c_str()));
        if (!fs->is_open())
          throw FileNotOpenException(file.c_str());
        open(fs.release());
      }
    }

    void open(Base *fs) {
      try {
        if (!fs->good())
          throw FileNotGoodException();
        close();
        obj = fs;
        this->rdbuf(obj->rdbuf());
      } catch (...) {
        if (fs != def)
          delete fs;
        if (obj == fs)
          obj = NULL;
        throw;
      }
    }

    void close() {
      this->rdbuf(NULL);
      if (obj != def)
        delete obj;
      obj = NULL;
    }

    ~AlternateStream() { close(); }
  };

  typedef AlternateStream<std::ostream, std::ofstream> AOStream;
  typedef AlternateStream<std::istream, std::ifstream> AIStream;

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_ALTERNATESTREAM_HPP
