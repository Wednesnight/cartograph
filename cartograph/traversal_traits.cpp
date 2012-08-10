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

#include <cartograph/traversal_traits.h>
#include <cartograph/tile_traits.h>

namespace cartograph {
namespace pathfinding {

/*****************************************************************************
 * struct default_costs<rectangular_tile_traits>
 */

template <>
unit_t
default_costs<rectangular_tile_traits>::traversal_cost(vector_t const & coords,
    directions_t const & dir)
{
  // For a nonimal cost of 1 for edge neighbours, a traversal to a diagonal
  // node would be sqrt(1 + 1) ~ 1.4 - we'll make that 10 and 14.
  switch (dir) {
    case NORTH:
    case EAST:
    case SOUTH:
    case WEST:
      return 10;

    case NORTH_EAST:
    case SOUTH_EAST:
    case SOUTH_WEST:
    case NORTH_WEST:
      return 14;

    default:
      return invalid_unit;
  }
}


template <>
unit_t
default_costs<rectangular_tile_traits>::average_traversal_cost()
{
  // ((4 * 10) + (4 * 14)) / 8
  return 12;
}





/*****************************************************************************
 * struct default_costs<triangular_tile_traits>
 */

template <>
unit_t
default_costs<triangular_tile_traits>::traversal_cost(vector_t const & coords,
    directions_t const & dir)
{
  if (dir == DIR_START || dir >= DIR_END) {
    return invalid_unit;
  }

  // Triangles... always more difficult than the rest. With triangles we have
  // three different costs:
  // - edge neighbours
  // - corner neighbours that lie directly opposite an edge neighbour
  // - all other corner neighbours
  //
  // Do the math yourself, but the costs are 2, 4, and ~ 3.5 respectively, so
  // we'll make that 200, 400 and 350. The extra zero at the end is to honour
  // the average better, which would be 325 (see below).
  //
  // But of course it depends on whether we have an upside down triangle or not
  // what directions can be considered to be edge, corner or other neighbours...
  static const unit_t cost_up[] = {
    400,          // NORTH
    invalid_unit, // NORTH_NORTH_EAST
    350,          // NORTH_EAST
    invalid_unit, // EAST_NORTH_EAST
    200,          // EAST
    350,          // EAST_SOUTH_EAST
    400,          // SOUTH_EAST
    350,          // SOUTH_SOUTH_EAST
    200,          // SOUTH
    350,          // SOUTH_SOUTH_WEST
    400,          // SOUTH_WEST
    350,          // WEST_SOUTH_WEST
    200,          // WEST
    invalid_unit, // WEST_NORTH_WEST
    350,          // NORTH_WEST
    invalid_unit, // NORTH_NORTH_WEST
  };
  static const unit_t cost_down[] = {
    200,          // NORTH
    350,          // NORTH_NORTH_EAST
    400,          // NORTH_EAST
    350,          // EAST_NORTH_EAST
    200,          // EAST
    invalid_unit, // EAST_SOUTH_EAST
    350,          // SOUTH_EAST
    invalid_unit, // SOUTH_SOUTH_EAST
    400,          // SOUTH
    invalid_unit, // SOUTH_SOUTH_WEST
    350,          // SOUTH_WEST
    invalid_unit, // WEST_SOUTH_WEST
    200,          // WEST
    350,          // WEST_NORTH_WEST
    400,          // NORTH_WEST
    350,          // NORTH_NORTH_WEST
  };

  unit_t sum = coords.m_x + coords.m_y;
  if (sum % 2) {
    // pointy-side down triangle
    return cost_down[dir];
  }

  // even sum, i.e. pointy-side up triangle.
  return cost_up[dir];
}



template <>
unit_t
default_costs<triangular_tile_traits>::average_traversal_cost()
{
  // ((3 * 200) + (3 * 400) + (6 * 350)) / 12
  return 325;
}



/*****************************************************************************
 * struct default_costs<hexagonal_tile_traits>
 */

template <>
unit_t
default_costs<hexagonal_tile_traits>::traversal_cost(vector_t const & coords,
    directions_t const & dir)
{
  // Hexagonal tiles only touch on edges - the cost is always the same.
  return 1;
}



template <>
unit_t
default_costs<hexagonal_tile_traits>::average_traversal_cost()
{
  // (6 * 1) / 6
  return 1;
}



}} // namespace cartograph::pathfinding
