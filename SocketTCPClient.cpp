// vim: set sw=2 ts=8:
/*
 * Copyright (c) 2004-2008 Hardware-Software-CoDesign, University of
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

#include <sstream>
#include <stdexcept>

#include <assert.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <arpa/inet.h>
//#include <sys/select.h>
#include <netdb.h>

namespace CoSupport { namespace Streams {

SocketTCPClient::SocketTCPClient(const char *host, uint16_t port)
  : sockClient(-1)
{
  std::ostringstream peerAddressStream;
  
  peerAddressStream << host << ":" << port;
  peerAddress = peerAddressStream.str();
  
  if ((sockClient = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    throw std::runtime_error(std::string("socket: ") + strerror(errno));
  
  addrinfo  hints;
  addrinfo *res;
  int err;
  
  memset(&hints, 0, sizeof(hints));
  hints.ai_family   = PF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = 0;
  
  if ((err = getaddrinfo(host, 0, &hints, &res)) != 0)
    throw std::runtime_error(std::string("getaddrinfo: ") + gai_strerror(err));
  
  for (addrinfo *i = res; i; i = i->ai_next) {
    //std::cerr << "Found address" << std::endl;
    switch (i->ai_family) {
      case PF_INET: {
        //std::cerr << " Family: IPv4" << std::endl;
        
        // get address
        sockaddr_in* sa = (sockaddr_in*)i->ai_addr;
        assert(sa->sin_family == i->ai_family);
        //std::cerr << " Address: " << inet_ntoa(sa->sin_addr) << std::endl;
        
        // fill in port
        sa->sin_port = htons(port);
        
        // server (i.e. simulation) has to run prior to this call
        if (connect(sockClient, i->ai_addr, sizeof(sockaddr_in)) < 0)
          throw std::runtime_error(std::string("connect: ") + strerror(errno));
        
        break;
      }
      default:
//        std::cerr << " Unknown family: " << i->ai_family << std::endl;
        break;
    }
  }
  
  freeaddrinfo(res);
  
  if (sockClient == -1)
    throw std::runtime_error("Could not connect to server");
  in.open(boost::iostreams::file_descriptor_source(sockClient));
  out.open(boost::iostreams::file_descriptor_sink(sockClient));
}

SocketTCPClient::~SocketTCPClient() {
  assert(sockClient != -1);
  close(sockClient);
}

} } // namespace CoSupport::Streams
