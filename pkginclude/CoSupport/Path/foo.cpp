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

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <boost/scoped_array.hpp>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace CoSupport { namespace Path {

namespace fs = boost::filesystem;

void GenericGeneratorContext::setExecutableLocationImpl(
    const char *bindir, const char *builddir, const char *argv0)
{
  fs::path fsBin(bindir);
  fs::path fsBld(builddir);
  fs::path fsPrg(argv0);
#ifndef NDEBUG  
  assert(fsBin.has_root_path());
  for (fs::path::iterator iter = fsBin.begin();
       iter != fsBin.end();
       ++iter)
    assert(*iter != "..");
  assert(fsBld.has_root_path());
  for (fs::path::iterator iter = fsBld.begin();
       iter != fsBld.end();
       ++iter)
    assert(*iter != "..");
#endif //NDEBUG
  bool found = false;
  
  if (!exists(fsPrg) && strchr(argv0, '/') == NULL) {
    const char *pathEnv = getenv("PATH");
    if (pathEnv != NULL) {
      // ugh => next try search in path
      char *saveptr = NULL;
      std::string path(pathEnv);
      for (char *pathElem = strtok_r(&path[0], ":", &saveptr);
           pathElem != NULL;
           pathElem = strtok_r(NULL, ":", &saveptr)) {
        fsPrg = fs::path(pathElem) / argv0;
        if (exists(fsPrg))
          { found = true; break; }
      }
    }
  } else
    found = true;
  if (!found) {
    std::ostringstream msg;
    msg << "argv[0] \"" << argv0 << "\" fucked up can't find myself!";
    throw std::runtime_error(msg.str().c_str());
  }
  if (!fsPrg.has_root_path()) {
    // make it absolute
    size_t                    buf = 32;
    boost::scoped_array<char> cwd(new char[buf]);
    
    while (getcwd(cwd.get(), buf) == NULL) {
      if (errno != ERANGE) {
        std::ostringstream msg;
        msg << "Can't get current working directory: " << strerror(errno) << "!";
        throw std::runtime_error(msg.str().c_str());
      }
      cwd.reset(new char[buf *= 2]);
    }
    fsPrg = cwd.get() / fsPrg;
  }
//outDbgFSRes << "myself:   " << fsPrg << std::endl;
//outDbgFSRes << "bindir:   " << fsBin << std::endl;
//outDbgFSRes << "builddir: " << fsBld << std::endl;
  
  // Handle magic .libs/lt-xxxx stuff
  {
    fs::path::iterator iter = fsPrg.end();
    if (iter != fsPrg.begin()) {
      --iter;
#if BOOST_FILESYSTEM_VERSION > 2
      if (iter->native().length() >= 3 && iter->native().substr(0,3) == "lt-") {
#else 
      if (iter->length() >= 3 && iter->substr(0,3) == "lt-") {
#endif
        if (iter != fsPrg.begin()) {
#if BOOST_FILESYSTEM_VERSION > 2
          std::string prgNameNoLt = iter->native().substr(3, std::string::npos);
#else 
          std::string prgNameNoLt = iter->substr(3, std::string::npos);
#endif
          --iter;
          if (*iter == ".libs") {
            // ok fsPrg is of type .../.libs/lt-prgNameNoLt => change this to .../prgNameNoLt
            fs::path fsPrgNew = fsPrg.parent_path().parent_path() / prgNameNoLt;
            if (exists(fsPrgNew))
              fsPrg = fsPrgNew;
          }
        }
      }
    }
  }
  
  fsPrg = cleanup(fsPrg);
  
//if (outDbgFSRes.isVisible(Debug::High)) {
//  outDbgFSRes << "myself:   " << fsPrg << std::endl;
//  outDbgFSRes << "bindir:   " << fsBin << std::endl;
//  outDbgFSRes << "builddir: " << fsBld << std::endl;
//}
  
  fs::path reqBin;
  fs::path reqBld;
  for (ResourceLocMap::iterator iter = resourceLocMap.begin();
       iter != resourceLocMap.end();
       ++iter) {
    fs::path tmp = pathTrailingDifferent(fsBin, iter->second.installLoc);
//  outDbgFSRes << "pathTrailingDifferent('" << fsBin << "', '" << iter->second.installLoc << "') => '" << tmp << "'" << std::endl;
    if (tmp.string().length() > reqBin.string().length())
      reqBin = tmp;
    tmp = pathTrailingDifferent(fsBld, iter->second.buildLoc);
//  outDbgFSRes << "pathTrailingDifferent('" << fsBld << "', '" << iter->second.buildLoc << "') => '" << tmp << "'" << std::endl;
    if (tmp.string().length() > reqBld.string().length())
      reqBld = tmp;
  }
//if (outDbgFSRes.isVisible(Debug::High)) {
//  outDbgFSRes << "reqBin: " << reqBin << std::endl;
//  outDbgFSRes << "reqBld: " << reqBld << std::endl;
//}
  
  fsPrg = fsPrg.parent_path();
  
  std::vector<fs::path> concatBin, concatBld;
  stripBin = numParts(fsBin) - numParts(reqBin);
  stripBld = numParts(fsBld) - numParts(reqBld);
  for (ResourceLocMap::iterator iter = resourceLocMap.begin();
       iter != resourceLocMap.end();
       ++iter) {
    concatBin.push_back(stripHead(iter->second.installLoc, stripBin));
    concatBld.push_back(stripHead(iter->second.buildLoc, stripBld));
  }
  
//if (outDbgFSRes.isVisible(Debug::High)) {
//  outDbgFSRes << "concatBin: " << concatBin << std::endl;
//  outDbgFSRes << "concatBld: " << concatBld << std::endl;
//}
  
  graftBin = pathTrailingGraft(fsPrg, reqBin, concatBin);
  graftBld = pathTrailingGraft(fsPrg, reqBld, concatBld);
  
//if (outDbgFSRes.isVisible(Debug::High)) {
//  outDbgFSRes << "graftBin: " << graftBin << std::endl;
//  outDbgFSRes << "graftBld: " << graftBld << std::endl;
//}
  
  if (!graftBin.first && !graftBld.first) {
    throw std::runtime_error("Can't detect valid resource locations!");
  }
  
  if (graftBin.first) {
    for (ResourceLocMap::iterator iter = resourceLocMap.begin();
         iter != resourceLocMap.end();
         ++iter)
      iter->second.resolved = graftBin.second / stripHead(iter->second.installLoc, stripBin);
  } else {
    for (ResourceLocMap::iterator iter = resourceLocMap.begin();
         iter != resourceLocMap.end();
         ++iter)
      iter->second.resolved = graftBld.second / stripHead(iter->second.buildLoc, stripBld);
  }
}

void GenericGeneratorContext::registerResourceLocation(const std::string &key,
    const std::string &installLoc, const std::string &buildLoc)
{
//if (outDbgFSRes.isVisible(Debug::Low)) {
//  outDbgFSRes << "New resource with key " << key << " at\n"
//                 "  install location   " << installLoc << " or\n"
//                 "  build dir location " << buildLoc << " registered." << std::endl;
//}
  std::pair<ResourceLocMap::iterator, bool> status =
    resourceLocMap.insert(std::make_pair(key, Resource()));
  if (!status.second)
    throw Exception::DuplicateResourceKey(key);
  status.first->second.installLoc = installLoc;
  status.first->second.buildLoc   = buildLoc;
  if (graftBin.first) {
    status.first->second.resolved = graftBin.second / stripHead(installLoc, stripBin);
  } else if (graftBld.first) {
    status.first->second.resolved = graftBld.second / stripHead(buildLoc, stripBld);
  }
}

fs::path GenericGeneratorContext::retrieveResourceLocation(const std::string &key) const
{
  ResourceLocMap::const_iterator iter =
    resourceLocMap.find(key);
//if (outDbgFSRes.isVisible(Debug::Medium)) {
//  if (iter == resourceLocMap.end())
//    outDbgFSRes << "Requested resource with key " << key << " not found!" << std::endl;
//  else
//    outDbgFSRes << "Requested resource with key " << key << " found at " << iter->second.resolved << "." << std::endl;
//}
  if (iter == resourceLocMap.end())
    throw Exception::MissingResourceKey(key);
  return iter->second.resolved;
}

} } // namespace CoSupport::Path
