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

#include <CoSupport/Path/manipulation.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <boost/scoped_array.hpp>

#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace CoSupport { namespace Path {

namespace fs = boost::filesystem;

boost::filesystem::path pathTrailingDifferent(boost::filesystem::path const &p1, boost::filesystem::path const &p2) {
  boost::filesystem::path::iterator iter, jter;
  
  for (iter = p1.begin(), jter = p2.begin();
       iter != p1.end() && jter != p2.end();
       ++iter, ++jter) {
    if (*iter != *jter)
      break;
  }
  boost::filesystem::path retval;
  for (; iter != p1.end(); ++iter)
    retval /= *iter;
  return retval;
}

boost::filesystem::path pathLeadingDifferent(boost::filesystem::path const &p1, boost::filesystem::path const &p2) {
  boost::filesystem::path::iterator iter, jter;
  
  for (iter = p1.end(), jter = p2.end();
       iter != p1.begin() && jter != p2.begin();
       ) {
    --iter; --jter;
    if (*iter != *jter) {
      ++iter;
      break;
    }
  }
  boost::filesystem::path retval;
  for (; iter != p1.begin(); ) {
    --iter;
    retval = *iter / retval;
  }
  return retval;
}

size_t numParts(boost::filesystem::path const &p) {
  size_t numParts = 0;
  for (boost::filesystem::path::const_iterator iter = p.begin();
       iter != p.end();
       ++iter)
    ++numParts;
  return numParts;
}

boost::filesystem::path prefix(boost::filesystem::path const &p, size_t n) {
  boost::filesystem::path retval;
  for (boost::filesystem::path::const_iterator iter = p.begin(); n > 0 && iter != p.end(); --n)
    retval /= *iter++;
  return retval;
}

boost::filesystem::path stripPrefix(boost::filesystem::path const &p, size_t n) {
  boost::filesystem::path retval;
  boost::filesystem::path::const_iterator iter = p.begin();
  for (; n > 0 && iter != p.end(); --n)
    ++iter;
  for (; iter != p.end(); ++iter)
    retval /= *iter;
  return retval;
}

static
boost::filesystem::path cleanup(boost::filesystem::path p, boost::filesystem::path const *cwd) {
  struct stat st1, st2;
  
  // Filter out ".." and "." from p.
  if (stat((p.is_absolute() ? p : *cwd/p).c_str(), &st1) != 0) {
    std::ostringstream msg;
    msg << "Can't stat " << p << ": " << strerror(errno) << "!";
    throw std::runtime_error(msg.str().c_str());
  }
  fs::path pBase = p.is_absolute() ? p.root_path() : *cwd;
  fs::path pCleaned;
  for (fs::path::iterator iter = p.begin();
       iter != p.end();
       ) {
//  std::cout << "pCleaned: " << pCleaned << std::endl;
//  std::cout << "pCleaned.filename(): " << pCleaned.filename() << std::endl;
//  std::cout << "considering path segment " << *iter << std::endl;
    if (*iter == "." ||
        (*iter == ".." && pCleaned.filename() == pBase.root_directory())) {
      ++iter;
    } else if (*iter != ".." ||
               pCleaned.filename() == ".." ||
               pCleaned.empty()) {
      pCleaned /= *iter; ++iter;
    } else {
      ++iter;
      fs::path tryClean = pBase/pCleaned.parent_path();
      for (fs::path::iterator jter = iter;
           jter != p.end();
           ++jter)
        tryClean /= *jter;
//    std::cout << "considering " << tryClean << std::endl;
      if (stat(tryClean.string().c_str(), &st2) == 0 &&
          st1.st_ino == st2.st_ino &&
          st1.st_dev == st2.st_dev) {
        // success
        pCleaned = pCleaned.parent_path();
//      std::cout << "now at " << pCleaned << std::endl;
      } else {
        // break out the heavy guns we need symlink analysis
        int                       length, buf = 32;
        boost::scoped_array<char> lnk(new char[buf]);
        
        while ((length = readlink((pBase/pCleaned).string().c_str(), lnk.get(), buf)) >= buf) {
//        std::cout << "length: " << length << " buf: " << buf << std::endl;
          lnk.reset(new char[buf *= 2]);
        }
        if (length < 0) {
          std::ostringstream msg;
          msg << "Can't readlink " << pBase/pCleaned << ": " << strerror(errno) << "!";
          throw std::runtime_error(msg.str().c_str());
        }
        lnk[length] = '\0';
        pCleaned.remove_filename();
        pCleaned /= lnk.get();
        for (fs::path::iterator jter = --iter;
             jter != p.end();
             ++jter)
          pCleaned /= *jter;
        // start over
        p        = pCleaned;
        pBase    = p.is_absolute() ? p.root_path() : *cwd;
        pCleaned = "";
        iter      = p.begin();
//      std::cout << "start over with: " << p << std::endl;
      }
    }
  }
  return pCleaned;
}

boost::filesystem::path cleanup(
    boost::filesystem::path const &p,
    boost::filesystem::path const &cwd)
  { return cleanup(p, &cwd); }

boost::filesystem::path cleanup(
    boost::filesystem::path const &p) {
  if (!p.is_absolute()) {
    fs::path cwd = fs::current_path();
    return cleanup(p, &cwd);
  } else {
    return cleanup(p, NULL);
  }
}

std::pair<bool, boost::filesystem::path> pathTrailingGraft(
    boost::filesystem::path p, boost::filesystem::path const &g, std::vector<boost::filesystem::path> const &concat)
{
  std::pair<bool, boost::filesystem::path> retval;
  retval.first  = false;
  retval.second = p;
  
  struct stat st1, st2;
  
  if (stat(p.string().c_str(), &st1) != 0) {
    std::ostringstream msg;
    msg << "Can't stat " << p << ": " << strerror(errno) << "!";
    throw std::runtime_error(msg.str().c_str());
  }
  do {
    boost::filesystem::path graft = (retval.second = p) / g;
    if (stat(graft.string().c_str(), &st2) == 0 &&
        st1.st_ino == st2.st_ino &&
        st1.st_dev == st2.st_dev) {
      retval.first = true;
      for (std::vector<boost::filesystem::path>::const_iterator iter = concat.begin();
           iter != concat.end();
           ++iter) {
        if (!exists(p / *iter))
          { retval.first = false; break; }
      }
      if (retval.first)
        return retval;
    }
    p = retval.second.parent_path();
  } while (p != retval.second);
  return retval;
}

} } // namespace CoSupport::Path
