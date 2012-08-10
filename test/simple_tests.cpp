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

#include <cppunit/extensions/HelperMacros.h>

#include <cartograph/types.h>
#include <cartograph/error.h>

class SimpleTest
  : public CppUnit::TestFixture
{
public:
  CPPUNIT_TEST_SUITE(SimpleTest);

    CPPUNIT_TEST(testError);
    CPPUNIT_TEST(testVector);

  CPPUNIT_TEST_SUITE_END();
private:

  void testError()
  {
    namespace cg = cartograph;

    // Error code serialization
    cg::error_t err = cg::CG_OK;
    std::stringstream s1;
    s1 << err;
    CPPUNIT_ASSERT_EQUAL(std::string("CG_OK"), s1.str());

    // Exceptions.
    cg::exception ex(cg::CG_OK);
    CPPUNIT_ASSERT(ex.what() != 0);
    CPPUNIT_ASSERT_EQUAL(std::string("No error"), std::string(ex.what()));

    CPPUNIT_ASSERT(ex == cg::CG_OK);

    std::stringstream s2;
    s2 << ex;
    CPPUNIT_ASSERT_EQUAL(std::string("<CG_OK:No error>"), s2.str());
  }


  void testVector()
  {
    namespace cg = cartograph;

    // Default is invalid
    cg::vector_t v;
    CPPUNIT_ASSERT_EQUAL(v, cg::invalid_vector);

    // Test equality
    cg::vector_t v1(0, 1);
    cg::vector_t v2(0, 1);
    CPPUNIT_ASSERT_EQUAL(v1, v2);

    // Test distance function
    CPPUNIT_ASSERT_EQUAL(v1.distance(), v2.distance());
    cg::vector_t v3(1, 0);
    CPPUNIT_ASSERT_EQUAL(v1.distance(), v3.distance());

    cg::vector_t v4(1, 1);
    CPPUNIT_ASSERT(v1.distance() < v4.distance());

    // Test < operator - it's actually comparing distances.
    CPPUNIT_ASSERT(v1 < v4);

    // Test != operator
    CPPUNIT_ASSERT(v1 != v4);

    // Minus and plus operators.
    cg::vector_t sum = v1 + v4;
    CPPUNIT_ASSERT_EQUAL(cg::unit_t(1), sum.m_x);
    CPPUNIT_ASSERT_EQUAL(cg::unit_t(2), sum.m_y);

    cg::vector_t diff = v1 - v4;
    CPPUNIT_ASSERT_EQUAL(cg::unit_t(-1), diff.m_x);
    CPPUNIT_ASSERT_EQUAL(cg::unit_t(0), diff.m_y);
  }
};


CPPUNIT_TEST_SUITE_REGISTRATION(SimpleTest);
