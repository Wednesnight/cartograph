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


namespace cartograph {
namespace pathfinding {

/*****************************************************************************
 * simple_traversal_traits
 */

template <
  typename node_groupT
>
join_t
simple_traversal_traits<node_groupT>::join_types()
{
  return PATHFINDING_DEFAULT;
}




template <
  typename node_groupT
>
bool
simple_traversal_traits<node_groupT>::is_impassable(vector_t const & coords,
    directions_t const & d)
{
  return false;
}




template <
  typename node_groupT
>
unit_t
simple_traversal_traits<node_groupT>::traversal_cost(vector_t const & coords,
    directions_t const & d)
{
  return default_costs<
    typename node_groupT::tile_traits_t
  >::traversal_cost(coords, d);
}




template <
  typename node_groupT
>
unit_t
simple_traversal_traits<node_groupT>::average_traversal_cost()
{
  return default_costs<
    typename node_groupT::tile_traits_t
  >::average_traversal_cost();
}



}} // namespace cartograph::pathfinding
