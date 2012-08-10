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

#include <sstream>
#include <set>

#include <cppunit/extensions/HelperMacros.h>

#include <cartograph/node_group.h>
#include <cartograph/tile_traits.h>
#include <cartograph/pathfinding.h>
#include <cartograph/heuristics.h>
#include <cartograph/traversal_traits.h>

namespace
{

struct test_node
{
  test_node(bool blocked = false)
    : m_blocked(blocked)
  {
  }

  bool m_blocked;
};



template <typename mapT>
struct traversal_traits
  : public cartograph::pathfinding::simple_traversal_traits<mapT>
{
  traversal_traits(mapT const & m, bool edges_only = false)
    : m_map(m)
    , m_edges_only(edges_only)
  {
  }

  bool
  is_impassable(cartograph::vector_t const & coords,
      cartograph::directions_t const & d)
  {
    return m_map(coords).get_relative(d)->m_blocked;
  }


  cartograph::join_t
  join_types()
  {
    return (m_edges_only ? cartograph::EDGES : cartograph::PATHFINDING_DEFAULT);
  }


  mapT const & m_map;
  bool m_edges_only;
};





template <
  typename tile_traitsT
>
struct expected_results
{
};


template <>
struct expected_results<cartograph::rectangular_tile_traits>
{
  std::deque<cartograph::vector_t> unblocked_results;
  std::deque<cartograph::vector_t> blocked_results;
  std::deque<cartograph::vector_t> edges_results;

