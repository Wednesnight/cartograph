/**
 * This file is part of cartograph, a library for handling tile-based game maps
 * Copyright (C) 2008 Jens Finkhaeuser <unwesen@users.sourceforge.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * If this license is unacceptable to you or your business, please contact the
 * author with your specific requirements.
 **/

#include <cassert>
#include <cstdlib>
#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>


namespace cartograph {
namespace pathfinding {


namespace detail {

/**
 * The open list is a bit more complicated. In some instances we need to quickly
 * find a node by it's coordinates, in other's by it's cost. Either way, once
 * we've found it, we need it's g_cost, coordinates, parent. So the following
 * structure makes for a good value type regardless.
 *
 * For lookup, we'll keep a multi_index_container of pointers to the structure.
 **/
struct ol_entry_t
{
  ol_entry_t(vector_t const & coords, unit_t const & g_cost, unit_t const & f_cost,
      boost::shared_ptr<ol_entry_t> parent)
    : m_coords(coords)
    , m_g_cost(g_cost)
    , m_f_cost(f_cost)
    , m_parent(parent)
  {
  }

  vector_t                      m_coords;
  unit_t                        m_g_cost;
  unit_t                        m_f_cost;
  boost::weak_ptr<ol_entry_t>   m_parent;
};
typedef boost::shared_ptr<ol_entry_t> ol_entry_ptr;

struct vector_index {};
struct f_cost_index {};
typedef boost::multi_index_container<
  ol_entry_ptr,
  boost::multi_index::indexed_by<
    boost::multi_index::ordered_unique<
      boost::multi_index::tag<vector_index>,
      boost::multi_index::member<ol_entry_t, vector_t, &ol_entry_t::m_coords>
    >,
    boost::multi_index::ordered_non_unique<
      boost::multi_index::tag<f_cost_index>,
      boost::multi_index::member<ol_entry_t, unit_t, &ol_entry_t::m_f_cost>
    >
  >
> open_list_t;

/**
 * The closed list for the pathfinder is fairly simple in nature: we just need
 * to stuff all visited coordinates into it. But since removing ancestors from
 * the open list would take them out of scope and release them, we'd end up with
 * only the last node in the path - we must store the shared pointer as well.
 **/
typedef std::map<vector_t, ol_entry_ptr> closed_list_t;


/**
 * The pathfinder implements the A* algorithm proper, and keeps state between
 * iterations.
 **/
template <
  typename traversal_traitsT,
  typename node_groupT
>
struct pathfinder
{
  // Convenience typedefs
  typedef typename node_groupT::tile_traits_t tile_traits_t;

  typedef boost::function<
    unit_t (node_groupT const &, vector_t const &, vector_t const &,
        vector_t const &, traversal_traitsT &)
  > heuristic_t;

  typedef typename open_list_t::index<vector_index>::type vector_index_t;
  typedef typename open_list_t::index<f_cost_index>::type f_cost_index_t;


  // ctor
  pathfinder(node_groupT const & group, vector_t const & start,
      vector_t const & end, traversal_traitsT & traversal_traits,
      heuristic_t heuristic)
    : m_group(group)
    , m_start(start)
    , m_end(end)
    , m_traversal_traits(traversal_traits)
    , m_heuristic(heuristic)
    , m_join_types(m_traversal_traits.join_types())
  {
  }


