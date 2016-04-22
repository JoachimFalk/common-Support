/*
 * Copyright (c) 2004-2009 Hardware-Software-CoDesign, University of
 * Erlangen-Nuremberg. All rights reserved.
 * 
 *   This library is free software; you can redistribute it and/or modify it under
 *   the terms of the GNU Lesser General Public License as published by the Free
 *   Software Foundation; either version 2 of the License, or (at your option) any
 *   later version.
 * 
 *   This library is distributed in the hope that it will be useful, but WITHOUT
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *   FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 *   details.
 * 
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with this library; if not, write to the Free Software Foundation, Inc.,
 *   59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 * 
 * --- This software and any associated documentation is provided "as is" 
 * 
 * IN NO EVENT SHALL HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG
 * BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN
 * NUREMBERG HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF ERLANGEN NUREMBERG, SPECIFICALLY
 * DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE PROVIDED
 * HEREUNDER IS ON AN "AS IS" BASIS, AND HARDWARE-SOFTWARE-CODESIGN, UNIVERSITY OF
 * ERLANGEN NUREMBERG HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 * ENHANCEMENTS, OR MODIFICATIONS.
 */

#include <CoSupport/Path/manipulation.hpp>
#include <CoSupport/Path/resourcelocations.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <iostream>

using namespace CoSupport::Path;
namespace fs = boost::filesystem;

