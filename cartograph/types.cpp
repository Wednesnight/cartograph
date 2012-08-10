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

#include <cartograph/types.h>

#include <cmath>

#include <boost/integer_traits.hpp>

namespace cartograph {

/**
 * Invalid unit_t/vector_t values.
 **/
const unit_t invalid_unit = boost::integer_traits<unit_t>::const_min;
const vector_t invalid_vector = vector_t();



/*****************************************************************************
 * struct vector_t
 */
vector_t::vector_t(unit_t x, unit_t y)
  : m_x(x)
  , m_y(y)
{
}



double
vector_t::distance(vector_t const & other /* = vector_t(0, 0) */) const
{
  vector_t diff = *this;
  diff -= other;
  return std::sqrt((diff.m_x * diff.m_x) + (diff.m_y * diff.m_y));
}



bool
vector_t::operator<(vector_t const & other) const
{
  if (m_x < other.m_x) {
    return true;
  }
  return ((m_x == other.m_x) && (m_y < other.m_y));
}


bool
vector_t::operator==(vector_t const & other) const
{
  return ((m_x == other.m_x) && (m_y == other.m_y));
}



bool
vector_t::operator!=(vector_t const & other) const
{
  return !(*this == other);
}



vector_t &
vector_t::operator+=(vector_t const & other)
{
  m_x += other.m_x;
  m_y += other.m_y;
  return *this;
}


vector_t &
vector_t::operator-=(vector_t const & other)
{
  m_x -= other.m_x;
  m_y -= other.m_y;
  return *this;
}



vector_t
vector_t::operator+(vector_t const & other) const
{
  vector_t tmp = *this;
  tmp += other;
  return tmp;
}



vector_t
vector_t::operator-(vector_t const & other) const
{
  vector_t tmp = *this;
  tmp -= other;
  return tmp;
}



std::ostream &
operator<<(std::ostream & os, vector_t const & vec)
{
  os << "(" << vec.m_x << ", " << vec.m_y << ")";
  return os;
}


} // namespace cartograph


