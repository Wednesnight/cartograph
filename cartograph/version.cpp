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

#include <cartograph/cg-config.h>
#include <cartograph/version.h>
#include <cartograph/types.h>

namespace cartograph {


std::pair<uint16_t, uint16_t>
version()
{
    return std::make_pair<uint16_t, uint16_t>(CARTOGRAPH_MAJOR, CARTOGRAPH_MINOR);
}



char const * const copyright_string = "cartograph "
    CG_STRINGIFY(CARTOGRAPH_MAJOR) "." CG_STRINGIFY(CARTOGRAPH_MINOR)
    " - Copyright (c) 2008 by Jens Finkhaeuser.\n"
    "This program comes with ABSOLUTELY NO WARRANTY. It is also free software,\n"
    "and you are welcome to redistribute it under certain conditions. Details\n"
    "on both are available in the license text downloadable at\n"
    "<http://www.gnu.org/licenses/gpl-3.0.txt>.\n";


} // namespace cartograph
