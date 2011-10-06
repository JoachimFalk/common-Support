/* vim: set sw=2 ts=8: */
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

#ifndef _INCLUDED_COSUPPORT_STREAMS_STL_OUTPUT_FOR_VECTOR_HPP
#define _INCLUDED_COSUPPORT_STREAMS_STL_OUTPUT_FOR_VECTOR_HPP

#include <ostream>
//#include <vector>

namespace CoSupport { namespace DataTypes {

  template<
    class DERIVED,
    class ITER_,
    class VALUE,
    class REFERENCE,
    class CONSTREFERENCE,
    class PTR_,
    class CONSTPTR_
  >
  class VectorInterface;

} } // namespace CoSupport::DataTypes

namespace std {

  template <class T, class A> class vector;
                                                                                  
  template <typename T, class A>
  std::ostream &operator << (std::ostream &out, const std::vector<T,A> &l) {
    out << "[Vector:";
    for (typename std::vector<T,A>::const_iterator iter = l.begin();
         iter != l.end();
         ++iter )
      out << (iter == l.begin() ? "" : ", ") << *iter;
    out << "]";
    return out;
  }

  template<
    class D, class I,
    class V, class R, class CR, class P, class CP
  >
  std::ostream &operator << (
      std::ostream &out,
      const CoSupport::DataTypes::VectorInterface<D,I,V,R,CR,P,CP> &l)
  {
    out << "[Vector:";
    for (typename CoSupport::DataTypes::VectorInterface<D,I,V,R,CR,P,CP>::const_iterator iter = l.begin();
         iter != l.end();
         ++iter )
      out << (iter == l.begin() ? "" : ", ") << *iter;
    out << "]";
    return out;
  }

} // namespace std

#endif // _INCLUDED_COSUPPORT_STREAMS_STL_OUTPUT_FOR_VECTOR_HPP
