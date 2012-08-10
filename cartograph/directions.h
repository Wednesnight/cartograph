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

#ifndef CG_DIRECTIONS_H
#define CG_DIRECTIONS_H


#include <ostream>

#include <cartograph/types.h>

#undef CG_DIRECTION_START
#undef CG_DIRECTION
#undef CG_DIRECTION_END

#ifdef CG_DIRECTIONS_IMPL

#define CG_DIRECTION_START          \
    static const struct {           \
      char const *  dir_name;       \
    } direction_names[] = {


#define CG_DIRECTION(name)          \
      { CG_STRINGIFY(name) },       \

#define CG_DIRECTION_END            \
    };

#else

#define CG_DIRECTION_START          \
    enum directions_t               \
    {                               \
      DIR_START = -1,

// XXX ensure that first starts at zero
#define CG_DIRECTION(name)          \
      name,

#define CG_DIRECTION_END            \
      DIR_END                       \
    };

#endif

namespace cartograph {

/**
 * Define available directions. Each shape tile may only define a subset of
 * these directions; see doc/design.txt for details.
 **/
CG_DIRECTION_START
CG_DIRECTION(NORTH)
CG_DIRECTION(NORTH_NORTH_EAST)
CG_DIRECTION(NORTH_EAST)
CG_DIRECTION(EAST_NORTH_EAST)
CG_DIRECTION(EAST)
CG_DIRECTION(EAST_SOUTH_EAST)
CG_DIRECTION(SOUTH_EAST)
CG_DIRECTION(SOUTH_SOUTH_EAST)
CG_DIRECTION(SOUTH)
CG_DIRECTION(SOUTH_SOUTH_WEST)
CG_DIRECTION(SOUTH_WEST)
CG_DIRECTION(WEST_SOUTH_WEST)
CG_DIRECTION(WEST)
CG_DIRECTION(WEST_NORTH_WEST)
CG_DIRECTION(NORTH_WEST)
CG_DIRECTION(NORTH_NORTH_WEST)
CG_DIRECTION_END

#ifndef CG_DIRECTIONS_IMPL
std::ostream & operator<<(std::ostream & os, directions_t const & dir);
#endif


} // namespace cartograph

#endif // guard
