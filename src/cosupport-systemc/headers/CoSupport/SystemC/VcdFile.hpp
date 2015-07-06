// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2004-2011 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_SYSTEMC_VCDFILE_HPP
#define _INCLUDED_COSUPPORT_SYSTEMC_VCDFILE_HPP

#include <CoSupport/compatibility-glue/nullptr.h>

#include <systemc.h>

#include <boost/shared_ptr.hpp>

namespace CoSupport { namespace SystemC {

/**
 * wrapper for a VCD file
 * the VCD file is automatically closed by destructor
 */
class VcdFile: public NonCopyable
{
public:
  typedef boost::shared_ptr<VcdFile> Ptr;

  VcdFile(const std::string fileName) :
    vcdFile_(nullptr)
  {
    vcdFile_ = sc_create_vcd_trace_file(fileName.c_str());
    vcdFile_->set_time_unit(1, SC_PS);
  }

  virtual ~VcdFile()
  {
    assert(vcdFile_ != nullptr);
    sc_close_vcd_trace_file(vcdFile_);
  }

  template<typename T>
  boost::shared_ptr<sc_signal<T> > getOrCreateTraceSignal(
      const std::string signal)
  {
    // shared between all instances
    static std::map<std::string, boost::shared_ptr<sc_signal<T> > > signalMap;
    if (signalMap.find(signal) == signalMap.end()) {
      boost::shared_ptr < sc_signal<T> > sig(new sc_signal<T> );
      signalMap[signal] = sig;

      sc_trace(vcdFile_, *sig, signal);
    }
    return signalMap[signal];
  }
private:
  sc_trace_file * vcdFile_;
};

} } // namespace CoSupport::SystemC

#endif // _INCLUDED_COSUPPORT_SYSTEMC_VCDFILE_HPP