  /**
   * Main entry point into the algorithm. Sets up the start node, and starts
   * processing nodes from there...
   **/
  error_t
  find_path(std::deque<vector_t> & result)
  {
    // For the start node, the F cost is equal to H, as G is zero.
    unit_t h_cost = m_heuristic(m_group, m_start, m_start, m_end,
        m_traversal_traits);

    ol_entry_ptr current_ptr = ol_entry_ptr(
        new ol_entry_t(m_start, 0, h_cost, ol_entry_ptr())
      );

    m_new_open_list.insert(current_ptr);

    do {
      // Drop current entry from open list...
      vector_index_t & ol_vector_index = m_new_open_list.get<vector_index>();
      vector_index_t::iterator v_iter = ol_vector_index.find(current_ptr->m_coords);
      if (v_iter != ol_vector_index.end()) {
        ol_vector_index.erase(v_iter);
      }

      // ... and add it to closed list.
      m_closed_list.insert(std::make_pair(current_ptr->m_coords, current_ptr));

      // Iterate over adjacents nodes.
      typename node_groupT::node current_node = m_group(current_ptr->m_coords);
      directions_t const * const dirs = current_node.available_dirs(m_join_types);
      for (directions_t const * d = dirs ; *d != DIR_END ; ++d) {

        // Must be valid - because we got the dirs from available_dirs().
        vector_t n_coords = tile_traits_t::get_relative(current_ptr->m_coords, *d);
        assert(n_coords != invalid_vector);

        // Success! We've found the end node!
        if (n_coords == m_end) {
          // Walk backwards from here to the start node. Since we walk backwards,
          // we push coordinates to the front of the deque so it later on becomes
          // iteratable front-to-back.
          while (current_ptr) {
            result.push_front(current_ptr->m_coords);
            current_ptr = current_ptr->m_parent.lock();
          }
          result.push_back(m_end);
          return CG_OK;
        }

        // Not an end node, we may process this further...
        //
        // Only consider nodes that are actually on the map.
        if (m_group.is_empty(n_coords)) {
          continue;
        }

        // Ignore impassable nodes
        if (m_traversal_traits.is_impassable(current_ptr->m_coords, *d)) {
          continue;
        }

        // Skip nodes on the closed list.
        if (m_closed_list.find(n_coords) != m_closed_list.end()) {
          continue;
        }

        // Traversal traits contains terrain cost.
        unit_t g_cost = current_ptr->m_g_cost
          + m_traversal_traits.traversal_cost(current_ptr->m_coords, *d);

        // If we find the same node in the open list with a higher g_cost, we want
        // to replace that entry with a new one. That way we'll find the lowest
        // cost for reaching each tested node.
        v_iter = ol_vector_index.find(n_coords);
        if (v_iter != ol_vector_index.end()) {
          if ((*v_iter)->m_g_cost > g_cost) {
            // Found a cheaper one, erase the current entry.
            ol_vector_index.erase(v_iter);
          } else {
            // Found a more or equally expensive entry... we're finished for this
            // node, it's already on the open list in the best possible config.
            continue;
          }
        }

        // We've either just erased info from the open list for this node, or it's
        // newly encountered... either way, we'll add it to the open_list again
        // with it's proper f_cost
        unit_t h_cost = m_heuristic(m_group, m_start, n_coords, m_end,
            m_traversal_traits);

        unit_t f_cost = g_cost + h_cost;

        ol_entry_ptr node_ptr = ol_entry_ptr(
            new ol_entry_t(n_coords, g_cost, f_cost, current_ptr)
          );
        m_new_open_list.insert(node_ptr);
      }

      // After pushing all neighbours to the open list, we now need to find the
      // node on the open list with the lowest F cost of them all..
      f_cost_index_t & ol_f_cost_index = m_new_open_list.get<f_cost_index>();
      f_cost_index_t::iterator f_iter = ol_f_cost_index.lower_bound(0);

      // Great, let's process this one further.
      current_ptr = *f_iter;
    } while (true);
  }

  node_groupT const & m_group;
  vector_t const &    m_start;
  vector_t const &    m_end;

  traversal_traitsT & m_traversal_traits;
  heuristic_t         m_heuristic;

  join_t              m_join_types;

  open_list_t         m_new_open_list;
  closed_list_t       m_closed_list;
};


} // namespace detail


template <
  typename node_groupT,
  typename traversal_traitsT,
  typename heuristicT
>
error_t
a_star(std::deque<vector_t> & result, node_groupT const & group,
    vector_t const & start, vector_t const & end,
    traversal_traitsT & traversal_traits,
    heuristicT const & heuristic)
{
#ifndef CG_DISABLE_CONCEPT_CHECKS
  boost::function_requires<
    concepts::TraversalTraitsConcept<traversal_traitsT>
  >();

  boost::function_requires<
    concepts::HeuristicConcept<node_groupT, traversal_traitsT, heuristicT>
  >();
#endif


  // Prevent bogus input.
  if (!group.is_valid(start) || !group.is_valid(end)) {
    return CG_INVALID_COORDS;
  }

  typedef detail::pathfinder<traversal_traitsT, node_groupT> pathfinder_t;

  pathfinder_t pathfinder(group, start, end, traversal_traits, heuristic);

  return pathfinder.find_path(result);
}


}} // namespace cartograph::pathfinding