int main(int argc, char *argv[]) {

  std::cout << getExecutableLocation(argv[0]) << std::endl;

  {
    fs::path result = pathTrailingDifferent("a/b/c/d", "a/b/e/f/g");
    std::cout << "pathTrailingDifferent(\"a/b/c/d\", \"a/b/e/f/g\"): " << result << std::endl;
    assert(result == "c/d");
  }
  {
    fs::path result = pathTrailingDifferent("a/b/e/f/g", "a/b/c/d");
    std::cout << "pathTrailingDifferent(\"a/b/e/f/g\", \"a/b/c/d\"): " << result << std::endl;
    assert(result == "e/f/g");
  }
  {
    fs::path result = pathTrailingDifferent("a/./b/e/f/g", "a/b/c/d");
    std::cout << "pathTrailingDifferent(\"a/./b/e/f/g\", \"a/b/c/d\"): " << result << std::endl;
    assert(result == "./b/e/f/g");
  }
  {
    fs::path result = pathTrailingDifferent("a/b/c/d", "a/b/c/d");
    std::cout << "pathTrailingDifferent(\"a/b/c/d\", \"a/b/c/d\"): " << result << std::endl;
    assert(result == "");
  }
  {
    fs::path result = pathLeadingDifferent("d/c/b/a", "g/f/e/b/a");
    std::cout << "pathLeadingDifferent(\"d/c/b/a\", \"g/f/e/b/a\"): " << result << std::endl;
    assert(result == "d/c");
  }
  {
    fs::path result = pathLeadingDifferent("g/f/e/b/a", "d/c/b/a");
    std::cout << "pathLeadingDifferent(\"g/f/e/b/a\", \"d/c/b/a\"): " << result << std::endl;
    assert(result == "g/f/e");
  }
  {
    fs::path result = pathLeadingDifferent("a/b/c/d", "a/b/c/d");
    std::cout << "pathLeadingDifferent(\"a/b/c/d\", \"a/b/c/d\"): " << result << std::endl;
    assert(result == "");
  }
  {
    size_t result = numParts("/");
    std::cout << "numParts(\"/\"): " << result << std::endl;
    assert(result == 1);
  }
  {
    size_t result = numParts("a/./b/");
    std::cout << "numParts(\"a/./b/\"): " << result << std::endl;
    assert(result == 4);
  }
  {
    size_t result = numParts("a/./b");
    std::cout << "numParts(\"a/./b\"): " << result << std::endl;
    assert(result == 3);
  }
  {
    size_t result = numParts("//x/y/../z");
    std::cout << "numParts(\"//x/y/../z\"): " << result << std::endl;
    assert(result == 5);
  }
  {
    fs::path result = prefix("a/./b/");
    std::cout << "prefix(\"a/./b/\"): " << result << std::endl;
    assert(result == "a");
  }
  {
    fs::path result = prefix("a/./b/", 2);
    std::cout << "prefix(\"a/./b/\", 2): " << result << std::endl;
    assert(result == "a/.");
  }
  {
    fs::path result = prefix("/a/..//b/", 1);
    std::cout << "prefix(\"/a/..//b/\", 1): " << result << std::endl;
    assert(result == "/");
  }
  {
    fs::path result = prefix("/a/..//b/", 2);
    std::cout << "prefix(\"/a/..//b/\", 2): " << result << std::endl;
    assert(result == "/a");
  }
  {
    fs::path result = prefix("/a/..//b/", 3);
    std::cout << "prefix(\"/a/..//b/\", 3): " << result << std::endl;
    assert(result == "/a/..");
  }
  {
    fs::path result = prefix("/a/..//b/", 4);
    std::cout << "prefix(\"/a/..//b/\", 4): " << result << std::endl;
    assert(result == "/a/../b");
  }
  {
    fs::path result = prefix("/a/..//b/", 5);
    std::cout << "prefix(\"/a/..//b/\", 5): " << result << std::endl;
    assert(result == "/a/../b/");
  }
  {
    fs::path result = stripPrefix("a/./b/");
    std::cout << "stripPrefix(\"a/./b/\"): " << result << std::endl;
    assert(result == "./b/");
  }
  {
    fs::path result = stripPrefix("a/./b/", 2);
    std::cout << "stripPrefix(\"a/./b/\", 2): " << result << std::endl;
    assert(result == "b/");
  }
  {
    fs::path result = stripPrefix("/a/..//b/", 0);
    std::cout << "stripPrefix(\"/a/..//b/\", 0): " << result << std::endl;
    assert(result == "/a/../b/");
  }
  {
    fs::path result = stripPrefix("/a/..//b/", 1);
    std::cout << "stripPrefix(\"/a/..//b/\", 1): " << result << std::endl;
    assert(result == "a/../b/");
  }
  {
    fs::path result = stripPrefix("/a/..//b/", 2);
    std::cout << "stripPrefix(\"/a/..//b/\", 2): " << result << std::endl;
    assert(result == "../b/");
  }
  {
    fs::path result = stripPrefix("/a/..//b/", 3);
    std::cout << "stripPrefix(\"/a/..//b/\", 3): " << result << std::endl;
    assert(result == "b/");
  }
  {
    fs::path result = stripPrefix("/a/..//b/", 4);
    std::cout << "stripPrefix(\"/a/..//b/\", 4): " << result << std::endl;
    assert(result == ".");
  }
  {
    fs::path result = stripPrefix("/a/..//b/", 5);
    std::cout << "stripPrefix(\"/a/..//b/\", 5): " << result << std::endl;
    assert(result == "");
  }
  {
    // Setup temporary filesystem structure to test cleanup functionality
    fs::path tmpDir = fs::temp_directory_path() / fs::unique_path("%%%%-%%%%-%%%%-%%%%");
    std::cout << "tmpDir: " << tmpDir << std::endl;
    fs::create_directories(tmpDir/"a/b/c");
    fs::create_directories(tmpDir/"d");
    fs::create_symlink("a/b", tmpDir/"e");
    // Now do test of cleanup functionality
    {
      fs::path result = cleanup("/../../.."/tmpDir);
      std::cout << "cleanup(" << "/../../.."/tmpDir << "): " << result << std::endl;
    }
    {
      fs::path result = cleanup(tmpDir/"a/.//b/..");
      std::cout << "cleanup(" << tmpDir/"a/.//b/.." << "): " << result << std::endl;
    }
    {
      fs::path result = cleanup("a/.//b/..", tmpDir);
      std::cout << "cleanup(\"a/.//b/..\", " << tmpDir << "): " << result << std::endl;
    }
    {
      fs::path result = cleanup(".//b/../../d", tmpDir/"a");
      std::cout << "cleanup(\".//b/../../d\", " << tmpDir/"a" << "): " << result << std::endl;
    }
    {
      fs::path result = cleanup("c/../c/../../../d", tmpDir/"a/b");
      std::cout << "cleanup(\"c/../c/../../d\", " << tmpDir/"a/b" << "): " << result << std::endl;
    }
    {
      fs::path result = cleanup("e/../../d", tmpDir);
      std::cout << "cleanup(\"e/../../d\", " << tmpDir << "): " << result << std::endl;
    }
    // Remove temporary filesystem structure to test cleanup functionality
    fs::remove_all(tmpDir);
  }

/// pathTrailingDifferent("a/b/e/f/g", "a/b/c/d") == "e/f/g"
  return 0;
}
