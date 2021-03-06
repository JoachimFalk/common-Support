// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_STREAMS_ITERATOR_DUMP_HPP
#define _INCLUDED_COSUPPORT_STREAMS_ITERATOR_DUMP_HPP

#include <iostream>
#include <list>

namespace CoSupport { namespace Streams {

/**
 * dumps elements from iterator b to iterator e
 * format is "<Prefix><Element><Suffix>"
 * first prefix can be omitted with skip_first, last suffix can be omitted with skip_last
 */
template<class ITER, class OUT, class PRE, class SUF>
OUT& dump(ITER b, ITER e, OUT &o, const PRE &p, bool skip_first, const SUF &s, bool skip_last)
{
  bool first = true;
  for(; b != e; ++b) {
    if(!first)
      o << s;
    if(!first || !skip_first)
      o << p;
    o << *b;
    first = false;
  }
  if(!skip_last)
    o << s;
  return o;
}

/**
 * dumps elements from iterator b to iterator e
 * format is "<Element>[<Seperator><Element>...]"
 */
template<class ITER, class OUT, class SEP>
OUT& dump(ITER b, ITER e, OUT &o, const SEP &s)
{
  bool first = true;
  for(; b != e; ++b) {
    if(!first)
      o << s;
    o << *b;
    first = false;
  }
  return o;
}

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_ITERATOR_DUMP_HPP
