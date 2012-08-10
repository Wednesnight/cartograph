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

namespace
{

struct empty_test_node
{
};

template <
  typename tile_traitsT
>
struct expected_results
{
  static uint32_t const neighbour_nodes;
  static uint32_t const edge_neighbour_nodes;
};


template <>
uint32_t const
expected_results<cartograph::triangular_tile_traits>::neighbour_nodes = 12;

template <>
uint32_t const
expected_results<cartograph::rectangular_tile_traits>::neighbour_nodes = 8;

template <>
uint32_t const
expected_results<cartograph::hexagonal_tile_traits>::neighbour_nodes = 6;


template <>
uint32_t const
expected_results<cartograph::triangular_tile_traits>::edge_neighbour_nodes = 3;

template <>
uint32_t const
expected_results<cartograph::rectangular_tile_traits>::edge_neighbour_nodes = 4;

template <>
uint32_t const
expected_results<cartograph::hexagonal_tile_traits>::edge_neighbour_nodes = 6;



} // anonymous namespace

template <
  typename tile_traitsT
>
class NodeGroupTest
  : public CppUnit::TestFixture
{
public:
  CPPUNIT_TEST_SUITE(NodeGroupTest<tile_traitsT>);

    CPPUNIT_TEST(testConcepts);
    CPPUNIT_TEST(testDirIterAll);
    CPPUNIT_TEST(testDirIterEdges);
    CPPUNIT_TEST(testUniqueId);
    CPPUNIT_TEST(testCoordinates);
    CPPUNIT_TEST(testBoundary);
    CPPUNIT_TEST(testMoving);
    CPPUNIT_TEST(testErase);

  CPPUNIT_TEST_SUITE_END();

public:
  void setUp()
  {
    for (uint32_t x = 0 ; x < 50 ; ++x) {
      for (uint32_t y = 0 ; y < 50 ; ++y) {
        if (empty_test_map.is_valid(x, y)) {
          empty_test_map(x, y) = empty_test_node();
        }
      }
    }
  }


  void tearDown()
  {
    empty_test_map.clear();
  }


  typedef cartograph::node_group<empty_test_node, tile_traitsT> empty_test_map_t;
  empty_test_map_t empty_test_map;

private:


  void testConcepts()
  {
    // This test just has to compile. It's essentially superfluous given that
    // the class itself wouldn't compile either, but having this test ensures
    // we can change setUp()/tearDown() at will while still ensuring the below
    // compiles.

    namespace cg = cartograph;

    // test tile traits concepts.
    cg::node_group<empty_test_node, tile_traitsT> ng1;
  }


  void testDirIterAll()
  {
    namespace cg = cartograph;

    // all available dirs
    typename empty_test_map_t::node n = empty_test_map(5, 5);

    cg::directions_t const * d = n.available_dirs(cg::ALL_JOIN_TYPES);
    uint32_t count = 0;
    for ( ; *d != cg::DIR_END ; ++d, ++count) {
      // May not throw.
      typename empty_test_map_t::node n2 = n.get_relative(*d);
    }

    CPPUNIT_ASSERT_EQUAL(expected_results<tile_traitsT>::neighbour_nodes, count);
  }


  void testDirIterEdges()
  {
    namespace cg = cartograph;

    // only edge dirs
    typename empty_test_map_t::node n = empty_test_map(5, 5);

    cg::directions_t const * d = n.available_dirs(cg::EDGES);
    uint32_t count = 0;
    for ( ; *d != cg::DIR_END ; ++d, ++count) {
      // May not throw.
      typename empty_test_map_t::node n2 = n.get_relative(*d);
    }

    CPPUNIT_ASSERT_EQUAL(expected_results<tile_traitsT>::edge_neighbour_nodes, count);
  }



  void testUniqueId()
  {
    namespace cg = cartograph;

    std::set<typename empty_test_map_t::node_id_t> ids;

    for (uint32_t x = 0 ; x < 50 ; ++x) {
      for (uint32_t y = 0 ; y < 50 ; ++y) {
        if (empty_test_map.is_valid(x, y)) {
          size_t prevcount = ids.size();
          typename empty_test_map_t::node n = empty_test_map(x, y);
          ids.insert(n.id());

          // If after inserting each nodes id the containers size becomes
          // larger by one, due to the set's uniqueness-contstraint, that means
          // each id was unique.
          // It also implicitly means that the node_group truly stores unique
          // data per unique coordinate set.
          CPPUNIT_ASSERT_EQUAL(prevcount + 1, ids.size());
        }
      }
    }
  }


  void testCoordinates()
  {
    namespace cg = cartograph;

    std::set<typename empty_test_map_t::node_id_t> ids;

    for (uint32_t x = 0 ; x < 50 ; ++x) {
      for (uint32_t y = 0 ; y < 50 ; ++y) {
        if (empty_test_map.is_valid(x, y)) {
          typename empty_test_map_t::node n = empty_test_map(x, y);

          // Test that the node has the same idea of it's coordinates as the
          // owning node_group...
          CPPUNIT_ASSERT_EQUAL(cg::vector_t(x, y), n.coordinates());
        }
      }
    }
  }


  void testBoundary()
  {
    namespace cg = cartograph;

    // Test whether iteration over all nodes works as expected.
    cg::vector_t min = empty_test_map.min_coords();
    cg::vector_t max = empty_test_map.max_coords();
    CPPUNIT_ASSERT_EQUAL(cg::vector_t(0, 0), min);
    CPPUNIT_ASSERT_EQUAL(cg::vector_t(50, 50), max);


    size_t count = 0;
    for (cg::unit_t x = min.m_x ; x < max.m_x ; ++x) {
      for (cg::unit_t y = min.m_y ; y < max.m_y ; ++y) {
        if (empty_test_map.is_empty(x, y)) {
          continue;
        }
        ++count;
      }
    }
    CPPUNIT_ASSERT_EQUAL(empty_test_map.size(), count);
  }


  void testMoving()
  {
    namespace cg = cartograph;

    cg::vector_t min = empty_test_map.min_coords();
    cg::vector_t max = empty_test_map.max_coords();
    CPPUNIT_ASSERT_EQUAL(cg::vector_t(0, 0), min);
    CPPUNIT_ASSERT_EQUAL(cg::vector_t(50, 50), max);

    // Move 1, 1 to -1, -1. Afterwards, the boundary must have grown by one
    // in each direction.
    typename empty_test_map_t::node n = empty_test_map(1, 1);
    CPPUNIT_ASSERT_EQUAL(true, n.move_to(-1, -1));
    CPPUNIT_ASSERT_EQUAL(cg::vector_t(-1, -1), n.coordinates());

    min = empty_test_map.min_coords();
    max = empty_test_map.max_coords();
    CPPUNIT_ASSERT_EQUAL(cg::vector_t(-1, -1), min);
    CPPUNIT_ASSERT_EQUAL(cg::vector_t(50, 50), max);

    // Moving a node to an occupied position must fail
    CPPUNIT_ASSERT_EQUAL(false, n.move_to(2, 2));
    CPPUNIT_ASSERT_EQUAL(cg::vector_t(-1, -1), n.coordinates());

    // But moving it back to it's original location must succeed.
    CPPUNIT_ASSERT_EQUAL(true, n.move_to(1, 1));
    CPPUNIT_ASSERT_EQUAL(cg::vector_t(1, 1), n.coordinates());

    // And then the boundary must be as before.
    min = empty_test_map.min_coords();
    max = empty_test_map.max_coords();
    CPPUNIT_ASSERT_EQUAL(cg::vector_t(0, 0), min);
    CPPUNIT_ASSERT_EQUAL(cg::vector_t(50, 50), max);
  }


  void testErase()
  {
    namespace cg = cartograph;

    CPPUNIT_ASSERT_EQUAL(false, empty_test_map.is_empty(49, 49));
    CPPUNIT_ASSERT_EQUAL(true, empty_test_map.erase(49, 49));
    CPPUNIT_ASSERT_EQUAL(true, empty_test_map.is_empty(49, 49));
  }


};


CPPUNIT_TEST_SUITE_REGISTRATION(NodeGroupTest<cartograph::triangular_tile_traits>);
CPPUNIT_TEST_SUITE_REGISTRATION(NodeGroupTest<cartograph::rectangular_tile_traits>);
CPPUNIT_TEST_SUITE_REGISTRATION(NodeGroupTest<cartograph::hexagonal_tile_traits>);
