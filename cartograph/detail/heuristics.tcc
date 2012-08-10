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

#include <boost/static_assert.hpp>
#include <cartograph/tile_traits.h>

namespace cartograph {
namespace pathfinding {
namespace heuristics {

/*****************************************************************************
 * "Dijkstra heuristics"
 */

template <
  typename node_groupT,
  typename traversal_traitsT
>
unit_t
dijkstra(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits)
{
  // Dijkstra's algorithm is essentially A* without a heuristic - so if we
  // always return zero for the heuristic function, we get Dijkstra's algorithm.
  return unit_t();
}



/*****************************************************************************
 * Manhattan heuristics
 */

namespace detail {

/**
 * Specialized in heuristics.cpp, but only for rectangular_tile_traits.
 * Manhattan-style heuristics don't work at all for anything but rectangular
 * tiles, and don't even work very well then.
 **/
template <
  typename tile_traitsT
>
struct manhattan_is_specialized;

template <>
struct manhattan_is_specialized<rectangular_tile_traits>
{
};

} // namespace detail



template <
  typename node_groupT,
  typename traversal_traitsT
>
unit_t
manhattan(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits)
{
  BOOST_STATIC_ASSERT(sizeof(detail::manhattan_is_specialized<
        typename node_groupT::tile_traits_t
      >) != 0);

  return traversal_traits.average_traversal_cost()
    * std::abs(end.m_x - current.m_x) + std::abs(end.m_y - current.m_y);
}


/*****************************************************************************
 * Diagonal heuristics
 */

namespace detail {
// Diagonal heuristics need to be specialized for each shape tile, as that
// strongly influences the possible directions in which one can move, and
// their relative weight.
template <
  typename tile_traitsT,
  typename node_groupT,
  typename traversal_traitsT
>
struct diagonal
{
};



template <
  typename node_groupT,
  typename traversal_traitsT
>
struct diagonal<
  rectangular_tile_traits,
  node_groupT,
  traversal_traitsT
>
{
  inline unit_t
  operator()(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits)
  {
    unit_t x_diff = std::abs(current.m_x - end.m_x);
    unit_t y_diff = std::abs(current.m_y - end.m_y);
    unit_t h_diagonal = std::min(x_diff, y_diff);
    unit_t h_straight = x_diff + y_diff;

    unit_t s_cost = traversal_traits.traversal_cost(vector_t(0, 0), NORTH);
    unit_t d_cost = traversal_traits.traversal_cost(vector_t(0, 0), NORTH_EAST);

    // Assume that we walk as much as possible diagonally at d_cost, then the
    // remainder at s_cost.
    return (d_cost * h_diagonal) + (s_cost * (h_straight - 2 * h_diagonal));
  }
};


template <
  typename node_groupT,
  typename traversal_traitsT
>
struct diagonal<
  hexagonal_tile_traits,
  node_groupT,
  traversal_traitsT
>
{
  inline unit_t
  operator()(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits)
  {
    // For once, hexagonal tiles actually make things a bit tricky, because
    // they do not allow horizontal movement (in our model alignment). On the
    // other hand, movement to any tile costs the same amount, as there are no
    // corner neighbours.
    //
    // The upshot is that if the x distance between current and end is less than
    // the y distance, we'll have to move down and diagonally. If it is more,
    // we have to approximate horizontal movement by going NW/SW, etc.
    // repeatedly. Fortunately this double movement is balanced by the fact that
    // vertically we skip every second line, so for determining how much we can
    // go diagonally, it doesn't matter...

    unit_t x_diff = std::abs(current.m_x - end.m_x);
    unit_t y_diff = std::abs(current.m_y - end.m_y);

    unit_t h_diagonal = std::min(x_diff, y_diff);

    // The remaining distance in lines/columns is simply the original distance
    // minus h_diagonal, as with every diagonal step we also move a step closer
    // to the end.
    unit_t h_straight = std::max(x_diff, y_diff) - h_diagonal;

    // h_straight now reflects the remaining distance in rows/columns, but now
    // vertical movement along N rows only costs N/2 steps, while horizontal
    // movement along N columns costs N steps... so we have to differentiate
    // between whether our remaining distance is horizontal or vertical.
    if (x_diff < y_diff) {
      // The remaining distance is vertical, so halve the costs.
      h_straight /= 2;
    }

    // Now the cost for each tile-to-tile movement is actually the same...
    return traversal_traits.average_traversal_cost() * (h_diagonal + h_straight);
  }
};



template <
  typename node_groupT,
  typename traversal_traitsT
>
struct diagonal<
  triangular_tile_traits,
  node_groupT,
  traversal_traitsT
>
{
  inline unit_t
  operator()(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits)
  {
    // For triangular tiles, we make the simplifying assumption that the
    // shortest path follows exclusively along edge neighbours. This is likely
    // to be accurate, given that allowing corner nodes in a path produces weird
    // looking paths, and probably won't be done much - and if it's not
    // accurate, it won't be off by miles.
    //
    // Given that assumption, if we walk diagonally as far as we can, the
    // resultant path costs would actually be the same as the manhattan
    // distance would be, if we allowed corner movement as well.
    return traversal_traits.average_traversal_cost()
        * std::abs(end.m_x - current.m_x) + std::abs(end.m_y - current.m_y);
  }
};



} // namespace detail


template <
  typename node_groupT,
  typename traversal_traitsT
>
unit_t
diagonal(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits)
{
  detail::diagonal<
    typename node_groupT::tile_traits_t,
    node_groupT,
    traversal_traitsT
  > d;
  return d(group, start, current, end, traversal_traits);
}




/*****************************************************************************
 * Tiebreaker heuristics
 */
namespace detail {

inline
unit_t
line_of_sight_crossproduct(vector_t const & start, vector_t const & current,
    vector_t const & end)
{
  unit_t dx1 = current.m_x - end.m_x;
  unit_t dy1 = current.m_y - end.m_y;
  unit_t dx2 = start.m_x - end.m_x;
  unit_t dy2 = start.m_y - end.m_y;
  return std::abs((dx1 * dy2) - (dx2 * dy1));
}


template <
  typename node_groupT,
  typename traversal_traitsT,
  typename heuristicT
>
inline
unit_t
generic_tiebreaker(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits, heuristicT const & _heuristic)
{
  typedef boost::function<
    unit_t (node_groupT const &, vector_t const &, vector_t const &,
        vector_t const &, traversal_traitsT &)
  > heuristic_t;

  heuristic_t heuristic = _heuristic;

  unit_t h = heuristic(group, start, current, end, traversal_traits);

  unit_t cross = line_of_sight_crossproduct(start, current, end);

  // The cross product should weigh in at around 1/1000 if a step costs 1 at
  // minimum, and the number of steps is estimated to not exceed 1000. We'll be
  // a bit more generic here (and more computing intensive) by instead picking
  // the average traversal cost as the step cost, and the same heuristic method
  // using at each step applied to the total path as the maximum.
  unit_t total = heuristic(group, start, start, end, traversal_traits);

  unit_t avg = traversal_traits.average_traversal_cost();

  h = (h * (total + avg)) / avg;
  h += cross;
  h = (h * avg) / (total + avg);

  return h;
}


} // namespace detail




template <
  typename node_groupT,
  typename traversal_traitsT
>
unit_t
manhattan_tiebreaker(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits)
{
  return detail::generic_tiebreaker(group, start, current, end, traversal_traits,
      &manhattan<node_groupT, traversal_traitsT>);
}



template <
  typename node_groupT,
  typename traversal_traitsT
>
unit_t
diagonal_tiebreaker(node_groupT const & group, vector_t const & start,
    vector_t const & current, vector_t const & end,
    traversal_traitsT & traversal_traits)
{
  return detail::generic_tiebreaker(group, start, current, end, traversal_traits,
      &diagonal<node_groupT, traversal_traitsT>);
}


}}} // namespace cartograph::pathfinding::heuristics
