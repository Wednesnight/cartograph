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

#ifndef CG_HEURISTICS_H
#define CG_HEURISTICS_H

#include <cartograph/types.h>

namespace cartograph {
namespace pathfinding {
namespace heuristics {

/**
 * Always returns zero. The A* pathfinding algorithm essentially turns into
 * Dijkstras algorithm if there are no heuristics values. Yields accurate
 * results, but is not terribly fast.
 **/
template <
  typename node_groupT,
  typename traversal_traitsT
>
unit_t
dijkstra(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits);


/**
 * Implements Manhattan heuristics, which assume that the shortest path between
 * two nodes is to walk first walk the difference between start and end node on
 * one axis, then on the other.
 * Due to it's nature, it's *only* usable for rectangular (square, really)
 * tiles where no diagonal movement is allowed.
 * Note: will not compile with anything but rectangular_tile_traits.
 **/
template <
  typename node_groupT,
  typename traversal_traitsT
>
unit_t
manhattan(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits);


/**
 * Implements heuristics that assume both traversal to edge and corner
 * neighbours is admissible, with costs for both taken from the passed
 * traversal_traits.
 **/
template <
  typename node_groupT,
  typename traversal_traitsT
>
unit_t
diagonal(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits);


/**
 * Implements the manhattan heuristics above, but with an additional tiebreaker
 * which prefers nodes that are closer to the line-of-sight path from the
 * current node to the end node.
 * The same restrictions apply as for the manhattan heuristics above.
 **/
template <
  typename node_groupT,
  typename traversal_traitsT
>
unit_t
manhattan_tiebreaker(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits);


/**
 * Implements the diagonal heuristics above, but with an additional tiebreaker
 * which prefers nodes that are closer to the line-of-sight path from the
 * current node to the end node.
 **/
template <
  typename node_groupT,
  typename traversal_traitsT
>
unit_t
diagonal_tiebreaker(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits);

}}} // namespace cartograph::pathfinding::heuristics

#include <cartograph/detail/heuristics.tcc>

#endif // guard
