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

#include <boost/concept_check.hpp>

namespace cartograph {
namespace pathfinding {
namespace concepts {

/*****************************************************************************
 * HeuristicConcept
 */
template <
  typename node_groupT,
  typename traversal_traitsT,
  typename heuristicT
>
struct HeuristicConcept
{
  void constraints()
  {
    typedef boost::function<
      unit_t (node_groupT const &, vector_t const &, vector_t const &,
          vector_t const &, traversal_traitsT &)
    > heuristic_t;

    heuristic_t h = heuristic;
    boost::ignore_unused_variable_warning(h);
  }

  heuristicT const & heuristic;
};


/*****************************************************************************
 * TraversalTraitsConcept
 */
template <
    typename traversal_traitsT
>
struct TraversalTraitsConcept
{
  void constraints()
  {
    join_t t = instance.join_types();
    boost::ignore_unused_variable_warning(t);

    bool b = instance.is_impassable(coords, dir);
    boost::ignore_unused_variable_warning(b);

    unit_t u = instance.traversal_cost(coords, dir);

    u = instance.average_traversal_cost();

    const_constraints(instance);
  }


  void const_constraints(traversal_traitsT const & c_inst)
  {
  }

  traversal_traitsT &   instance;
  vector_t const &      coords;
  directions_t const &  dir;
};

}}} // namespace cartograph::pathfinding::concepts
