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

#ifndef CG_NODE_GROUP_H
#define CG_NODE_GROUP_H

#include <map>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include <cartograph/types.h>
#include <cartograph/tile_traits.h>

#ifndef CG_DISABLE_CONCEPT_CHECKS
// Include concepts
#include <cartograph/detail/node_group_concepts.h>
#endif

namespace cartograph {

/**
 * Default id generator, see node_group below.
 **/
struct simple_id_generator
{
  typedef unit_t node_id_t;

  simple_id_generator()
    : m_current()
  {
  }


  node_id_t get_unique_id()
  {
    node_id_t ret = m_current;
    ++m_current;
    return ret;
  }


  void reset()
  {
    m_current = node_id_t();
  }


  node_id_t m_current;
};




/**
 * The node_group class is the primary data structure of cartograph. It defines
 * a collection of tiles and their spatial relationship to each other. For
 * details on some of the assumptions on which this code is based, read the
 * doc/design.txt file in this source distribution.
 *
 * In brief, a node_group may be viewed as a game map or segment of a game map,
 * anchored on an origin node at coordinate (0, 0). Other tiles in the map are
 * positioned relative to this origin node.
 *
 * The node_group class distinguishes between the user-specified node data, and
 * the traits each tile has based on it's shape - hexagonal tiles fit together
 * differently from triangular tiles, and therefore neighbourship between two
 * tiles must be viewed differently.
 *
 * But node_group tries to present an abstract view of these relationships, such
 * that it becomes possible to write algorithms that are agnostic to the shape
 * of tiles.
 *
 * The embedded node class represents each position in the node group - whether
 * or not actual tile data is associated with it, is up to the user.
 **/
template <
  // The data type used to store user-specific information for each node. It's
  // entirely opaque to the node_group class.
  typename node_dataT,
  // Defines traits based on the shape of tiles. Predefined trait classes are
  // located in cartograph/tile_traits.h, and include triangular_tile_traits,
  // rectangular_tile_traits and hexagonal_tile_traits.
  typename tile_traitsT,
  // ID generator, defaulting to the simple_id_generator above. Must conform to
  // the IDGeneratorConcept in node_group_concepts.
  typename id_generatorT = simple_id_generator
>
class node_group
{
#ifndef CG_DISABLE_CONCEPT_CHECKS
  // Concept checks
  BOOST_CLASS_REQUIRE(tile_traitsT, concepts, TileTraitsConcept);
  BOOST_CLASS_REQUIRE(id_generatorT, concepts, IDGeneratorConcept);
#endif

public:
  // Convenience typedef
  typedef boost::shared_ptr<node_dataT> node_data_ptr;
  typedef node_dataT                    node_data_t;
  typedef tile_traitsT                  tile_traits_t;

  // Within the node_group, each tile not only has a unique position, but also
  // a unique id, which allows a node to retain it's identity when being
  // relocated.
  typedef typename id_generatorT::node_id_t node_id_t;

  /**
   * The node class represents each position in the node_group. Technically, a
   * node instance is only a facade for a node_group-internal data structure,
   * which means that all node instances that refer to the same node id
   * reference the exact same node, not copies of each other.
   **/
  class node
  {
  public:
    /**
     * There is no (public) constructor other than the copy constructor.
     **/
    node(node const & other);

    /**
     * Destructor
     **/
    ~node();

    /**
     * @returns the node id of the node this instance references.
     **/
    node_id_t id() const;

    /**
     * @returns the coordinates (relative to the node_group's origin) of the
     *    node this instance references.
     **/
    vector_t coordinates() const;

    /**
     * Dereferencing the node instance gives access to the user-defined node
     * data.
     **/
    node_dataT const * operator->() const;
    node_dataT * operator->();

    node_dataT const * get() const;
    node_dataT * get();

    /**
     * Not all nodes must have user-defined node data associated with them - a
     * node might also represent an empty position in a node_group. This
     * operator returns true if the node points to actual data, and false
     * otherwise.
     **/
    operator bool() const;

    /**
     * Assigning user-defined data to the node instance will overwrite any
     * user-defined data recorded in the node_group for the node.
     **/
    node & operator=(node_dataT const & other);

    /**
     * Returns a DIR_END terminated array of directions in which the node has
     * neighbouring nodes. Directions are sorted in clock-wise order, with the
     * northernmost direction first (12 o'clock).
     *
     * @param join_type Nodes may touch upon other nodes by edge or by corners.
     *    Depending on the application, only some types of neighbours may be of
     *    interest.
     **/
    directions_t const * const available_dirs(join_t join_type
        = PATHFINDING_DEFAULT) const;

