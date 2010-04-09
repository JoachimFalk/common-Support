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

#ifndef _INCLUDED_COSUPPORT_STREAMS_DEBUGOSTREAM_HPP
#define _INCLUDED_COSUPPORT_STREAMS_DEBUGOSTREAM_HPP

#include "FilterOStream.hpp"
#include "IndentStreambuf.hpp"
#include "HeaderFooterStreambuf.hpp"
#include "ColorStreambuf.hpp"
#include "DebugStreambuf.hpp"
#include <iostream>

namespace CoSupport { namespace Streams {

class DebugOStream: public FilterOStream {
public:
  IndentStreambuf        indenter;
  HeaderFooterStreambuf  headerFooter;
  ColorStreambuf         colorer;
  DebugStreambuf         debuglevel;

  /// construct a new object which uses the streambuffer
  /// of the specified stream as initial target
  DebugOStream(std::ostream &os, const Debug &dbg = Debug::Low)
    : FilterOStream(os),
      debuglevel(dbg)
  {
    //std::cerr << "Enter DebugOStream(std::ostream &os, const Debug &dbg)" << std::endl;
    insert(indenter);
    insert(headerFooter);
    insert(colorer);
    insert(debuglevel);
    //std::cerr << "Leave DebugOStream(std::ostream &os, const Debug &dbg)" << std::endl;
  }

  /// set a new debug level
  void setLevel(const Debug &dbg)
    { return debuglevel.setLevel(dbg); }

};

extern DebugOStream dout;

} } // namespace CoSupport::Streams

#endif // _INCLUDED_COSUPPORT_STREAMS_DEBUGOSTREAM_HPP
