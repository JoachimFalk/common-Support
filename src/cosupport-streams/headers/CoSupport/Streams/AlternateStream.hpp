// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2018 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2020 FAU -- Joachim Falk <joachim.falk@fau.de>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) any
 * later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#ifndef _INCLUDED_COSUPPORT_STREAMS_ALTERNATESTREAM_HPP
#define _INCLUDED_COSUPPORT_STREAMS_ALTERNATESTREAM_HPP

#include <CoSupport/compatibility-glue/nullptr.h>

#include <ostream>
#include <istream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <memory>

#include "export_config.h"

namespace CoSupport { namespace Streams {

struct COSUPPORT_STREAMS_API FileNotOpenException
  : public std::runtime_error
{
  FileNotOpenException(const char *file);
};

struct COSUPPORT_STREAMS_API FileNotGoodException
: public std::runtime_error
{
  FileNotGoodException();
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
    : Base(nullptr), cmp(cmp), obj(nullptr), def(&def) {}

  AlternateStream(Base &def, const std::string &file, const std::string &cmp)
    : Base(nullptr), cmp(cmp), obj(nullptr), def(&def) { open(file); }

  void open(const std::string &file) {
    if (file == cmp)
      open(def);
    else {
      std::unique_ptr<FStream> fs(new FStream(file.c_str()));
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
        obj = nullptr;
      throw;
    }
  }

  void close() {
    this->rdbuf(nullptr);
    if (obj != def)
      delete obj;
    obj = nullptr;
  }

  ~AlternateStream() { close(); }
};

typedef AlternateStream<std::ostream, std::ofstream> AOStream;
typedef AlternateStream<std::istream, std::ifstream> AIStream;

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_ALTERNATESTREAM_HPP
