// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <CoSupport/Streams/DebugStreambuf.hpp>

namespace CoSupport { namespace Streams {

Debug::Debug(size_t level) :
  level(level)
{}
  
Debug::Debug(size_t level, const std::string& prefix) :
  level(level), prefix(prefix)
{}

//bool Debug::isVisible(Debug const &dbg) const
//  { return dbg.level >= level; }

const Debug Debug::Low(0);
const Debug Debug::Medium(1);
const Debug Debug::High(2);
const Debug Debug::None(3);

std::ostream &operator << (std::ostream &os, const Debug &d) {
  DebugStreambuf *buf = static_cast<DebugStreambuf *>(os.pword(DebugStreambuf::index));
  if (buf)
    buf->setOutputLevel(d);
  return os;
}

ScopedDebug::ScopedDebug(std::ostream &os, const Debug &dbg)
  : dbgBuf(static_cast<DebugStreambuf *>(os.pword(DebugStreambuf::index))),
    oldDbg(dbgBuf ? dbgBuf->swapOutputLevel(dbg) : Debug::None) {}

ScopedDebug::~ScopedDebug()
{
  if (dbgBuf)
    dbgBuf->setOutputLevel(oldDbg);
}

DebugStreambuf::DebugStreambuf(
    const Debug &dbg,
    bool visible,
    std::streambuf *next) :
  FilterStreambuf(next),
  streamLevel(dbg.level),
  outputLevel(visible ? streamLevel : -1),
  newline(true)
{}

void DebugStreambuf::setStreamLevel(const Debug &dbg)
  { streamLevel = dbg.level; }

void DebugStreambuf::setOutputLevel(const Debug &dbg) {
  outputLevel = dbg.level;
  prefix      = dbg.prefix;
}

Debug DebugStreambuf::swapOutputLevel(const Debug &dbg) {
  Debug retval = outputLevel;
  setOutputLevel(dbg);
  return retval;
}

bool DebugStreambuf::isVisible(Debug const &dbg) const
  { return streamLevel <= dbg.level; }

int DebugStreambuf::overflow(int c) {
  if (streamLevel > outputLevel)
    return 1;

  if (newline) {
    next->sputn(prefix.c_str(), prefix.size());
    newline = false;
  }
  if (!newline && c == '\n') {
    newline = true;
  } 

  return next->sputc(c);
}

bool DebugStreambuf::hasManip() const
{ return true; }

int DebugStreambuf::getIndex() const
{ return index; }

const int DebugStreambuf::index(std::ostream::xalloc());

} } // namespace CoSupport::Streams
