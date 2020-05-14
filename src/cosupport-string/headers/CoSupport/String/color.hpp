// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2017 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_STRING_COLOR_HPP
#define _INCLUDED_COSUPPORT_STRING_COLOR_HPP

#include <string>

#include <stdint.h>

#include "export_config.h"

namespace CoSupport { namespace String {

  class COSUPPORT_STRING_API
  Color {
    typedef Color this_type;
  public:
    Color(uint32_t color = 0)
      : color(color) {}
    Color(unsigned char r, unsigned char g, unsigned char b)
      : color(
          (static_cast<uint32_t>(r)      ) |
          (static_cast<uint32_t>(g) <<  8) |
          (static_cast<uint32_t>(b) << 16)) {}

    unsigned char r() const
      { return (color      ) & 0xFF; }
    unsigned char g() const
      { return (color >>  8) & 0xFF; }
    unsigned char b() const
      { return (color >> 16) & 0xFF; }

    bool operator <(this_type const &rhs) const
      { return color < rhs.color; }

    bool operator ==(this_type const &rhs) const
      { return color == rhs.color; }
    bool operator !=(this_type const &rhs) const
      { return color != rhs.color; }
  protected:
    uint32_t color;
  };

  COSUPPORT_STRING_API
  Color lookupColor(char const *const  colorName);

  COSUPPORT_STRING_API
  Color lookupColor(std::string const &colorName);

  COSUPPORT_STRING_API
  Color getColor(unsigned int colorIndex);

} } // namespace CoSupport::String

#endif // _INCLUDED_COSUPPORT_STRING_COLOR_HPP
