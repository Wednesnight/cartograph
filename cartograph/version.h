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

#ifndef CG_VERSION_H
#define CG_VERSION_H

#ifndef __cplusplus
#error You are trying to include a C++ only header file
#endif

#include <stdint.h>

#include <utility>

namespace cartograph {

/**
 * XXX Note: consider the following definitions to be frozen. Users of this
 *           library can always rely, especially, on the version() function's
 *           prototype, and perform compatibility checks at runtime.
 **/

/**
 * Return the library version as a pair of two integer values. The return
 * value's "first" member contains the major version number, the "second"
 * member the minor version number.
 **/
std::pair<uint16_t, uint16_t> version();


/**
 * Return the library version as a string, with appropriate copyright notice.
 **/
extern char const * const copyright_string;

} // namespace cartograph

#endif // guard
