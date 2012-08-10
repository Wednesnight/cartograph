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

#include <cartograph/directions.h>
#undef CG_DIRECTIONS_H
#define CG_DIRECTIONS_IMPL
#include <cartograph/directions.h>

namespace cartograph {

std::ostream &
operator<<(std::ostream & os, directions_t const & dir)
{
  switch (dir) {
    case DIR_START:
      os << "DIR_START";
      break;

    case DIR_END:
      os << "DIR_END";
      break;

    default:
      os << direction_names[dir].dir_name;
      break;
  }
  return os;
}


} // namespace cartograph
