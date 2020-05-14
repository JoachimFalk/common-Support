// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2012 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#ifndef _INCLUDED_COSUPPORT_BOOST_GRAPH_UNDIRECT_GRAPH_HPP
#define _INCLUDED_COSUPPORT_BOOST_GRAPH_UNDIRECT_GRAPH_HPP

#include "../init_namespace.hpp"

#include <boost/graph/properties.hpp>
#include <boost/graph/graph_traits.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/variant.hpp>

#include <cassert>

namespace CoSupport { namespace boost {

  template <class G>
  class undirect_graph {
  protected:
    struct EdgeDesc {
      typename graph_traits<G>::edge_descriptor e;
      bool                                      swap;

      EdgeDesc() {}
      EdgeDesc(
          typename graph_traits<G>::edge_descriptor e,
          bool swap)
        : e(e), swap(swap) {}

      bool operator ==(const EdgeDesc &ed)
        { return e == ed.e; }
      bool operator !=(const EdgeDesc &ed)
        { return e != ed.e; }
    };

    template <bool IET>
    class IterTemplate
    : public boost::iterator_facade<
        IterTemplate<IET>,
        EdgeDesc,
        boost::forward_traversal_tag,
//      boost::bidirectional_traversal_tag,
//      boost::random_access_traversal_tag,
        EdgeDesc> {
    private:
      friend class boost::iterator_core_access;
      friend class undirect_graph<G>;

      typedef boost::variant<
        typename graph_traits<G>::out_edge_iterator,
        typename graph_traits<G>::in_edge_iterator>   IterCurr;

      typename graph_traits<G>::out_edge_iterator     iterOEnd;
      typename graph_traits<G>::in_edge_iterator      iterIBegin;
      IterCurr                                        iterCurr;

      struct DerefVisit {
        typedef EdgeDesc result_type;
        
        result_type operator()(const typename graph_traits<G>::out_edge_iterator &iter) const
          { return result_type(*iter, IET); }
        result_type operator()(const typename graph_traits<G>::in_edge_iterator &iter) const
          { return result_type(*iter, !IET); }
      };
      struct IncrementVisit {
        typedef bool result_type;

        IterTemplate<IET> &i;

        IncrementVisit(IterTemplate<IET> &i): i(i) {}

        result_type operator()(typename graph_traits<G>::out_edge_iterator &iter) const
          { return ++iter == i.iterOEnd; }
        result_type operator()(typename graph_traits<G>::in_edge_iterator &iter) const
          { ++iter; return false; }
      };

    public:
      IterTemplate() {}
      IterTemplate(
        typename graph_traits<G>::out_edge_iterator iterOEnd,
        typename graph_traits<G>::in_edge_iterator  iterIBegin,
        typename graph_traits<G>::out_edge_iterator iter)
        : iterOEnd(iterOEnd), iterIBegin(iterIBegin) {
        if (iter != iterOEnd)
          iterCurr = iter;
        else
          iterCurr = iterIBegin;
      }
      IterTemplate(
        typename graph_traits<G>::out_edge_iterator iterOEnd,
        typename graph_traits<G>::in_edge_iterator  iterIBegin,
        typename graph_traits<G>::in_edge_iterator  iter)
        : iterOEnd(iterOEnd), iterIBegin(iterIBegin), iterCurr(iter) {}

//      template <typename FF, typename VV>
//      int distance_to(IterTemplate<FF,VV> const &iter) const {
//        return (iter.i - f->rIndex).getValue() -
//               (     i - f->rIndex).getValue();
//      }
      bool equal(const IterTemplate &iter) const
        { return iterCurr == iter.iterCurr; }
//      void advance( int n )
//        { i += n; }
      void increment() {
        if (apply_visitor(IncrementVisit(*this), iterCurr))
          iterCurr = iterIBegin;
      }
//      void decrement()
//        { i--; }
      EdgeDesc dereference() const
        { return apply_visitor(DerefVisit(), iterCurr); }
    };
  public:
    typedef typename graph_traits<G>::vertex_descriptor       vertex_descriptor;
    //typedef typename graph_traits<G>::edge_descriptor         edge_descriptor;
    typedef EdgeDesc                                          edge_descriptor;
    typedef typename graph_traits<G>::adjacency_iterator      adjacency_iterator;
    //typedef typename graph_traits<G>::out_edge_iterator       out_edge_iterator;
    typedef IterTemplate<false>                               out_edge_iterator;
    //typedef typename graph_traits<G>::in_edge_iterator        in_edge_iterator;
    typedef IterTemplate<true>                                in_edge_iterator;
    typedef typename graph_traits<G>::vertex_iterator         vertex_iterator;
    typedef typename graph_traits<G>::edge_iterator           edge_iterator;
    
