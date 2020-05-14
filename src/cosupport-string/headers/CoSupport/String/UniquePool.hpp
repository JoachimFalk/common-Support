// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2012 brentrop <brentrop.tobias@gmail.com>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_STRING_UNIQUEPOOL_HPP
#define _INCLUDED_COSUPPORT_STRING_UNIQUEPOOL_HPP

#include <map>
#include <sstream>
#include <string>

#include "export_config.h"

namespace CoSupport { namespace String {

/**
 * Use this class to generate unique instances of names.  Should a <name>
 * already exists in the uniqe pool it will generate the same name postfixed
 * with an index starting from 1, i.e., <name>_<index>.
 */
class COSUPPORT_STRING_API
UniquePool {
private:
  typedef std::map<std::string, size_t> Pool;
private:
  Pool pool;
public:
  std::string operator ()(const std::string &str);
};
  
} } // namespace CoSupport::String

#endif // _INCLUDED_COSUPPORT_STRING_UNIQUEPOOL_HPP
