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

#include <cartograph/tile_traits.h>

namespace cartograph {

/*****************************************************************************
 * struct rectangular_tile_traits
 */

directions_t const * const
rectangular_tile_traits::available_dirs(vector_t const & coords, join_t join_type)
{
  static const directions_t edges[] = {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    DIR_END,
  };
  static const directions_t corners[] = {
    NORTH_EAST,
    SOUTH_EAST,
    SOUTH_WEST,
    NORTH_WEST,
    DIR_END,
  };
  static const directions_t all[] = {
    NORTH,
    NORTH_EAST,
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    NORTH_WEST,
    DIR_END,
  };

  if (join_type & EDGES) {
    if (join_type & CORNERS_FACING) {
      return all;
    }
    return edges;

  }

  if (join_type & CORNERS_FACING) {
    return corners;
  }

  return NULL;
}


vector_t
rectangular_tile_traits::get_relative(vector_t const & coords,
    directions_t const & dir)
{
  switch (dir) {
    case NORTH:
      return coords + vector_t(0, -1);

    case NORTH_EAST:
      return coords + vector_t(1, -1);

    case EAST:
      return coords + vector_t(1, 0);

    case SOUTH_EAST:
      return coords + vector_t(1, 1);

    case SOUTH:
      return coords + vector_t(0, 1);

    case SOUTH_WEST:
      return coords + vector_t(-1, 1);

    case WEST:
      return coords + vector_t(-1, 0);

    case NORTH_WEST:
      return coords + vector_t(-1, -1);

    default:
      return invalid_vector;
  }
}



bool
rectangular_tile_traits::is_valid(vector_t const & coords)
{
  // Any type of coordinate is valid, at least in theory - except for
  // specifically invalid ones, of course.
  return (coords != invalid_vector);
}




/*****************************************************************************
 * struct triangular_tile_traits
 */
namespace detail {

directions_t const * const
available_dirs_triangular_up(join_t join_type)
{
  static const directions_t edges[] = {
    EAST,
    SOUTH,
    WEST,
    DIR_END,
  };

  static const directions_t facing[] = {
    NORTH,
    SOUTH_EAST,
    SOUTH_WEST,
    DIR_END,
  };

  static const directions_t oblique[] = {
    NORTH_EAST,
    EAST_SOUTH_EAST,
    SOUTH_SOUTH_EAST,
    SOUTH_SOUTH_WEST,
    WEST_SOUTH_WEST,
    NORTH_WEST,
    DIR_END,
  };

  static const directions_t edges_facing[] = {
    NORTH,
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    DIR_END,
  };

  static const directions_t edges_oblique[] = {
    NORTH_EAST,
    EAST,
    EAST_SOUTH_EAST,
    SOUTH_SOUTH_EAST,
    SOUTH,
    SOUTH_SOUTH_WEST,
    WEST_SOUTH_WEST,
    WEST,
    NORTH_WEST,
    DIR_END,
  };

  static const directions_t facing_oblique[] = {
    NORTH,
    NORTH_EAST,
    EAST_SOUTH_EAST,
    SOUTH_EAST,
    SOUTH_SOUTH_EAST,
    SOUTH_SOUTH_WEST,
    SOUTH_WEST,
    WEST_SOUTH_WEST,
    NORTH_WEST,
    DIR_END,
  };

  static const directions_t all[] = {
    NORTH,
    NORTH_EAST,
    EAST,
    EAST_SOUTH_EAST,
    SOUTH_EAST,
    SOUTH_SOUTH_EAST,
    SOUTH,
    SOUTH_SOUTH_WEST,
    SOUTH_WEST,
    WEST_SOUTH_WEST,
    WEST,
    NORTH_WEST,
    DIR_END,
  };

  if (join_type & EDGES) {

    if (join_type & CORNERS_FACING) {

      if (join_type & CORNERS_OBLIQUE) {
        return all;
      }
      return edges_facing;

    } else if (join_type & CORNERS_OBLIQUE) {
      return edges_oblique;
    }

    return edges;
  }

  if (join_type & CORNERS_FACING) {
    if (join_type & CORNERS_OBLIQUE) {
      return facing_oblique;
    }

    return facing;
  }

  if (join_type & CORNERS_OBLIQUE) {
    return oblique;
  }

  return NULL;
}


directions_t const * const
available_dirs_triangular_down(join_t join_type)
{
  static const directions_t edges[] = {
    NORTH,
    EAST,
    WEST,
    DIR_END,
  };

  static const directions_t facing[] = {
    NORTH_EAST,
    SOUTH,
    NORTH_WEST,
    DIR_END,
  };

  static const directions_t oblique[] = {
    NORTH_NORTH_EAST,
    EAST_NORTH_EAST,
    SOUTH_EAST,
    SOUTH_WEST,
    WEST_NORTH_WEST,
    NORTH_NORTH_WEST,
    DIR_END,
  };

  static const directions_t edges_facing[] = {
    NORTH,
    NORTH_EAST,
    EAST,
    SOUTH,
    WEST,
    NORTH_WEST,
    DIR_END,
  };

  static const directions_t edges_oblique[] = {
    NORTH,
    NORTH_NORTH_EAST,
    EAST_NORTH_EAST,
    EAST,
    SOUTH_EAST,
    SOUTH_WEST,
    WEST,
    WEST_NORTH_WEST,
    NORTH_NORTH_WEST,
    DIR_END,
  };

  static const directions_t facing_oblique[] = {
    NORTH_NORTH_EAST,
    NORTH_EAST,
    EAST_NORTH_EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST_NORTH_WEST,
    NORTH_WEST,
    NORTH_NORTH_WEST,
    DIR_END,
  };

  static const directions_t all[] = {
    NORTH,
    NORTH_NORTH_EAST,
    NORTH_EAST,
    EAST_NORTH_EAST,
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    WEST_NORTH_WEST,
    NORTH_WEST,
    NORTH_NORTH_WEST,
    DIR_END,
  };

  if (join_type & EDGES) {

    if (join_type & CORNERS_FACING) {

      if (join_type & CORNERS_OBLIQUE) {
        return all;
      }
      return edges_facing;

    } else if (join_type & CORNERS_OBLIQUE) {
      return edges_oblique;
    }

    return edges;
  }

  if (join_type & CORNERS_FACING) {
    if (join_type & CORNERS_OBLIQUE) {
      return facing_oblique;
    }

    return facing;
  }

  if (join_type & CORNERS_OBLIQUE) {
    return oblique;
  }

  return NULL;
}


} // namespace detail

directions_t const * const
triangular_tile_traits::available_dirs(vector_t const & coords, join_t join_type)
{
  // Due to the requirement that (0, 0) having it's bottom edge aligned with
  // the screen top/bottom, we have the situation that tiles whose (x + y) is
  // even are aligned in the same way as (0, 0), and all others are
  // upside-down.
  unit_t sum = coords.m_x + coords.m_y;

  if (sum % 2) {
    // odd sum
    return detail::available_dirs_triangular_down(join_type);
  }
  // even sum
  return detail::available_dirs_triangular_up(join_type);
}


vector_t
triangular_tile_traits::get_relative(vector_t const & coords, directions_t const & dir)
{
  if (dir == DIR_START || dir >= DIR_END) {
    return invalid_vector;
  }

  static const vector_t relative_up[] = {
    vector_t(0, -1),      // NORTH
    invalid_vector,       // NORTH_NORTH_EAST
    vector_t(1, -1),      // NORTH_EAST
    invalid_vector,       // EAST_NORTH_EAST
    vector_t(1, 0),       // EAST
    vector_t(2, 0),       // EAST_SOUTH_EAST
    vector_t(2, 1),       // SOUTH_EAST
    vector_t(1, 1),       // SOUTH_SOUTH_EAST
    vector_t(0, 1),       // SOUTH
    vector_t(-1, 1),      // SOUTH_SOUTH_WEST
    vector_t(-2, 1),      // SOUTH_WEST
    vector_t(-2, 0),      // WEST_SOUTH_WEST
    vector_t(-1, 0),      // WEST
    invalid_vector,       // WEST_NORTH_WEST
    vector_t(-1, -1),     // NORTH_WEST
    invalid_vector,       // NORTH_NORTH_WEST
  };
  static const vector_t relative_down[] = {
    vector_t(0, -1),      // NORTH
    vector_t(1, -1),      // NORTH_NORTH_EAST
    vector_t(2, -1),      // NORTH_EAST
    vector_t(2, 0),       // EAST_NORTH_EAST
    vector_t(1, 0),       // EAST
    invalid_vector,       // EAST_SOUTH_EAST
    vector_t(1, 1),       // SOUTH_EAST
    invalid_vector,       // SOUTH_SOUTH_EAST
    vector_t(0, 1),       // SOUTH
    invalid_vector,       // SOUTH_SOUTH_WEST
    vector_t(-1, 1),      // SOUTH_WEST
    invalid_vector,       // WEST_SOUTH_WEST
    vector_t(-1, 0),      // WEST
    vector_t(-2, 0),      // WEST_NORTH_WEST
    vector_t(-2, -1),     // NORTH_WEST
    vector_t(-1, -1),     // NORTH_NORTH_WEST
  };

  unit_t sum = coords.m_x + coords.m_y;
  if (sum % 2) {
    // odd sum, i.e. pointy-side down triangle.
    if (dir > unit_t(sizeof(relative_down) / sizeof(vector_t))) {
      return invalid_vector;
    }
    return coords + relative_down[dir];
  }

  // even sum, i.e. pointy-side up triangle.
  if (dir > unit_t(sizeof(relative_up) / sizeof(vector_t))) {
    return invalid_vector;
  }
  return coords + relative_up[dir];
}


bool
triangular_tile_traits::is_valid(vector_t const & coords)
{
  // Any type of coordinate is valid, at least in theory - except for
  // specifically invalid ones, of course.
  return (coords != invalid_vector);
}



/*****************************************************************************
 * struct hexagonal_tile_traits
 */
directions_t const * const
hexagonal_tile_traits::available_dirs(vector_t const & coords, join_t join_type)
{
  static const directions_t dirs[] = {
    NORTH,
    NORTH_EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    NORTH_WEST,
    DIR_END,
  };

  if (join_type & EDGES) {
    return dirs;
  }
  return NULL;
}


vector_t
hexagonal_tile_traits::get_relative(vector_t const & coords,
    directions_t const & dir)
{
  switch (dir) {
    case NORTH:
      return coords + vector_t(0, -2);

    case NORTH_EAST:
      return coords + vector_t(1, -1);

    case SOUTH_EAST:
      return coords + vector_t(1, 1);

    case SOUTH:
      return coords + vector_t(0, 2);

    case SOUTH_WEST:
      return coords + vector_t(-1, 1);

    case NORTH_WEST:
      return coords + vector_t(-1, -1);

    default:
      return invalid_vector;
  }
}


bool
hexagonal_tile_traits::is_valid(vector_t const & coords)
{
  if (coords == invalid_vector) {
    return false;
  }

  // The particular constraints laid out in design.txt imply that only coords
  // are valid where both row and column are either even or odd - no mixed
  // coords allowed.
  unit_t sum = coords.m_x + coords.m_y;
  if (sum % 2) {
    // one of the coordinates was even, the other odd, or there would be no
    // remainder: 1 + 1 is equally even as 2 + 2, but 1 + 2 isn't.
    return false;
  }
  return true;
}




} // namespace cartograph
