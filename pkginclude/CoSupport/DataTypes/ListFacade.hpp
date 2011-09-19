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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_LISTFACADE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_LISTFACADE_HPP

#include "ListInterface.hpp"

namespace CoSupport { namespace DataTypes {

  // This is obsolete use ListInterface instead.
  template <
    class DERIVED, // The derived list container being constructed
    class ITER_,
    class VALUE,
    class REFERENCE = typename boost::add_reference<VALUE>::type,
    class CONSTREFERENCE = typename boost::add_reference<typename boost::add_const<VALUE>::type>::type,
    class PTR_ = typename boost::add_pointer<VALUE>::type,
    class CONSTPTR_ = typename boost::add_pointer<typename boost::add_const<VALUE>::type>::type
  >
  class  COSUPPORT_ATTRIBUTE_DEPRECATED ListFacade
  : public ListInterface<DERIVED, ITER_, VALUE, REFERENCE, CONSTREFERENCE, PTR_, CONSTPTR_>
  {
    typedef ListFacade                                                    this_type;
    typedef ListInterface
      <DERIVED, ITER_, VALUE, REFERENCE, CONSTREFERENCE, PTR_, CONSTPTR_> base_type;
  private:
    //
    // Curiously Recurring Template interface.
    //
    DERIVED &derived()
      { return *static_cast<DERIVED *>(this); }

    DERIVED const &derived() const
      { return *static_cast<DERIVED const *>(this); }
  public:
    typedef typename base_type::iterator        iterator;
    typedef typename base_type::const_iterator  const_iterator;

    iterator begin()
      { return base_type::template construct<iterator>(this->derived().first()); }
    const_iterator begin() const
      { return base_type::template construct<const_iterator>(this->derived().first()); }

    iterator end()
      { return base_type::template construct<iterator>(this->derived().last()); }
    const_iterator end() const
      { return base_type::template construct<const_iterator>(this->derived().last()); }

    iterator erase(const iterator &iter) {
      return base_type::template construct<iterator>
        (this->derived().del(base_type::template retrieve(iter)));
    }
    iterator insert(const iterator &iter, const typename this_type::value_type &v) {
      return base_type::template construct<iterator>
        (this->derived().add(base_type::template retrieve(iter), v));
    }
  };

} } // namespace CoSupport::DataTypes

#endif // _INCLUDED_COSUPPORT_DATATYPES_LISTFACADE_HPP
