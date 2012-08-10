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

#ifndef CG_PATHFINDING_H
#define CG_PATHFINDING_H

#include <deque>

#include <boost/function.hpp>

#ifndef CG_DISABLE_CONCEPT_CHECKS
// Include concepts
#include <cartograph/detail/pathfinding_concepts.h>
#endif

namespace cartograph {
namespace pathfinding {

/**
 * Implements A* pathfinding. Given a node_group, a start node (coordinates)
 * and end node (coordinates), the function returns a deque of coordinates of
 * nodes that need to be traversed to reach the end node.
 * See heuristics.h for details on some heuristics functions you can use with
 * this algorithm.
 * See traversal_traits.h for details on an example traversal traits
 * implementation.
 *
 * @param result List of nodes to be traversed to go from start to end, given
 *    the restrictions in the node_group.
 * @param group Node group containing all the possible nodes for finding a path,
 *    including start & end node.
 * @param start Start position to find path to...
 * @param end ... here.
 * @param traversal_traits Traits type that delivers information about each node
 *    to be tested, such as it's terrain cost, etc.
 * @param heuristic Heuristics function to use for figuring out how good a node
 *    is in terms of getting closer to the goal. To turn A* into Dijkstra's
 *    algorithm, always use a heuristic value of 0 - the dijkstra function in
 *    heuristics.h does just that.
 **/
template <
  typename node_groupT,
  typename traversal_traitsT,
  typename heuristicT
>
error_t
a_star(std::deque<vector_t> & result, node_groupT const & group,
    vector_t const & start, vector_t const & end,
    traversal_traitsT & traversal_traits,
    heuristicT const & heuristic);

}} // namespace cartograph::pathfinding

#include <cartograph/detail/pathfinding.tcc>

#endif // guard
