// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c) 2004-2009 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
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

#ifndef _INCLUDED_COSUPPORT_BOOST_GRAPH_GRAPH_TRAITS_HPP
#define _INCLUDED_COSUPPORT_BOOST_GRAPH_GRAPH_TRAITS_HPP

#include "../init_namespace.hpp"

#include <set>
#include <boost/graph/graph_traits.hpp>

namespace CoSupport { namespace boost {

template <typename G>
class graph_traits: public ::boost::graph_traits<G> {
  typedef graph_traits<G> this_type;
public:
  typedef G                                               graph;
  typedef std::set<typename this_type::edge_descriptor>   edge_descriptor_set;
  typedef std::set<typename this_type::vertex_descriptor> vertex_descriptor_set;
  typedef std::pair<
    typename this_type::edge_iterator,
    typename this_type::edge_iterator>                    edge_iterator_pair;
  typedef std::pair<
    typename this_type::in_edge_iterator,
    typename this_type::in_edge_iterator>                 in_edge_iterator_pair;
  typedef std::pair<
    typename this_type::out_edge_iterator,
    typename this_type::out_edge_iterator>                out_edge_iterator_pair;
  typedef std::pair<
    typename this_type::vertex_iterator,
    typename this_type::vertex_iterator>                  vertex_iterator_pair;
};

} } // namespace CoSupport::boost

#endif // _INCLUDED_COSUPPORT_BOOST_GRAPH_GRAPH_TRAITS_HPP