  expected_results()
  {
    namespace cg = cartograph;

    unblocked_results.push_back(cg::vector_t(4, 4));
    unblocked_results.push_back(cg::vector_t(5, 4));
    unblocked_results.push_back(cg::vector_t(6, 4));
    unblocked_results.push_back(cg::vector_t(7, 4));
    unblocked_results.push_back(cg::vector_t(8, 4));
    unblocked_results.push_back(cg::vector_t(9, 4));
    unblocked_results.push_back(cg::vector_t(10, 4));
    unblocked_results.push_back(cg::vector_t(11, 4));
    unblocked_results.push_back(cg::vector_t(12, 4));
    unblocked_results.push_back(cg::vector_t(13, 5));
    unblocked_results.push_back(cg::vector_t(14, 6));
    unblocked_results.push_back(cg::vector_t(15, 7));
    unblocked_results.push_back(cg::vector_t(16, 8));
    unblocked_results.push_back(cg::vector_t(17, 9));
    unblocked_results.push_back(cg::vector_t(18, 10));
    unblocked_results.push_back(cg::vector_t(19, 11));
    unblocked_results.push_back(cg::vector_t(20, 12));
    unblocked_results.push_back(cg::vector_t(21, 13));
    unblocked_results.push_back(cg::vector_t(22, 14));
    unblocked_results.push_back(cg::vector_t(23, 15));
    unblocked_results.push_back(cg::vector_t(24, 16));
    unblocked_results.push_back(cg::vector_t(25, 17));
    unblocked_results.push_back(cg::vector_t(26, 18));
    unblocked_results.push_back(cg::vector_t(27, 19));
    unblocked_results.push_back(cg::vector_t(28, 20));
    unblocked_results.push_back(cg::vector_t(29, 21));
    unblocked_results.push_back(cg::vector_t(30, 22));
    unblocked_results.push_back(cg::vector_t(31, 23));
    unblocked_results.push_back(cg::vector_t(32, 24));
    unblocked_results.push_back(cg::vector_t(33, 25));
    unblocked_results.push_back(cg::vector_t(34, 26));
    unblocked_results.push_back(cg::vector_t(35, 27));
    unblocked_results.push_back(cg::vector_t(36, 28));
    unblocked_results.push_back(cg::vector_t(37, 29));
    unblocked_results.push_back(cg::vector_t(38, 30));
    unblocked_results.push_back(cg::vector_t(39, 31));
    unblocked_results.push_back(cg::vector_t(40, 32));
    unblocked_results.push_back(cg::vector_t(41, 33));
    unblocked_results.push_back(cg::vector_t(42, 34));
    unblocked_results.push_back(cg::vector_t(43, 35));
    unblocked_results.push_back(cg::vector_t(44, 36));
    unblocked_results.push_back(cg::vector_t(45, 37));


    blocked_results.push_back(cg::vector_t(4, 4));
    blocked_results.push_back(cg::vector_t(5, 4));
    blocked_results.push_back(cg::vector_t(6, 4));
    blocked_results.push_back(cg::vector_t(7, 4));
    blocked_results.push_back(cg::vector_t(8, 4));
    blocked_results.push_back(cg::vector_t(9, 4));
    blocked_results.push_back(cg::vector_t(10, 4));
    blocked_results.push_back(cg::vector_t(11, 4));
    blocked_results.push_back(cg::vector_t(12, 4));
    blocked_results.push_back(cg::vector_t(13, 4));
    blocked_results.push_back(cg::vector_t(14, 4));
    blocked_results.push_back(cg::vector_t(15, 4));
    blocked_results.push_back(cg::vector_t(16, 4));
    blocked_results.push_back(cg::vector_t(17, 4));
    blocked_results.push_back(cg::vector_t(18, 4));
    blocked_results.push_back(cg::vector_t(19, 4));
    blocked_results.push_back(cg::vector_t(20, 4));
    blocked_results.push_back(cg::vector_t(21, 4));
    blocked_results.push_back(cg::vector_t(22, 4));
    blocked_results.push_back(cg::vector_t(23, 4));
    blocked_results.push_back(cg::vector_t(24, 4));
    blocked_results.push_back(cg::vector_t(25, 5));
    blocked_results.push_back(cg::vector_t(26, 6));
    blocked_results.push_back(cg::vector_t(26, 7));
    blocked_results.push_back(cg::vector_t(26, 8));
    blocked_results.push_back(cg::vector_t(26, 9));
    blocked_results.push_back(cg::vector_t(26, 10));
    blocked_results.push_back(cg::vector_t(26, 11));
    blocked_results.push_back(cg::vector_t(26, 12));
    blocked_results.push_back(cg::vector_t(26, 13));
    blocked_results.push_back(cg::vector_t(26, 14));
    blocked_results.push_back(cg::vector_t(26, 15));
    blocked_results.push_back(cg::vector_t(26, 16));
    blocked_results.push_back(cg::vector_t(26, 17));
    blocked_results.push_back(cg::vector_t(26, 18));
    blocked_results.push_back(cg::vector_t(27, 19));
    blocked_results.push_back(cg::vector_t(28, 20));
    blocked_results.push_back(cg::vector_t(29, 21));
    blocked_results.push_back(cg::vector_t(30, 22));
    blocked_results.push_back(cg::vector_t(31, 23));
    blocked_results.push_back(cg::vector_t(32, 24));
    blocked_results.push_back(cg::vector_t(33, 25));
    blocked_results.push_back(cg::vector_t(34, 26));
    blocked_results.push_back(cg::vector_t(35, 27));
    blocked_results.push_back(cg::vector_t(36, 28));
    blocked_results.push_back(cg::vector_t(37, 29));
    blocked_results.push_back(cg::vector_t(38, 30));
    blocked_results.push_back(cg::vector_t(39, 31));
    blocked_results.push_back(cg::vector_t(40, 32));
    blocked_results.push_back(cg::vector_t(41, 33));
    blocked_results.push_back(cg::vector_t(42, 34));
    blocked_results.push_back(cg::vector_t(43, 35));
    blocked_results.push_back(cg::vector_t(44, 36));
    blocked_results.push_back(cg::vector_t(45, 37));


    edges_results.push_back(cg::vector_t(4, 4));
    edges_results.push_back(cg::vector_t(5, 4));
    edges_results.push_back(cg::vector_t(6, 4));
    edges_results.push_back(cg::vector_t(7, 4));
    edges_results.push_back(cg::vector_t(8, 4));
    edges_results.push_back(cg::vector_t(9, 4));
    edges_results.push_back(cg::vector_t(10, 4));
    edges_results.push_back(cg::vector_t(11, 4));
    edges_results.push_back(cg::vector_t(12, 4));
    edges_results.push_back(cg::vector_t(13, 4));
    edges_results.push_back(cg::vector_t(14, 4));
    edges_results.push_back(cg::vector_t(15, 4));
    edges_results.push_back(cg::vector_t(16, 4));
    edges_results.push_back(cg::vector_t(17, 4));
    edges_results.push_back(cg::vector_t(18, 4));
    edges_results.push_back(cg::vector_t(19, 4));
    edges_results.push_back(cg::vector_t(20, 4));
    edges_results.push_back(cg::vector_t(21, 4));
    edges_results.push_back(cg::vector_t(22, 4));
    edges_results.push_back(cg::vector_t(23, 4));
    edges_results.push_back(cg::vector_t(24, 4));
    edges_results.push_back(cg::vector_t(25, 4));
    edges_results.push_back(cg::vector_t(26, 4));
    edges_results.push_back(cg::vector_t(27, 4));
    edges_results.push_back(cg::vector_t(28, 4));
    edges_results.push_back(cg::vector_t(29, 4));
    edges_results.push_back(cg::vector_t(30, 4));
    edges_results.push_back(cg::vector_t(31, 4));
    edges_results.push_back(cg::vector_t(32, 4));
    edges_results.push_back(cg::vector_t(33, 4));
    edges_results.push_back(cg::vector_t(34, 4));
    edges_results.push_back(cg::vector_t(35, 4));
    edges_results.push_back(cg::vector_t(36, 4));
    edges_results.push_back(cg::vector_t(37, 4));
    edges_results.push_back(cg::vector_t(38, 4));
    edges_results.push_back(cg::vector_t(39, 4));
    edges_results.push_back(cg::vector_t(40, 4));
    edges_results.push_back(cg::vector_t(41, 4));
    edges_results.push_back(cg::vector_t(42, 4));
    edges_results.push_back(cg::vector_t(43, 4));
    edges_results.push_back(cg::vector_t(44, 4));
    edges_results.push_back(cg::vector_t(45, 4));
    edges_results.push_back(cg::vector_t(45, 5));
    edges_results.push_back(cg::vector_t(45, 6));
    edges_results.push_back(cg::vector_t(45, 7));
    edges_results.push_back(cg::vector_t(45, 8));
    edges_results.push_back(cg::vector_t(45, 9));
    edges_results.push_back(cg::vector_t(45, 10));
    edges_results.push_back(cg::vector_t(45, 11));
    edges_results.push_back(cg::vector_t(45, 12));
    edges_results.push_back(cg::vector_t(45, 13));
    edges_results.push_back(cg::vector_t(45, 14));
    edges_results.push_back(cg::vector_t(45, 15));
    edges_results.push_back(cg::vector_t(45, 16));
    edges_results.push_back(cg::vector_t(45, 17));
    edges_results.push_back(cg::vector_t(45, 18));
    edges_results.push_back(cg::vector_t(45, 19));
    edges_results.push_back(cg::vector_t(45, 20));
    edges_results.push_back(cg::vector_t(45, 21));
    edges_results.push_back(cg::vector_t(45, 22));
    edges_results.push_back(cg::vector_t(45, 23));
    edges_results.push_back(cg::vector_t(45, 24));
    edges_results.push_back(cg::vector_t(45, 25));
    edges_results.push_back(cg::vector_t(45, 26));
    edges_results.push_back(cg::vector_t(45, 27));
    edges_results.push_back(cg::vector_t(45, 28));
    edges_results.push_back(cg::vector_t(45, 29));
    edges_results.push_back(cg::vector_t(45, 30));
    edges_results.push_back(cg::vector_t(45, 31));
    edges_results.push_back(cg::vector_t(45, 32));
    edges_results.push_back(cg::vector_t(45, 33));
    edges_results.push_back(cg::vector_t(45, 34));
    edges_results.push_back(cg::vector_t(45, 35));
    edges_results.push_back(cg::vector_t(45, 36));
    edges_results.push_back(cg::vector_t(45, 37));




  }


};



template <>
struct expected_results<cartograph::triangular_tile_traits>
{
  std::deque<cartograph::vector_t> unblocked_results;
  std::deque<cartograph::vector_t> blocked_results;
  std::deque<cartograph::vector_t> edges_results;

