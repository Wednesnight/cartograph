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

#ifndef CG_TRAVERSAL_TRAITS_H
#define CG_TRAVERSAL_TRAITS_H

#include <cartograph/directions.h>

namespace cartograph {
namespace pathfinding {

/**
 * The simple_traversal_traits structure is probably not of a lot of use to
 * real-world applications, but demonstrates the interface quite well. It
 * assumes that
 *  - Both edge and corner movements are permissible
 *  - No nodes are impassable
 *  - The traversal cost from one node to the other, and the average traversal
 *    cost are both taken unmodified from the default_costs below.
 **/
template <
  typename node_groupT
>
struct simple_traversal_traits
{
  /**
   * Return true if the pathing algorithm should consider only edge neighbours
   * to be valid nodes for traversal, false if corner neighbours are also
   * permitted.
   * This implementation allows all types of neighbour nodes.
   **/
  join_t join_types();

  /**
   * Return true if traversal from the given node in the given direction should
   * be considered impossible by the pathing algorithm.
   * This implementation does not consider any node to be impassable.
   **/
  bool is_impassable(vector_t const & coords, directions_t const & d);

  /**
   * Returns the cost of traversing from the node with the specified coordinates
   * into the given direction. You should both terrain costs and, if applicable,
   * different costs for movement to edge or corner neighbours in the return
   * value.
   * This implementation uses the traversal costs from the default_costs
   * structure below.
   **/
  unit_t traversal_cost(vector_t const & coords, directions_t const & d);

  /**
   * Returns the average traversal cost from one node to another, used for the
   * pathing algorithm's heuristics. In your implementation, you may wish to
   * consider e.g. the statistical likelihood of encountering a certain type of
   * terrain in the node_group passed to the constructor, etc.
   * This implementation uses the average traversal costs from the default_costs
   * structure below.
   **/
  unit_t average_traversal_cost();
};


/**
 * This structure defines default costs for moving from one node to another.
 * It's specialized for each of the tile traits in tile_traits.h.
 *
 * The returned traversal cost depends on the type of tile it's specialized on.
 * In general, though, the returned costs are (rounded) distances from one
 * tile's center to the other, normalized on the distance between edge
 * neighbours' centers.
 *
 * For square tiles, where the distance between edge neighbours' centers is a
 * nominal 1 unit, the distance to a diagonal tile's center would be
 * sqrt((1 * 1) + (1 * 1)) ~ 1.4. This implementation returns 10 and 14
 * respectively, to stick to whole numbers only.
 *
 * The average traversal cost is simply the sum of all traversal costs to
 * neighbour nodes, divided by the number of nodes. In the case of square tiles,
 * we'd arrive at an average cost of 12.
 *
 * Note: While the traits in tile_traits.h technically could be any shape, as
 *       long as they retain the characteristics of fitting seamlessly next to
 *       each other, this default_costs struct is a little more strict and
 *       demands tiles to have uniform length sides.
 *
 *       For example, for non-square rectangular tiles, traversal to a
 *       horizontal neighbour might be significantly different from traversal
 *       to a vertical neighbour. This struct ignores those differences.
 **/
template <
  typename tile_traitsT
>
struct default_costs
{
  static unit_t average_traversal_cost();
  static unit_t traversal_cost(vector_t const & coords, directions_t const & d);
};

}} // namespace cartograph::pathfinding

#include <cartograph/detail/traversal_traits.tcc>

#endif // guard
