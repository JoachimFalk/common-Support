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

#include <iostream>
#include <cassert>

#include <map>

#include <CoSupport/Streams/stl_output_for_set.hpp>
#include <CoSupport/DataTypes/Set.hpp>

typedef CoSupport::DataTypes::Set<int> TSet;
typedef std::map<TSet, int>            TSetInMap;

int main(int argc, char *argv[]) {
  TSet set;
  set.insert(3);
  set.insert(1);
  std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
  assert(set.size() == 2);
  assert(*set.begin() == 1);
  assert(*++set.begin() == 3);
  assert(*--set.end() == 3);

  set.insert(2);
  std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
  assert(set.size() == 3);
  assert(*++set.begin() == 2);

  set.erase(1);
  std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
  assert(set.size() == 2);
  assert(*set.begin() == 2);
  assert(*++set.begin() == 3);

  set.erase(--set.end());
  std::cout << "set: " << set << ", set.size(): " << set.size() << std::endl;
  assert(set.size() == 1);
  assert(*set.begin() == 2);

  TSetInMap  map;
  map.insert(std::make_pair(set, 13));
  for(TSet::iterator iter = set.begin();
      iter != set.end();
      ++iter) {
    if (iter != set.begin())
      std::cout << ", ";
    std::cout << *iter;
    int v = *iter;
    set.erase(iter);
    iter = set.insert(2*v).first;
    map.insert(std::make_pair(set, 13));
  }
  std::cout << std::endl;
  for(TSet::const_iterator iter = set.begin();
      iter != set.end();
      ++iter) {
    if (iter != set.begin())
      std::cout << ", ";
    std::cout << *iter;
  }
  std::cout << std::endl;
  set.erase(set.begin(), set.end());
  assert(set.empty());
  map.insert(std::make_pair(set, 33));
  return 0;
}