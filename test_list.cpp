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

#include <CoSupport/DataTypes/List.hpp>

typedef CoSupport::DataTypes::List<int> TList;
typedef std::map<TList, int>            TMap;

int main(int argc, char *argv[]) {
  TList list;
  TMap  map;

  list.push_back(1);
  list.push_back(2);
  list.push_front(0);
  list.push_front(-1);

  std::cout << list.size() << std::endl;

  map.insert(std::make_pair(list, 13));
  for(TList::iterator iter = list.begin();
      iter != list.end();
      ++iter) {
    if (iter != list.begin())
      std::cout << ", ";
    std::cout << *iter;
    int v = *iter;
    iter = list.insert(list.erase(iter), 2*v);
    map.insert(std::make_pair(list, 13));
  }
  std::cout << std::endl;
  for(TList::const_iterator iter = list.begin();
      iter != list.end();
      ++iter) {
    if (iter != list.begin())
      std::cout << ", ";
    std::cout << *iter;
  }
  std::cout << std::endl;
  list.erase(list.begin(), list.end());
  assert(list.empty());
  map.insert(std::make_pair(list, 33));
  return 0;
}