  expected_results()
  {
    namespace cg = cartograph;

    unblocked_results.push_back(cg::vector_t(4, 4));
    unblocked_results.push_back(cg::vector_t(4, 5));
    unblocked_results.push_back(cg::vector_t(5, 5));
    unblocked_results.push_back(cg::vector_t(7, 6));
    unblocked_results.push_back(cg::vector_t(8, 6));
    unblocked_results.push_back(cg::vector_t(10, 7));
    unblocked_results.push_back(cg::vector_t(11, 7));
    unblocked_results.push_back(cg::vector_t(13, 8));
    unblocked_results.push_back(cg::vector_t(14, 8));
    unblocked_results.push_back(cg::vector_t(16, 9));
    unblocked_results.push_back(cg::vector_t(17, 9));
    unblocked_results.push_back(cg::vector_t(19, 10));
    unblocked_results.push_back(cg::vector_t(19, 11));
    unblocked_results.push_back(cg::vector_t(21, 12));
    unblocked_results.push_back(cg::vector_t(21, 13));
    unblocked_results.push_back(cg::vector_t(23, 14));
    unblocked_results.push_back(cg::vector_t(23, 15));
    unblocked_results.push_back(cg::vector_t(25, 16));
    unblocked_results.push_back(cg::vector_t(25, 17));
    unblocked_results.push_back(cg::vector_t(27, 18));
    unblocked_results.push_back(cg::vector_t(27, 19));
    unblocked_results.push_back(cg::vector_t(29, 20));
    unblocked_results.push_back(cg::vector_t(29, 21));
    unblocked_results.push_back(cg::vector_t(31, 22));
    unblocked_results.push_back(cg::vector_t(31, 23));
    unblocked_results.push_back(cg::vector_t(33, 24));
    unblocked_results.push_back(cg::vector_t(33, 25));
    unblocked_results.push_back(cg::vector_t(35, 26));
    unblocked_results.push_back(cg::vector_t(35, 27));
    unblocked_results.push_back(cg::vector_t(37, 28));
    unblocked_results.push_back(cg::vector_t(37, 29));
    unblocked_results.push_back(cg::vector_t(39, 30));
    unblocked_results.push_back(cg::vector_t(39, 31));
    unblocked_results.push_back(cg::vector_t(41, 32));
    unblocked_results.push_back(cg::vector_t(41, 33));
    unblocked_results.push_back(cg::vector_t(43, 34));
    unblocked_results.push_back(cg::vector_t(43, 35));
    unblocked_results.push_back(cg::vector_t(45, 36));
    unblocked_results.push_back(cg::vector_t(45, 37));

    blocked_results.push_back(cg::vector_t(4, 4));
    blocked_results.push_back(cg::vector_t(6, 5));
    blocked_results.push_back(cg::vector_t(7, 5));
    blocked_results.push_back(cg::vector_t(9, 6));
    blocked_results.push_back(cg::vector_t(10, 6));
    blocked_results.push_back(cg::vector_t(12, 7));
    blocked_results.push_back(cg::vector_t(13, 7));
    blocked_results.push_back(cg::vector_t(15, 8));
    blocked_results.push_back(cg::vector_t(16, 8));
    blocked_results.push_back(cg::vector_t(18, 9));
    blocked_results.push_back(cg::vector_t(19, 9));
    blocked_results.push_back(cg::vector_t(21, 10));
    blocked_results.push_back(cg::vector_t(22, 10));
    blocked_results.push_back(cg::vector_t(24, 11));
    blocked_results.push_back(cg::vector_t(24, 12));
    blocked_results.push_back(cg::vector_t(26, 13));
    blocked_results.push_back(cg::vector_t(27, 13));
    blocked_results.push_back(cg::vector_t(27, 14));
    blocked_results.push_back(cg::vector_t(27, 15));
    blocked_results.push_back(cg::vector_t(27, 16));
    blocked_results.push_back(cg::vector_t(27, 17));
    blocked_results.push_back(cg::vector_t(27, 18));
    blocked_results.push_back(cg::vector_t(27, 19));
    blocked_results.push_back(cg::vector_t(29, 20));
    blocked_results.push_back(cg::vector_t(29, 21));
    blocked_results.push_back(cg::vector_t(31, 22));
    blocked_results.push_back(cg::vector_t(31, 23));
    blocked_results.push_back(cg::vector_t(33, 24));
    blocked_results.push_back(cg::vector_t(33, 25));
    blocked_results.push_back(cg::vector_t(35, 26));
    blocked_results.push_back(cg::vector_t(35, 27));
    blocked_results.push_back(cg::vector_t(37, 28));
    blocked_results.push_back(cg::vector_t(37, 29));
    blocked_results.push_back(cg::vector_t(39, 30));
    blocked_results.push_back(cg::vector_t(39, 31));
    blocked_results.push_back(cg::vector_t(41, 32));
    blocked_results.push_back(cg::vector_t(41, 33));
    blocked_results.push_back(cg::vector_t(43, 34));
    blocked_results.push_back(cg::vector_t(43, 35));
    blocked_results.push_back(cg::vector_t(45, 36));
    blocked_results.push_back(cg::vector_t(45, 37));

    edges_results.push_back(cg::vector_t(4, 4));
    edges_results.push_back(cg::vector_t(5, 4));
    edges_results.push_back(cg::vector_t(6, 4));
    edges_results.push_back(cg::vector_t(7, 4));
    edges_results.push_back(cg::vector_t(8, 4));
    edges_results.push_back(cg::vector_t(9, 4));
    edges_results.push_back(cg::vector_t(10, 4));
    edges_results.push_back(cg::vector_t(11, 4));
    edges_results.push_back(cg::vector_t(12, 4));
    edges_results.push_back(cg::vector_t(13, 4));
    edges_results.push_back(cg::vector_t(14, 4));
    edges_results.push_back(cg::vector_t(15, 4));
    edges_results.push_back(cg::vector_t(16, 4));
    edges_results.push_back(cg::vector_t(17, 4));
    edges_results.push_back(cg::vector_t(18, 4));
    edges_results.push_back(cg::vector_t(19, 4));
    edges_results.push_back(cg::vector_t(20, 4));
    edges_results.push_back(cg::vector_t(21, 4));
    edges_results.push_back(cg::vector_t(22, 4));
    edges_results.push_back(cg::vector_t(23, 4));
    edges_results.push_back(cg::vector_t(24, 4));
    edges_results.push_back(cg::vector_t(25, 4));
    edges_results.push_back(cg::vector_t(26, 4));
    edges_results.push_back(cg::vector_t(26, 5));
    edges_results.push_back(cg::vector_t(27, 5));
    edges_results.push_back(cg::vector_t(27, 6));
    edges_results.push_back(cg::vector_t(28, 6));
    edges_results.push_back(cg::vector_t(28, 7));
    edges_results.push_back(cg::vector_t(29, 7));
    edges_results.push_back(cg::vector_t(29, 8));
    edges_results.push_back(cg::vector_t(30, 8));
    edges_results.push_back(cg::vector_t(30, 9));
    edges_results.push_back(cg::vector_t(31, 9));
    edges_results.push_back(cg::vector_t(31, 10));
    edges_results.push_back(cg::vector_t(32, 10));
    edges_results.push_back(cg::vector_t(32, 11));
    edges_results.push_back(cg::vector_t(33, 11));
    edges_results.push_back(cg::vector_t(33, 12));
    edges_results.push_back(cg::vector_t(34, 12));
    edges_results.push_back(cg::vector_t(34, 13));
    edges_results.push_back(cg::vector_t(35, 13));
    edges_results.push_back(cg::vector_t(35, 14));
    edges_results.push_back(cg::vector_t(36, 14));
    edges_results.push_back(cg::vector_t(36, 15));
    edges_results.push_back(cg::vector_t(37, 15));
    edges_results.push_back(cg::vector_t(37, 16));
    edges_results.push_back(cg::vector_t(38, 16));
    edges_results.push_back(cg::vector_t(38, 17));
    edges_results.push_back(cg::vector_t(39, 17));
    edges_results.push_back(cg::vector_t(39, 18));
    edges_results.push_back(cg::vector_t(40, 18));
    edges_results.push_back(cg::vector_t(40, 19));
    edges_results.push_back(cg::vector_t(41, 19));
    edges_results.push_back(cg::vector_t(41, 20));
    edges_results.push_back(cg::vector_t(42, 20));
    edges_results.push_back(cg::vector_t(42, 21));
    edges_results.push_back(cg::vector_t(43, 21));
    edges_results.push_back(cg::vector_t(43, 22));
    edges_results.push_back(cg::vector_t(44, 22));
    edges_results.push_back(cg::vector_t(44, 23));
    edges_results.push_back(cg::vector_t(45, 23));
    edges_results.push_back(cg::vector_t(45, 24));
    edges_results.push_back(cg::vector_t(46, 24));
    edges_results.push_back(cg::vector_t(46, 25));
    edges_results.push_back(cg::vector_t(47, 25));
    edges_results.push_back(cg::vector_t(47, 26));
    edges_results.push_back(cg::vector_t(48, 26));
    edges_results.push_back(cg::vector_t(48, 27));
    edges_results.push_back(cg::vector_t(49, 27));
    edges_results.push_back(cg::vector_t(49, 28));
    edges_results.push_back(cg::vector_t(48, 28));
    edges_results.push_back(cg::vector_t(48, 29));
    edges_results.push_back(cg::vector_t(49, 29));
    edges_results.push_back(cg::vector_t(49, 30));
    edges_results.push_back(cg::vector_t(48, 30));
    edges_results.push_back(cg::vector_t(48, 31));
    edges_results.push_back(cg::vector_t(49, 31));
    edges_results.push_back(cg::vector_t(49, 32));
    edges_results.push_back(cg::vector_t(48, 32));
    edges_results.push_back(cg::vector_t(48, 33));
    edges_results.push_back(cg::vector_t(49, 33));
    edges_results.push_back(cg::vector_t(49, 34));
    edges_results.push_back(cg::vector_t(48, 34));
    edges_results.push_back(cg::vector_t(48, 35));
    edges_results.push_back(cg::vector_t(47, 35));
    edges_results.push_back(cg::vector_t(47, 36));
    edges_results.push_back(cg::vector_t(46, 36));
    edges_results.push_back(cg::vector_t(46, 37));
    edges_results.push_back(cg::vector_t(45, 37));

  }
};



template <>
struct expected_results<cartograph::hexagonal_tile_traits>
{
  std::deque<cartograph::vector_t> unblocked_results;
  std::deque<cartograph::vector_t> blocked_results;
  std::deque<cartograph::vector_t> edges_results;

