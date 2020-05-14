// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2020 FAU -- Joachim Falk <joachim.falk@fau.de>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) any
 * later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#ifndef _INCLUDED_COSUPPORT_PATH_MANIPULATION_HPP
#define _INCLUDED_COSUPPORT_PATH_MANIPULATION_HPP

#include <vector>
#include <boost/filesystem/path.hpp>

#include "export_config.h"

namespace CoSupport { namespace Path {

/// Compute difference between p1 and p2 and return the trailing difference of p1
/// \param[in] p1 First path
/// \param[in] p2 Second path
/// \return trailing Difference of \p p1 between \p p1 and \p p2
///
/// \par Example
/// pathTrailingDifferent("a/b/c/d", "a/b/e/f/g") == "c/d"<br>
/// pathTrailingDifferent("a/b/e/f/g", "a/b/c/d") == "e/f/g"
COSUPPORT_PATH_API
boost::filesystem::path pathTrailingDifferent(boost::filesystem::path const &p1, boost::filesystem::path const &p2);

/// Compute difference between p1 and p2 and return the leading difference of p1
/// \param[in] p1 First path
/// \param[in] p2 Second path
/// \return trailing Difference of \p p1 between \p p1 and \p p2
///
/// \par Example
/// pathLeadingDifferent("d/c/b/a", "g/f/e/b/a") == "d/c"<br>
/// pathLeadingDifferent("g/f/e/b/a", "d/c/b/a") == "g/f/e"
COSUPPORT_PATH_API
boost::filesystem::path pathLeadingDifferent(boost::filesystem::path const &p1, boost::filesystem::path const &p2);

/// Compute number of components in path
/// \param[in] p The path
/// \return number of components in path \p p
///
/// \par Example
/// numParts("/") == 1<br>
/// numParts("a/./b/") == 4<br>
/// numParts("a/./b") == 3<br>
/// numParts("//x/y/../z") == 5<br>
COSUPPORT_PATH_API
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
COSUPPORT_PATH_API
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
COSUPPORT_PATH_API
boost::filesystem::path stripPrefix(boost::filesystem::path const &p, size_t n = 1);

COSUPPORT_PATH_API
boost::filesystem::path cleanup(boost::filesystem::path const &p);

COSUPPORT_PATH_API
boost::filesystem::path cleanup(boost::filesystem::path const &p, boost::filesystem::path const &cwd);

COSUPPORT_PATH_API
std::pair<bool, boost::filesystem::path> pathTrailingGraft(
    boost::filesystem::path p, boost::filesystem::path const &g, std::vector<boost::filesystem::path> const &concat);

} } // namespace CoSupport::Path

#endif //_INCLUDED_COSUPPORT_PATH_MANIPULATION_HPP