    //typedef typename graph_traits<G>::directed_category     directed_category;
    typedef boost::undirected_tag                             directed_category;
    typedef typename graph_traits<G>::edge_parallel_category  edge_parallel_category;
    typedef typename graph_traits<G>::traversal_category      traversal_category;
    
    typedef typename graph_traits<G>::vertices_size_type      vertices_size_type;
    typedef typename graph_traits<G>::edges_size_type         edges_size_type;
    typedef typename graph_traits<G>::degree_size_type        degree_size_type;
  public:
    G const &g;
  public:
    undirect_graph(G const &g)
      : g(g) {
      typedef typename boost::graph_traits<G>::directed_category directed;
      BOOST_STATIC_ASSERT((boost::is_convertible<directed, boost::directed_tag>::value)); 
    }

    static
    vertex_descriptor null_vertex() { return G::null_vertex(); }
  };

  using ::boost::num_vertices;
  template <class G>
  typename graph_traits<G>::vertices_size_type
  num_vertices(const undirect_graph<G> &g)
    { return num_vertices(g.g); }

  using ::boost::vertices;
  template <class G>
  std::pair<
    typename graph_traits<G>::vertex_iterator,
    typename graph_traits<G>::vertex_iterator >
  vertices(const undirect_graph<G> &g)
    { return vertices(g.g); }

  using ::boost::out_edges;
  template <class G>
  std::pair<
    typename undirect_graph<G>::out_edge_iterator,
    typename undirect_graph<G>::out_edge_iterator
  > out_edges(typename undirect_graph<G>::vertex_descriptor v,
              const undirect_graph<G> &g) {
    std::pair<
      typename graph_traits<G>::out_edge_iterator,
      typename graph_traits<G>::out_edge_iterator> p1 = out_edges(v, g.g);
    std::pair<
      typename graph_traits<G>::in_edge_iterator,
      typename graph_traits<G>::in_edge_iterator>  p2 = in_edges(v, g.g);
    return std::pair<
      typename undirect_graph<G>::out_edge_iterator,
      typename undirect_graph<G>::out_edge_iterator
    >(typename undirect_graph<G>::out_edge_iterator
        (p1.second, p2.first, p1.first),
      typename undirect_graph<G>::out_edge_iterator
        (p1.second, p2.first, p2.second));
  }

  using ::boost::in_edges;
  template <class G>
  std::pair<
    typename undirect_graph<G>::in_edge_iterator,
    typename undirect_graph<G>::in_edge_iterator
  > in_edges(typename undirect_graph<G>::vertex_descriptor v,
              const undirect_graph<G> &g) {
    std::pair<
      typename graph_traits<G>::out_edge_iterator,
      typename graph_traits<G>::out_edge_iterator> p1 = out_edges(v, g.g);
    std::pair<
      typename graph_traits<G>::in_edge_iterator,
      typename graph_traits<G>::in_edge_iterator>  p2 = in_edges(v, g.g);
    return std::pair<
      typename undirect_graph<G>::in_edge_iterator,
      typename undirect_graph<G>::in_edge_iterator
    >(typename undirect_graph<G>::in_edge_iterator
        (p1.second, p2.first, p1.first),
      typename undirect_graph<G>::in_edge_iterator
        (p1.second, p2.first, p2.second));
  }

  using ::boost::target;
  template <class G>
  typename undirect_graph<G>::vertex_descriptor
  target(const typename undirect_graph<G>::edge_descriptor &e,
         const undirect_graph<G> &g) {
    if (e.swap)
      return source(e.e, g.g);
    else
      return target(e.e, g.g);
  }

  using ::boost::source;
  template <class G>
  typename undirect_graph<G>::vertex_descriptor
  source(const typename undirect_graph<G>::edge_descriptor &e,
         const undirect_graph<G> &g) {
    if (e.swap)
      return target(e.e, g.g);
    else
      return source(e.e, g.g);
  }

  using ::boost::out_degree;
  template <class G>
  typename undirect_graph<G>::degree_size_type
  out_degree(const typename undirect_graph<G>::vertex_descriptor &v,
             const undirect_graph<G> &g) {
    return out_degree(v, g.g) + in_degree(v, g.g);
  }

  using ::boost::get;
  template <class G, typename P>
  typename property_map<undirect_graph<G>, P>::type
  get(P p, undirect_graph<G> &g)
    { return get(p, g.g); }
  template <class G, typename P>
  typename property_map<undirect_graph<G>, P>::const_type
  get(P p, const undirect_graph<G> &g)
    { return get(p, static_cast<const G &>(g.g)); }

} } // namespace CoSupport::boost

// Template specializations must be in same namespace as the template
// definition.
namespace boost {

  template <class G, typename P>
  struct property_map<CoSupport::boost::undirect_graph<G>, P>
    : public property_map<G,P> {};

} // namespace boost

#endif // _INCLUDED_COSUPPORT_BOOST_GRAPH_UNDIRECT_GRAPH_HPP