  expected_results()
  {
    namespace cg = cartograph;
    unblocked_results.push_back(cg::vector_t(4, 4));
    unblocked_results.push_back(cg::vector_t(5, 3));
    unblocked_results.push_back(cg::vector_t(6, 2));
    unblocked_results.push_back(cg::vector_t(7, 1));
    unblocked_results.push_back(cg::vector_t(8, 0));
    unblocked_results.push_back(cg::vector_t(9, 1));
    unblocked_results.push_back(cg::vector_t(10, 2));
    unblocked_results.push_back(cg::vector_t(11, 3));
    unblocked_results.push_back(cg::vector_t(12, 4));
    unblocked_results.push_back(cg::vector_t(13, 5));
    unblocked_results.push_back(cg::vector_t(14, 6));
    unblocked_results.push_back(cg::vector_t(15, 7));
    unblocked_results.push_back(cg::vector_t(16, 8));
    unblocked_results.push_back(cg::vector_t(17, 9));
    unblocked_results.push_back(cg::vector_t(18, 10));
    unblocked_results.push_back(cg::vector_t(19, 11));
    unblocked_results.push_back(cg::vector_t(20, 12));
    unblocked_results.push_back(cg::vector_t(21, 13));
    unblocked_results.push_back(cg::vector_t(22, 14));
    unblocked_results.push_back(cg::vector_t(23, 15));
    unblocked_results.push_back(cg::vector_t(24, 16));
    unblocked_results.push_back(cg::vector_t(25, 17));
    unblocked_results.push_back(cg::vector_t(26, 18));
    unblocked_results.push_back(cg::vector_t(27, 19));
    unblocked_results.push_back(cg::vector_t(28, 20));
    unblocked_results.push_back(cg::vector_t(29, 21));
    unblocked_results.push_back(cg::vector_t(30, 22));
    unblocked_results.push_back(cg::vector_t(31, 23));
    unblocked_results.push_back(cg::vector_t(32, 24));
    unblocked_results.push_back(cg::vector_t(33, 25));
    unblocked_results.push_back(cg::vector_t(34, 26));
    unblocked_results.push_back(cg::vector_t(35, 27));
    unblocked_results.push_back(cg::vector_t(36, 28));
    unblocked_results.push_back(cg::vector_t(37, 29));
    unblocked_results.push_back(cg::vector_t(38, 30));
    unblocked_results.push_back(cg::vector_t(39, 31));
    unblocked_results.push_back(cg::vector_t(40, 32));
    unblocked_results.push_back(cg::vector_t(41, 33));
    unblocked_results.push_back(cg::vector_t(42, 34));
    unblocked_results.push_back(cg::vector_t(43, 35));
    unblocked_results.push_back(cg::vector_t(44, 36));
    unblocked_results.push_back(cg::vector_t(45, 37));

    blocked_results.push_back(cg::vector_t(4, 4));
    blocked_results.push_back(cg::vector_t(5, 3));
    blocked_results.push_back(cg::vector_t(6, 2));
    blocked_results.push_back(cg::vector_t(7, 1));
    blocked_results.push_back(cg::vector_t(8, 0));
    blocked_results.push_back(cg::vector_t(9, 1));
    blocked_results.push_back(cg::vector_t(10, 0));
    blocked_results.push_back(cg::vector_t(11, 1));
    blocked_results.push_back(cg::vector_t(12, 0));
    blocked_results.push_back(cg::vector_t(13, 1));
    blocked_results.push_back(cg::vector_t(14, 0));
    blocked_results.push_back(cg::vector_t(15, 1));
    blocked_results.push_back(cg::vector_t(16, 0));
    blocked_results.push_back(cg::vector_t(17, 1));
    blocked_results.push_back(cg::vector_t(18, 0));
    blocked_results.push_back(cg::vector_t(19, 1));
    blocked_results.push_back(cg::vector_t(20, 0));
    blocked_results.push_back(cg::vector_t(21, 1));
    blocked_results.push_back(cg::vector_t(22, 2));
    blocked_results.push_back(cg::vector_t(23, 3));
    blocked_results.push_back(cg::vector_t(24, 4));
    blocked_results.push_back(cg::vector_t(25, 5));
    blocked_results.push_back(cg::vector_t(26, 6));
    blocked_results.push_back(cg::vector_t(27, 7));
    blocked_results.push_back(cg::vector_t(28, 8));
    blocked_results.push_back(cg::vector_t(29, 9));
    blocked_results.push_back(cg::vector_t(30, 10));
    blocked_results.push_back(cg::vector_t(31, 11));
    blocked_results.push_back(cg::vector_t(32, 12));
    blocked_results.push_back(cg::vector_t(33, 13));
    blocked_results.push_back(cg::vector_t(34, 14));
    blocked_results.push_back(cg::vector_t(35, 15));
    blocked_results.push_back(cg::vector_t(36, 16));
    blocked_results.push_back(cg::vector_t(37, 17));
    blocked_results.push_back(cg::vector_t(38, 18));
    blocked_results.push_back(cg::vector_t(39, 19));
    blocked_results.push_back(cg::vector_t(40, 20));
    blocked_results.push_back(cg::vector_t(41, 21));
    blocked_results.push_back(cg::vector_t(42, 22));
    blocked_results.push_back(cg::vector_t(43, 23));
    blocked_results.push_back(cg::vector_t(44, 24));
    blocked_results.push_back(cg::vector_t(45, 25));
    blocked_results.push_back(cg::vector_t(45, 27));
    blocked_results.push_back(cg::vector_t(45, 29));
    blocked_results.push_back(cg::vector_t(45, 31));
    blocked_results.push_back(cg::vector_t(45, 33));
    blocked_results.push_back(cg::vector_t(45, 35));
    blocked_results.push_back(cg::vector_t(45, 37));
    edges_results.push_back(cg::vector_t(4, 4));
    edges_results.push_back(cg::vector_t(5, 3));
    edges_results.push_back(cg::vector_t(6, 2));
    edges_results.push_back(cg::vector_t(7, 1));
    edges_results.push_back(cg::vector_t(8, 0));
    edges_results.push_back(cg::vector_t(9, 1));
    edges_results.push_back(cg::vector_t(10, 0));
    edges_results.push_back(cg::vector_t(11, 1));
    edges_results.push_back(cg::vector_t(12, 0));
    edges_results.push_back(cg::vector_t(13, 1));
    edges_results.push_back(cg::vector_t(14, 0));
    edges_results.push_back(cg::vector_t(15, 1));
    edges_results.push_back(cg::vector_t(16, 0));
    edges_results.push_back(cg::vector_t(17, 1));
    edges_results.push_back(cg::vector_t(18, 0));
    edges_results.push_back(cg::vector_t(19, 1));
    edges_results.push_back(cg::vector_t(20, 0));
    edges_results.push_back(cg::vector_t(21, 1));
    edges_results.push_back(cg::vector_t(22, 2));
    edges_results.push_back(cg::vector_t(23, 3));
    edges_results.push_back(cg::vector_t(24, 4));
    edges_results.push_back(cg::vector_t(25, 5));
    edges_results.push_back(cg::vector_t(26, 6));
    edges_results.push_back(cg::vector_t(27, 7));
    edges_results.push_back(cg::vector_t(28, 8));
    edges_results.push_back(cg::vector_t(29, 9));
    edges_results.push_back(cg::vector_t(30, 10));
    edges_results.push_back(cg::vector_t(31, 11));
    edges_results.push_back(cg::vector_t(32, 12));
    edges_results.push_back(cg::vector_t(33, 13));
    edges_results.push_back(cg::vector_t(34, 14));
    edges_results.push_back(cg::vector_t(35, 15));
    edges_results.push_back(cg::vector_t(36, 16));
    edges_results.push_back(cg::vector_t(37, 17));
    edges_results.push_back(cg::vector_t(38, 18));
    edges_results.push_back(cg::vector_t(39, 19));
    edges_results.push_back(cg::vector_t(40, 20));
    edges_results.push_back(cg::vector_t(41, 21));
    edges_results.push_back(cg::vector_t(42, 22));
    edges_results.push_back(cg::vector_t(43, 23));
    edges_results.push_back(cg::vector_t(44, 24));
    edges_results.push_back(cg::vector_t(45, 25));
    edges_results.push_back(cg::vector_t(45, 27));
    edges_results.push_back(cg::vector_t(45, 29));
    edges_results.push_back(cg::vector_t(45, 31));
    edges_results.push_back(cg::vector_t(45, 33));
    edges_results.push_back(cg::vector_t(45, 35));
    edges_results.push_back(cg::vector_t(45, 37));


  }
};

} // anonymous namespace

