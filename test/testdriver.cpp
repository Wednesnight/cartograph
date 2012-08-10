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

#include <iostream>
#include <stdexcept>

#include <cartograph/cg-config.h>
#include <cartograph/version.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextTestProgressListener.h>

#if defined(CG_FHTAGN_VERSION)
#include <fhtagn/util/cppunit_output.h>
#endif

int main(int argc, char **argv)
{
    std::cout << cartograph::copyright_string << std::endl;

    std::string testPath = (argc > 1) ? std::string(argv[1]) : "";

    // Create the event manager and test controller
    CppUnit::TestResult controller;


#if defined(CG_FHTAGN_VERSION)
    // Listener printing verbose test information
    fhtagn::util::VerboseOutput verbose(std::cout);
    controller.addListener(&verbose);
#else
    // Brief progress indicator, standard.
    CppUnit::TextTestProgressListener brief;
    controller.addListener(&brief);
#endif

    // Add a listener that colllects test result
    CppUnit::TestResultCollector result;
    controller.addListener(&result);


    // Add the top suite to the test runner
    CppUnit::TestRunner runner;
    runner.addTest(CppUnit::TestFactoryRegistry::getRegistry().makeTest());
    try {
        if (testPath.empty()) {
            runner.run(controller);
        } else {
            runner.run(controller, testPath);
        }

        // Print test in a compiler compatible format.
        CppUnit::CompilerOutputter outputter(&result,std::cerr);
        outputter.write();
    } catch (std::invalid_argument const & e) {
        // Test path not resolved
        std::cerr << std::endl <<  "ERROR: " << e.what() << std::endl;
        return 2;
    }

    return result.wasSuccessful() ? 0 : 1;
}