    /**
     * Returns the node that's relative to the current node in the given
     * direction.
     *
     * @param dir Direction to look for neighbours.
     *
     * @throws CG_INVALID_DIR if any direction not returned by available_dirs()
     *    above is specified.
     **/
    node get_relative(directions_t const & dir) const;

    /**
     * Move the node from it's current location in the node_group to the new
     * location. The node cannot be moved to an occupied position.
     *
     * @param coords New location to move the node to.
     *
     * @return true if successful, false on failures.
     **/
    bool move_to(unit_t const & x, unit_t const & y);
    bool move_to(vector_t const & coords);

  private:
    friend class node_group;

    node(node_group * group, node_id_t const & id,
        vector_t const & coords);

    // Owning node_group
    node_group *  m_group;
    // Id this node represents.
    node_id_t     m_id;
    // Coordinates of this node.
    vector_t      m_coords;
  };


  /**
   * Constructor - either is passed an id_generatorT that it copy-constructs
   * from, or default-constructs an id_generatorT internally.
   **/
  node_group();
  explicit node_group(id_generatorT const & gen);

  /**
   * Returns a node instance representing the given position relative to the
   * origin. If a node instance for the specified position does not exist, it
   * is created, making use of the unique identifier generator.
   *
   * @param (x, y), coords Cordinates of the node, either as individual x and
   *    y coordinates, or a vector_t.
   *
   * @throws CG_INVALID_COORDS if the provided coordinates are invalid according
   *            to the tile traits
   **/
  node operator()(unit_t const & x, unit_t const & y) const;
  node operator()(vector_t const & coords) const;

  /**
   * Returns true if the specified coordinates represent a valid position in
   * the group according to the tile traits, else false.
   **/
  bool is_valid(unit_t const & x, unit_t const & y) const;
  bool is_valid(vector_t const & coords) const;

  /**
   * Returns true if the node_group does not have node data stored for the given
   * position, false otherwise. Use this if you want to check for a node's
   * presence without inadvertently creating them.
   **/
  bool is_empty(unit_t const & x, unit_t const & y) const;
  bool is_empty(vector_t const & coords) const;

  /**
   * min_coords() returns a vector_t with the lowest m_x of any node in the
   * group, and the lowest m_y of any node in the group.
   *
   * max_coords() does pretty much the same but with the highest m_x/m_y of any
   * node in the group. The major difference is that it actually returns m_x/m_y
   * that are one higher than the maximums it found, which allows you to treat
   * the values the same as the return value of end() in STL containers.
   *
   * Together the two pairs of coordinates define a bounding box for all nodes
   * in the group, and thus allow for iteration over all nodes.
   *
   * Note though that there doesn't actually have to be a node in the group at
   * either position. It pays to check each position via is_empty() before
   * attempting to retrieve the corresponding node from the map.
   **/
  vector_t min_coords() const;
  vector_t max_coords() const;

  /**
   * Clears all map data.
   **/
  void clear();

  /**
   * Returns the number of nodes in the node_group.
   **/
  size_t size() const;

  /**
   * Move a node from one position to another in the node_group.
   *
   * @return true on success, false on failure.
   **/
  bool move(vector_t const & from, vector_t const & to);

  /**
   * Erase a node from the node_group. Any existing node instances for the given
   * node will be invalidated.
   *
   * @return true on success, false if the node was not found.
   **/
  bool erase(unit_t const & x, unit_t const & y);
  bool erase(vector_t const & coords);
private:

  // Retreive an user-defined node data for the given node id.
  node_data_ptr get(node_id_t const & id) const;

  // Set user defined node-data for the given node id. Also anchor the node at
  // the specified position in the group.
  // XXX This is fairly heavy-handed and will overwrite existing data. For now
  //     it looks entirely sufficient, but this function may require revisiting.
  void set(node_id_t const & id, vector_t const & coords,
      node_dataT const & data);

  // Map coordinates to node ids.
  typedef std::map<vector_t, node_id_t> coordinate_map_t;
  coordinate_map_t m_nodes;

  // Map node ids to user-defined node data.
  struct node_data_wrapper
  {
    vector_t  m_coords;
    node_data_ptr m_data;
  };
  typedef std::map<node_id_t, node_data_wrapper> node_data_map_t;
  node_data_map_t m_node_data;

  // Id generator
  mutable boost::scoped_ptr<id_generatorT> m_id_generator;
};

} // namespace cartograph


// Include implementation
#include <cartograph/detail/node_group.tcc>

#endif // guard
