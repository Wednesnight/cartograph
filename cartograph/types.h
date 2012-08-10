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

#ifndef CG_TYPES_H
#define CG_TYPES_H

/**
 * Stringify a symbol name.
 **/
#define CG_STRINGIFY(n) CG_STRINGIFY_HELPER(n)
#define CG_STRINGIFY_HELPER(n) #n

#include <stdint.h>

#include <ostream>

#include <cartograph/cg-config.h>

namespace cartograph {

/**
 * The basic unit type.
 **/
#if defined(HAVE_INT64_T)
typedef int64_t unit_t;
#else
#if defined(HAVE_INT32_t)
typedef int32_t unit_t;
#else
typedef int16_t unit_t;
#endif
#endif

extern const unit_t invalid_unit;


/**
 * Enumeration for the manner in which tiles can be joined with each other.
 * (compare directions.h, tile_traits.h, design.txt).
 **/
enum join_t
{
  // Edge-to-edge joined tiles
  EDGES               = 0x0001,

  // Tiles joined by a corner, where one tile is directly opposite the other
  CORNERS_FACING      = 0x0002,
  // Tiles joined by a corner, where tiles are not direclty opposite each other
  CORNERS_OBLIQUE     = 0x0004,
  // Both corner types
  CORNERS             = CORNERS_FACING | CORNERS_OBLIQUE,

  // The default type to use in pathfinding; oblique corners only exist for
  // triangular tiles, and would present shortcuts for edge-to-edge movement.
  PATHFINDING_DEFAULT = EDGES | CORNERS_FACING,

  // All types
  ALL_JOIN_TYPES      = EDGES | CORNERS
};



/**
 * A 2D vector type - used for coordinate references in node_groups, etc.
 *
 * A default-constructed vector_t is equivalent to invalid_vector (see below).
 **/
struct vector_t
{
  vector_t(unit_t x = invalid_unit, unit_t y = invalid_unit);

  /**
   * Useless for anything but uniqueness-constraints in sets. Returns true
   * if either m_x is smaller, or m_y.
   **/
  bool operator<(vector_t const & other) const;

  bool operator!=(vector_t const & other) const;
  bool operator==(vector_t const & other) const;

  vector_t & operator+=(vector_t const & other);
  vector_t & operator-=(vector_t const & other);

  vector_t operator+(vector_t const & other) const;
  vector_t operator-(vector_t const & other) const;

  /**
   * Compute the distance to the other vector.
   **/
  double distance(vector_t const & other = vector_t(0, 0)) const;

  unit_t m_x;
  unit_t m_y;
};

extern const vector_t invalid_vector;

std::ostream & operator<<(std::ostream & os, vector_t const & vec);


} // namespace cartograph

#endif // guard
