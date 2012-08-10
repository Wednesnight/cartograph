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

#ifndef CG_TILE_TRAITS_H
#define CG_TILE_TRAITS_H

#include <cartograph/directions.h>
#include <cartograph/types.h>

namespace cartograph {

/**
 * Traits for rectangular tiles - or any tile with four corners that can fit
 * together in the same manner as rectangles, i.e. a rhombus, parallelogram or
 * isoceles trapezoid.
 **/
struct rectangular_tile_traits
{
  static directions_t const * const
  available_dirs(vector_t const & coords, join_t join_type);

  static vector_t
  get_relative(vector_t const & coords, directions_t const & dir);

  static bool
  is_valid(vector_t const & coords);
};



/**
 * Traits for triangular tiles.
 **/
struct triangular_tile_traits
{
  static directions_t const * const
  available_dirs(vector_t const & coords, join_t join_type);

  static vector_t
  get_relative(vector_t const & coords, directions_t const & dir);

  static bool
  is_valid(vector_t const & coords);
};



/**
 * Traits for hexagonal tiles.
 **/
struct hexagonal_tile_traits
{
  static directions_t const * const
  available_dirs(vector_t const & coords, join_t join_type);

  static vector_t
  get_relative(vector_t const & coords, directions_t const & dir);

  static bool
  is_valid(vector_t const & coords);
};


} // namespace cartograph

#endif // guard
