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

#ifndef CG_ERROR_H
#define CG_ERROR_H

#include <ostream>
#include <stdexcept>

#include <cartograph/types.h>

#undef CG_ERROR_START
#undef CG_ERROR
#undef CG_ERROR_END

#ifdef CG_ERROR_IMPL

#define CG_ERROR_START              \
    static const struct {           \
      error_t       err_code;       \
      char const *  err_name;       \
      char const *  err_desc;       \
    } error_descriptions[] = {


#define CG_ERROR(name, value, desc) \
      {                             \
        name,                       \
        CG_STRINGIFY(name),         \
        desc                        \
      },

#define CG_ERROR_END                \
      {                             \
        INVALID_ERROR,              \
        0,                          \
        0                           \
      }                             \
    };

#else

#define CG_ERROR_START              \
    enum error_t                    \
    {

#define CG_ERROR(name, value, desc) \
      name = value,                 \

#define CG_ERROR_END                \
      INVALID_ERROR = -1            \
    };

#endif



namespace cartograph {

/*****************************************************************************
 * Error codes
 */

CG_ERROR_START

CG_ERROR(CG_OK,
    0,
    "No error")

CG_ERROR(CG_INVALID_COORDS,
    50,
    "Invalid coordinates provided")
CG_ERROR(CG_INVALID_DIR,
    51,
    "Invalid direction provided")

CG_ERROR_END


#ifndef CG_ERROR_IMPL
std::ostream & operator<<(std::ostream & os, error_t const & err);


/*****************************************************************************
 * Exceptions
 */

/**
 * Cartograph rarely uses exceptions to report errors, and only in cases where
 * you can avoid the error from occurring, but if it does throw, it throws an
 * instance of this class.
 **/
class exception
  : public std::runtime_error
{
public:
  explicit exception(error_t const & err);
  ~exception() throw();

  virtual char const * what() const throw();
  operator error_t () const throw();

  bool operator==(error_t const & err) const throw();
private:
  error_t m_err;
};

std::ostream & operator<<(std::ostream & os, exception const & ex);

#endif



} // namespace cartograph

#endif // guard
