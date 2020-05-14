// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2013 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2016 FAU -- Rafael Rosales <rafael.rosales@fau.de>
 *   2018 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <CoSupport/compatibility-glue/nullptr.h>

#include <CoSupport/Path/resourcelocations.hpp>
#include <CoSupport/Path/manipulation.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <boost/scoped_array.hpp>

#include <sstream>

#ifdef _MSC_VER
# include <windows.h>
#endif //_MSC_VER

namespace CoSupport { namespace Path {

namespace fs = boost::filesystem;

boost::filesystem::path getExecutableLocation(const char *argv0) {
#ifdef _MSC_VER
  TCHAR ownPath[MAX_PATH];
  GetModuleFileName(GetModuleHandle(NULL), ownPath, (sizeof(ownPath))); 
  return fs::path(ownPath);
#else //!defined(_MSC_VER)
  fs::path fsPrg(argv0);
  bool found = false;
  
  if (!exists(fsPrg) && strchr(argv0, '/') == nullptr) {
    const char *pathEnv = getenv("PATH");
    if (pathEnv != nullptr) {
      // ugh => next try search in path
      char *saveptr = nullptr;
      std::string path(pathEnv);
      for (char *pathElem = strtok_r(&path[0], ":", &saveptr);
           pathElem != nullptr;
           pathElem = strtok_r(nullptr, ":", &saveptr)) {
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
    
    while (getcwd(cwd.get(), buf) == nullptr) {
      if (errno != ERANGE) {
        std::ostringstream msg;
        msg << "Can't get current working directory: " << strerror(errno) << "!";
        throw std::runtime_error(msg.str().c_str());
      }
      cwd.reset(new char[buf *= 2]);
    }
    fsPrg = cwd.get() / fsPrg;
  }
  
  {
    fs::path::iterator iter = fsPrg.end();
    if (iter != fsPrg.begin()) {
      --iter;
#if BOOST_FILESYSTEM_VERSION > 2
      std::string prgName = iter->native();
#else 
      std::string prgName = *iter;
#endif
      if (iter != fsPrg.begin()) {
        --iter;
        if (*iter == ".libs") {
          fs::path fsPrgNew = fsPrg.parent_path().parent_path() / 
            ((prgName.length() >= 3 && prgName.substr(0,3) == "lt-")
              // fsPrg is of type .../.libs/lt-prgNameNoLt => change this to .../prgNameNoLt
              ? prgName.substr(3, std::string::npos)
              // fsPrg is of type .../.libs/prgName => change this to .../prgName
              : prgName);
          if (exists(fsPrgNew))
            fsPrg = fsPrgNew;
        }
      }
    }
  }
  fsPrg = cleanup(fsPrg);
  return fsPrg;
#endif //!defined(_MSC_VER)
}

/*
void GenericGeneratorContext::setExecutableLocationImpl(
    const char *bindir, const char *builddir, const char *argv0)
{
  fs::path fsBin(bindir);
  fs::path fsBld(builddir);
  fs::path fsPrg(getExecutableLocation(argv0));
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
    concatBin.push_back(stripPrefix(iter->second.installLoc, stripBin));
    concatBld.push_back(stripPrefix(iter->second.buildLoc, stripBld));
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
      iter->second.resolved = graftBin.second / stripPrefix(iter->second.installLoc, stripBin);
  } else {
    for (ResourceLocMap::iterator iter = resourceLocMap.begin();
         iter != resourceLocMap.end();
         ++iter)
      iter->second.resolved = graftBld.second / stripPrefix(iter->second.buildLoc, stripBld);
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
    status.first->second.resolved = graftBin.second / stripPrefix(installLoc, stripBin);
  } else if (graftBld.first) {
    status.first->second.resolved = graftBld.second / stripPrefix(buildLoc, stripBld);
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
*/

} } // namespace CoSupport::Path
