// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <CoSupport/Streams/LineNumberStreambuf.hpp>

#include <CoSupport/String/convert.hpp>

namespace CoSupport { namespace Streams {

using String::asStr;
using String::strAs;
  
LineNumberStreambuf::LineNumberStreambuf(std::streambuf *next)
  : FilterStreambuf(next),
    line(1),
    newline(true) {}

int LineNumberStreambuf::overflow(int c) {
  if (newline) {
    std::string tmp = asStr(line) + ": ";
    next->sputn(tmp.c_str(), tmp.size());
    newline = false;
  }
  if (!newline && c == '\n') {
    line++;
    newline = true;
  }
  return next->sputc(c);
}

} } // namespace CoSupport::Streams
