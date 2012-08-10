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

#include <cartograph/error.h>
#undef CG_ERROR_H
#define CG_ERROR_IMPL
#include <cartograph/error.h>

namespace cartograph {

/*****************************************************************************
 * Error codes
 */

std::ostream &
operator<<(std::ostream & os, error_t const & err)
{
  size_t max_error = sizeof(error_descriptions) / sizeof(error_descriptions[0]);
  for (size_t i = 0 ; i < max_error ; ++i) {
    if (error_descriptions[i].err_code == err) {
      os << error_descriptions[i].err_name;
      return os;
    }
  }

  os << "unidentified error";
  return os;
}


/*****************************************************************************
 * Exceptions
 */


exception::exception(error_t const & err)
  : std::runtime_error("") // don't need to store a string in the exception
  , m_err(err)
{
}



exception::~exception() throw()
{
}



char const *
exception::what() const throw()
{
  size_t max_error = sizeof(error_descriptions) / sizeof(error_descriptions[0]);
  for (size_t i = 0 ; i < max_error ; ++i) {
    if (error_descriptions[i].err_code == m_err) {
      return error_descriptions[i].err_desc;
    }
  }

  return "unidentified error";
}



exception::operator error_t () const throw()
{
  return m_err;
}



bool
exception::operator==(error_t const & err) const throw()
{
  return (m_err == err);
}



std::ostream &
operator<<(std::ostream & os, exception const & ex)
{
  os << "<" << static_cast<error_t>(ex) << ":" << ex.what() << ">";
  return os;
}




} // namespace cartograph


