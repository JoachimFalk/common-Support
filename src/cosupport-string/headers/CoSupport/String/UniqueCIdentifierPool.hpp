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

#ifndef _INCLUDED_COSUPPORT_STRING_UNIQUECIDENTIFIERPOOL_HPP
#define _INCLUDED_COSUPPORT_STRING_UNIQUECIDENTIFIERPOOL_HPP

#include <map>
#include <sstream>
#include <string>

#include "UniquePool.hpp"
#include "export_config.h"

namespace CoSupport { namespace String {

/**
 * Use this class to generate unique C identifiers from a name. Should <name>
 * not be a C identifier, then it will be modified. Should the modified name
 * already exists in the pool, then it will generate the modified same name
 * postfixed with an index starting from 1, that is <modified name>_<index>.
 */
class COSUPPORT_STRING_API
UniqueCIdentifierPool: public UniquePool {
  typedef UniquePool base_type;
public:
  /// Return a string derived from name which is modified to be
  /// a valid C indentifier.
  static std::string makeCIdentifier(const std::string &str);

  std::string operator ()(const std::string &str)
    { return base_type::operator()(makeCIdentifier(str)); }
};
  
} } // namespace CoSupport::String

#endif // _INCLUDED_COSUPPORT_STRING_UNIQUECIDENTIFIERPOOL_HPP
