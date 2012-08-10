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
namespace concepts {

/*****************************************************************************
 * TileTraitsConcept
 */
template <
    typename tile_traitsT
>
struct TileTraitsConcept
{
  void constraints()
  {
    // Return an array of directions_t, terminated in DIR_END (not checked for
    // here), for the given coordinates. If flag is true, return edge-only
    // neighbours, otherwise include corner neighbours.
    directions_t const * const d = tile_traitsT::available_dirs(coords, flag);
    boost::ignore_unused_variable_warning(d);

    // Return a node relative to the given node at the given direction.
    vector_t v = tile_traitsT::get_relative(coords, dir);
    boost::ignore_unused_variable_warning(v);

    // Return true if the given coordinates are valid for the tile shape,
    // false otherwise.
    bool b = tile_traitsT::is_valid(coords);
    boost::ignore_unused_variable_warning(b);
  }

  vector_t const &      coords;
  directions_t const &  dir;
  join_t const &        flag;
};


/*****************************************************************************
 * IDGeneratorConcept
 */
template <
    typename id_generatorT
>
struct IDGeneratorConcept
{
  void constraints()
  {
    // We're not checking for whether it's default-constructible, because
    // we're offering both to copy-construct or to default-construct.

    typename id_generatorT::node_id_t node_id;

    node_id = generator.get_unique_id();

    generator.reset();
  }

  id_generatorT & generator;
};


}} // namespace cartograph::concepts
