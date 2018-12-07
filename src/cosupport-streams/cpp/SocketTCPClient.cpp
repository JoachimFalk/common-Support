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

#include <CoSupport/Streams/SocketTCPClient.hpp>
#include <CoSupport/String/convert.hpp>

#include <sstream>
#include <stdexcept>
#include <cassert>

#include <string.h>
#include <errno.h>

#if !defined(_WIN32) && !defined(_WIN64)
# include <netinet/in.h>
# include <arpa/inet.h>
//#include <sys/select.h>
# include <netdb.h>
#endif // !defined(_WIN32) && !defined(_WIN64)

#include <boost/asio.hpp>
#include <boost/version.hpp>

namespace CoSupport { namespace Streams {

using CoSupport::String::asStr;
using CoSupport::String::strAs;

//using boost::asio::ip::tcp;

SocketTCPClient::SocketTCPClient(const char *host, uint16_t port)
  : sockClient(-1)
{
  std::ostringstream peerAddressStream;
  
  peerAddressStream << host << ":" << port;
  peerAddress = peerAddressStream.str();
  
  boost::asio::io_service io_service;
  
  boost::asio::ip::tcp::resolver            resolver(io_service);
  boost::asio::ip::tcp::resolver::query     query(host, asStr(port));
  
  boost::asio::ip::tcp::resolver::iterator  eiter = resolver.resolve(query);
  boost::asio::ip::tcp::resolver::iterator  eend;
  
  for (sockClient = -1;
       sockClient < 0 && eiter != eend;
       ++eiter) {
    boost::asio::ip::tcp::endpoint ep = *eiter;
    boost::asio::ip::tcp::endpoint::protocol_type prot = ep.protocol();
//  boost::asio::ip::address        addr = ep.address();
//  unsigned short                  port = ep.port();
    
    if ((sockClient = socket(prot.family(), prot.type(), 0)) >= 0) {
      if (connect(sockClient, ep.data(), ep.capacity()) >= 0) {
        break;
      }
//      throw std::runtime_error(std::string("connect: ") + strerror(errno));
#if !defined(_WIN32) && !defined(_WIN64)
      close(sockClient);
#else //defined(_WIN32) || defined(_WIN64)
      closesocket(sockClient);
#endif //defined(_WIN32) || defined(_WIN64)
      sockClient = -1;
    }
//    throw std::runtime_error(std::string("socket: ") + strerror(errno));
  }
  if (sockClient < 0)
    throw  boost::system::system_error(boost::asio::error::host_not_found);
#if BOOST_VERSION >= 104500
  in.open(boost::iostreams::file_descriptor_source(sockClient,
    boost::iostreams::never_close_handle));
  out.open(boost::iostreams::file_descriptor_sink(sockClient,
    boost::iostreams::never_close_handle));
#else
  in.open(boost::iostreams::file_descriptor_source(sockClient));
  out.open(boost::iostreams::file_descriptor_sink(sockClient));
#endif
}

void SocketTCPClient::shutdownWrite() {
  out.flush();
  // No more output
#if !defined(_WIN32) && !defined(_WIN64)
  shutdown(sockClient, SHUT_WR);
#else //defined(_WIN32) || defined(_WIN64)
  shutdown(sockClient, SD_SEND);
#endif //defined(_WIN32) || defined(_WIN64)
  out.setstate(out.badbit);
  // out.close(); danger this closes sockClient and therefore in too
}

SocketTCPClient::~SocketTCPClient() {
  assert(sockClient != -1);
#if !defined(_WIN32) && !defined(_WIN64)
  close(sockClient);
#else //defined(_WIN32) || defined(_WIN64)
  closesocket(sockClient);
#endif //defined(_WIN32) || defined(_WIN64)
}

} } // namespace CoSupport::Streams
