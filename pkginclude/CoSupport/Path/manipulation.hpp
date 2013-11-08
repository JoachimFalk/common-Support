// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c) 2013-2013 Hardware-Software-CoDesign, University of
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

#ifndef _INCLUDED_COSUPPORT_PATH_MANIPULATION_HPP
#define _INCLUDED_COSUPPORT_PATH_MANIPULATION_HPP

#include <vector>
#include <boost/filesystem/path.hpp>

namespace CoSupport { namespace Path {

/// Compute difference between p1 and p2 and return the trailing difference of p1
/// \param[in] p1 First path
/// \param[in] p2 Second path
/// \return trailing Difference of \p p1 between \p p1 and \p p2
///
/// \par Example
/// pathTrailingDifferent("a/b/c/d", "a/b/e/f/g") == "c/d"<br>
/// pathTrailingDifferent("a/b/e/f/g", "a/b/c/d") == "e/f/g"
boost::filesystem::path pathTrailingDifferent(boost::filesystem::path const &p1, boost::filesystem::path const &p2);

/// Compute number of components in path
/// \param[in] p The path
/// \return number of components in path \p p
///
/// \par Example
/// numParts("/") == 1<br>
/// numParts("a/./b/") == 4<br>
/// numParts("a/./b") == 3<br>
/// numParts("//x/y/../z") == 5<br>
size_t numParts(boost::filesystem::path const &p);
                                        
/// Compute prefix with n parts of path p
/// \param[in] p The path
/// \param[in] n The requested number of parts in the prefix
/// \return Computed prefix
///
/// \par Example
/// prefix("a/./b/") == "a"
/// prefix("a/./b/", 2) == "a/."
/// prefix("/a/..//b/", 1) == "/"
/// prefix("/a/..//b/", 2) == "/a"
/// prefix("/a/..//b/", 3) == "/a/.."
/// prefix("/a/..//b/", 4) == "/a/../b"
/// prefix("/a/..//b/", 5) == "/a/../b/."
boost::filesystem::path prefix(boost::filesystem::path const &p, size_t n = 1);
                                        
/// Strip n parts from the beginning of the path
/// \param[in] p The path
/// \param[in] n The requested number of parts to strip
/// \return Stripped path
///
/// \par Example
/// stripPrefix("a/./b/") == "./b/."
/// stripPrefix("a/./b/", 2) == "b/."
/// stripPrefix("/a/..//b/", 0) == "/a/../b/."
/// stripPrefix("/a/..//b/", 1) == "a/../b/."
/// stripPrefix("/a/..//b/", 2) == "../b/."
/// stripPrefix("/a/..//b/", 3) == "b/."
/// stripPrefix("/a/..//b/", 4) == "."
/// stripPrefix("/a/..//b/", 5) == ""
boost::filesystem::path stripPrefix(boost::filesystem::path const &p, size_t n = 1);

boost::filesystem::path cleanup(boost::filesystem::path const &p);
boost::filesystem::path cleanup(boost::filesystem::path const &p, boost::filesystem::path const &cwd);

std::pair<bool, boost::filesystem::path> pathTrailingGraft(
    boost::filesystem::path p, boost::filesystem::path const &g, std::vector<boost::filesystem::path> const &concat);

} } // namespace CoSupport::Path

#endif //_INCLUDED_COSUPPORT_PATH_MANIPULATION_HPP