template <
  typename tile_traitsT
>
class PathfindingTest
  : public CppUnit::TestFixture
{
public:
  CPPUNIT_TEST_SUITE(PathfindingTest<tile_traitsT>);

    CPPUNIT_TEST(testDijkstra);
    CPPUNIT_TEST(testDijkstraBlocked);
    CPPUNIT_TEST(testDijkstraBlockedEdgesOnly);

  CPPUNIT_TEST_SUITE_END();

public:
  void setUp()
  {
    // Find a path
    namespace cg = cartograph;
    start = cg::vector_t(4, 4);
    end = cg::vector_t(45, 37);


    for (uint32_t x = 0 ; x < 50 ; ++x) {
      for (uint32_t y = 0 ; y < 50 ; ++y) {
        if (test_map.is_valid(x, y)) {
          test_map(x, y) = test_node();
        }
      }
    }


    for (uint32_t x = 0 ; x < 50 ; ++x) {
      for (uint32_t y = 0 ; y < 50 ; ++y) {
        if (test_map.is_valid(x, y)) {
          bool blocked = false;
          if (x == 25 && y > 5 && y < 45) {
            blocked = true;
          }
          blocked_test_map(x, y) = test_node(blocked);
        }
      }
    }
  }


  void tearDown()
  {
    test_map.clear();
    blocked_test_map.clear();
  }


  typedef cartograph::node_group<test_node, tile_traitsT> test_map_t;
  test_map_t test_map;
  test_map_t blocked_test_map;

  cartograph::vector_t start;
  cartograph::vector_t end;

private:


  void testDijkstra()
  {
    namespace cg = cartograph;
    namespace cgp = cartograph::pathfinding;
    namespace cgph = cartograph::pathfinding::heuristics;

    // Find a path
    cg::vector_t start(4, 4);
    cg::vector_t end(45, 37);

    std::deque<cg::vector_t> result;

    cgp::simple_traversal_traits<test_map_t> stt;
    CPPUNIT_ASSERT_EQUAL(cg::CG_OK, cgp::a_star(result, test_map, start, end,
          stt, &cgph::dijkstra<
                  test_map_t,
                  cgp::simple_traversal_traits<test_map_t>
               >));

    expected_results<tile_traitsT> expected;
    CPPUNIT_ASSERT_EQUAL(expected.unblocked_results.size(), result.size());
    CPPUNIT_ASSERT(expected.unblocked_results == result);
  }


  void testDijkstraBlocked()
  {
    namespace cg = cartograph;
    namespace cgp = cartograph::pathfinding;
    namespace cgph = cartograph::pathfinding::heuristics;

    std::deque<cg::vector_t> result;

    traversal_traits<test_map_t> stt(blocked_test_map);
    CPPUNIT_ASSERT_EQUAL(cg::CG_OK, cgp::a_star(result, blocked_test_map, start,
          end, stt, &cgph::dijkstra<
                  test_map_t,
                  traversal_traits<test_map_t>
               >));

    expected_results<tile_traitsT> expected;
    CPPUNIT_ASSERT_EQUAL(expected.blocked_results.size(), result.size());
    CPPUNIT_ASSERT(expected.blocked_results == result);

    CPPUNIT_ASSERT(expected.unblocked_results.size() < result.size());
  }


  void testDijkstraBlockedEdgesOnly()
  {
    namespace cg = cartograph;
    namespace cgp = cartograph::pathfinding;
    namespace cgph = cartograph::pathfinding::heuristics;

    std::deque<cg::vector_t> result;

    traversal_traits<test_map_t> stt(blocked_test_map, true);
    CPPUNIT_ASSERT_EQUAL(cg::CG_OK, cgp::a_star(result, blocked_test_map, start,
          end, stt, &cgph::dijkstra<
                  test_map_t,
                  traversal_traits<test_map_t>
               >));

    expected_results<tile_traitsT> expected;
    CPPUNIT_ASSERT_EQUAL(expected.edges_results.size(), result.size());
    CPPUNIT_ASSERT(expected.edges_results == result);

    // They're the same for hexagons, but smaller for all other tile types
    CPPUNIT_ASSERT(expected.blocked_results.size() <= result.size());
  }


};

CPPUNIT_TEST_SUITE_REGISTRATION(PathfindingTest<cartograph::triangular_tile_traits>);
CPPUNIT_TEST_SUITE_REGISTRATION(PathfindingTest<cartograph::rectangular_tile_traits>);
CPPUNIT_TEST_SUITE_REGISTRATION(PathfindingTest<cartograph::hexagonal_tile_traits>);
