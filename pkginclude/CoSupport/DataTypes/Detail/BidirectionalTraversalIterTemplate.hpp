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

#ifndef _INCLUDED_COSUPPORT_DATATYPES_DETAIL_BIDIRECTIONALTRAVERSALITERTEMPLATE_HPP
#define _INCLUDED_COSUPPORT_DATATYPES_DETAIL_BIDIRECTIONALTRAVERSALITERTEMPLATE_HPP

#include <boost/iterator/iterator_facade.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/remove_const.hpp>

#include "../../Type/STLReferenceSelector.hpp"
#include "../../Type/STLPointerSelector.hpp"

namespace CoSupport { namespace DataTypes { namespace Detail {

  struct BidirectionalTraversalIterTemplateAccess;

  template <typename LISTFACADE>
  class BidirectionalTraversalIterTemplate
  : public boost::iterator_facade<
      BidirectionalTraversalIterTemplate<LISTFACADE>,
      typename LISTFACADE::value_type,
      boost::bidirectional_traversal_tag,
      typename Type::STLReferenceSelector<LISTFACADE>::type,
      typename LISTFACADE::difference_type> {
    typedef BidirectionalTraversalIterTemplate this_type;

    friend class BidirectionalTraversalIterTemplate<typename boost::remove_const<LISTFACADE>::type>;
    friend class BidirectionalTraversalIterTemplate<typename boost::add_const<LISTFACADE>::type>;
    friend class boost::iterator_core_access;
    friend struct BidirectionalTraversalIterTemplateAccess;
  private:
    typedef typename LISTFACADE::IterImpl IterImpl;
  public:
    // overwrite pointer type from boost which thinks it knows best
    typedef typename Type::STLPointerSelector<LISTFACADE>::type pointer;
  private:
    IterImpl iter;

    BidirectionalTraversalIterTemplate(const IterImpl &iter)
      : iter(iter) {}
  public:
    BidirectionalTraversalIterTemplate() {}

    BidirectionalTraversalIterTemplate(const BidirectionalTraversalIterTemplate<typename boost::remove_const<LISTFACADE>::type> &iter)
      : iter(iter.iter) {}

    // overwrite operator -> from boost which thinks it knows best
    pointer operator->() const {
      typename this_type::reference ref = dereference();
      return &ref;
    }
  protected:
    bool equal(const BidirectionalTraversalIterTemplate<typename boost::remove_const<LISTFACADE>::type> &n) const
      { return iter.equal(n.iter); }
    bool equal(const BidirectionalTraversalIterTemplate<typename boost::add_const<LISTFACADE>::type> &n) const
      { return iter.equal(n.iter); }
    void increment()
      { iter.next(); }
    void decrement()
      { iter.prev(); }
    typename this_type::reference dereference() const
      { return iter.deref(); }
  };

  struct BidirectionalTraversalIterTemplateAccess {
    template <class ITERTEMPLATE>
    static
    ITERTEMPLATE construct(const typename ITERTEMPLATE::IterImpl &iter)
      { return ITERTEMPLATE(iter); }
    template <class ITERTEMPLATE>
    static
    typename ITERTEMPLATE::IterImpl &retrieve(ITERTEMPLATE &iter)
      { return iter.iter; }
    template <class ITERTEMPLATE>
    static
    const typename ITERTEMPLATE::IterImpl &retrieve(const ITERTEMPLATE &iter)
      { return iter.iter; }
  };

} } } // namespace CoSupport::DataTypes::Detail

#endif // _INCLUDED_COSUPPORT_DATATYPES_DETAIL_BIDIRECTIONALTRAVERSALITERTEMPLATE_HPP
