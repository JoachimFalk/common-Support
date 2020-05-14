// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
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
