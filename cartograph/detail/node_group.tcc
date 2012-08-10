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

namespace cartograph {

/*****************************************************************************
 * Class node_group<>::node
 */

template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
node_group<node_dataT, tile_traitsT, id_generatorT>::node::node(
    node_group<node_dataT, tile_traitsT, id_generatorT> * group,
    node_id_t const & _id, vector_t const & coords)
  : m_group(group)
  , m_id(_id)
  , m_coords(coords)
{
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
node_group<node_dataT, tile_traitsT, id_generatorT>::node::node(
    node const & other)
  : m_group(other.m_group)
  , m_id(other.m_id)
  , m_coords(other.m_coords)
{
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
node_group<node_dataT, tile_traitsT, id_generatorT>::node::~node()
{
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
typename node_group<node_dataT, tile_traitsT, id_generatorT>::node_id_t
node_group<node_dataT, tile_traitsT, id_generatorT>::node::id() const
{
  return m_id;
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
vector_t
node_group<node_dataT, tile_traitsT, id_generatorT>::node::coordinates() const
{
  return m_coords;
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
node_dataT const *
node_group<node_dataT, tile_traitsT, id_generatorT>::node::operator->() const
{
  return get();
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
node_dataT *
node_group<node_dataT, tile_traitsT, id_generatorT>::node::operator->()
{
  return get();
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
node_dataT const *
node_group<node_dataT, tile_traitsT, id_generatorT>::node::get() const
{
  node_data_ptr p = m_group->get(m_id);
  return p.get();
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
node_dataT *
node_group<node_dataT, tile_traitsT, id_generatorT>::node::get()
{
  node_data_ptr p = m_group->get(m_id);
  return p.get();
}


template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
typename node_group<node_dataT, tile_traitsT, id_generatorT>::node &
node_group<node_dataT, tile_traitsT, id_generatorT>::node::operator=(
    node_dataT const & other)
{
  m_group->set(m_id, m_coords, other);
  return *this;
}


template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
node_group<node_dataT, tile_traitsT, id_generatorT>::node::operator bool() const
{
  node_data_ptr p = m_group->get(m_id);
  return (p);
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
typename node_group<node_dataT, tile_traitsT, id_generatorT>::node
node_group<node_dataT, tile_traitsT, id_generatorT>::node::get_relative(
    directions_t const & dir) const
{
  vector_t coords = tile_traitsT::get_relative(m_coords, dir);
  if (coords == invalid_vector) {
    throw exception(CG_INVALID_DIR);
  }

  return (*m_group)(coords);
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
bool
node_group<node_dataT, tile_traitsT, id_generatorT>::node::move_to(
    unit_t const & x, unit_t const & y)
{
  return move_to(vector_t(x, y));
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
bool
node_group<node_dataT, tile_traitsT, id_generatorT>::node::move_to(
    vector_t const & coords)
{
  bool result = m_group->move(m_coords, coords);
  if (result) {
    m_coords = coords;
  }
  return result;
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
directions_t const * const
node_group<node_dataT, tile_traitsT, id_generatorT>::node::available_dirs(
    join_t join_type) const
{
  return tile_traitsT::available_dirs(m_coords, join_type);
}




/*****************************************************************************
 * Class node_group<>
 */

template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
node_group<node_dataT, tile_traitsT, id_generatorT>::node_group()
  : m_id_generator(new id_generatorT())
{
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
node_group<node_dataT, tile_traitsT, id_generatorT>::node_group(
    id_generatorT const & gen)
  : m_id_generator(new id_generatorT(gen))
{
}





template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
typename node_group<node_dataT, tile_traitsT, id_generatorT>::node
node_group<node_dataT, tile_traitsT, id_generatorT>::operator()(
    unit_t const & x, unit_t const & y) const
{
  return operator()(vector_t(x, y));
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
typename node_group<node_dataT, tile_traitsT, id_generatorT>::node
node_group<node_dataT, tile_traitsT, id_generatorT>::operator()(
    vector_t const & coords) const
{
  if (!is_valid(coords)) {
    throw exception(CG_INVALID_COORDS);
  }

  node_id_t id = node_id_t();

  typename coordinate_map_t::const_iterator iter = m_nodes.find(coords);
  if (iter == m_nodes.end()) {
    id = m_id_generator->get_unique_id();
  } else {
    id = iter->second;
  }

  return node(const_cast<node_group *>(this), id, coords);
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
bool
node_group<node_dataT, tile_traitsT, id_generatorT>::is_valid(unit_t const & x,
    unit_t const & y) const
{
  return is_valid(vector_t(x, y));
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
bool
node_group<node_dataT, tile_traitsT, id_generatorT>::is_valid(
    vector_t const & coords) const
{
  return tile_traitsT::is_valid(coords);
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
typename node_group<node_dataT, tile_traitsT, id_generatorT>::node_data_ptr
node_group<node_dataT, tile_traitsT, id_generatorT>::get(
    node_id_t const & id) const
{
  typename node_data_map_t::const_iterator iter = m_node_data.find(id);
  if (iter == m_node_data.end()) {
    return node_data_ptr();
  }
  return iter->second.m_data;
}


template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
void
node_group<node_dataT, tile_traitsT, id_generatorT>::clear()
{
  m_nodes.clear();
  m_node_data.clear();
  m_id_generator->reset();
}




template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
bool
node_group<node_dataT, tile_traitsT, id_generatorT>::is_empty(unit_t const & x,
    unit_t const & y) const
{
  return is_empty(vector_t(x, y));
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
bool
node_group<node_dataT, tile_traitsT, id_generatorT>::is_empty(
    vector_t const & coords) const
{
  typename coordinate_map_t::const_iterator iter = m_nodes.find(coords);
  return (iter == m_nodes.end());
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
vector_t
node_group<node_dataT, tile_traitsT, id_generatorT>::min_coords() const
{
  // XXX Easy to optimize by remembering min/max when nodes are created.
  vector_t ret;

  typename coordinate_map_t::const_iterator node_end = m_nodes.end();
  for (typename coordinate_map_t::const_iterator iter = m_nodes.begin()
      ; iter != node_end ; ++iter)
  {
    if (ret.m_x == invalid_unit) {
      ret.m_x = iter->first.m_x;
    } else {
      ret.m_x = std::min<unit_t>(ret.m_x, iter->first.m_x);
    }

    if (ret.m_y == invalid_unit) {
      ret.m_y = iter->first.m_y;
    } else {
      ret.m_y = std::min<unit_t>(ret.m_y, iter->first.m_y);
    }
  }

  return ret;
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
vector_t
node_group<node_dataT, tile_traitsT, id_generatorT>::max_coords() const
{
  // XXX Easy to optimize by remembering min/max when nodes are created.
  vector_t ret;

  typename coordinate_map_t::const_iterator node_end = m_nodes.end();
  for (typename coordinate_map_t::const_iterator iter = m_nodes.begin()
      ; iter != node_end ; ++iter)
  {
    if (ret.m_x == invalid_unit) {
      ret.m_x = iter->first.m_x;
    } else {
      ret.m_x = std::max<unit_t>(ret.m_x, iter->first.m_x);
    }

    if (ret.m_y == invalid_unit) {
      ret.m_y = iter->first.m_y;
    } else {
      ret.m_y = std::max<unit_t>(ret.m_y, iter->first.m_y);
    }
  }

  // for STL-/pointer-like iteration
  ++ret.m_x;
  ++ret.m_y;
  return ret;
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
size_t
node_group<node_dataT, tile_traitsT, id_generatorT>::size() const
{
  return m_node_data.size();
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
void
node_group<node_dataT, tile_traitsT, id_generatorT>::set(node_id_t const & id,
    vector_t const & coords, node_dataT const & data)
{
  node_data_wrapper & w = m_node_data[id];
  w.m_data = node_data_ptr(new node_dataT(data));
  w.m_coords = coords;
  m_nodes[coords] = id;
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
bool
node_group<node_dataT, tile_traitsT, id_generatorT>::move(vector_t const & from,
    vector_t const & to)
{
  typename coordinate_map_t::iterator iter = m_nodes.find(from);
  if (iter == m_nodes.end()) {
    return false;
  }

  typename coordinate_map_t::const_iterator c_iter = m_nodes.find(to);
  if (c_iter != m_nodes.end()) {
    return false;
  }

  node_id_t id = iter->second;
  m_nodes.erase(iter);
  m_nodes[to] = id;

  return true;
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
bool
node_group<node_dataT, tile_traitsT, id_generatorT>::erase(unit_t const & x,
    unit_t const & y)
{
  return erase(vector_t(x, y));
}



template <
  typename node_dataT,
  typename tile_traitsT,
  typename id_generatorT
>
bool
node_group<node_dataT, tile_traitsT, id_generatorT>::erase(
    vector_t const & coords)
{
  typename coordinate_map_t::iterator iter = m_nodes.find(coords);
  if (iter == m_nodes.end()) {
    return false;
  }

  node_id_t id = iter->second;
  m_nodes.erase(iter);
  m_node_data.erase(id);

  return true;
}



} // namespace cartograph
