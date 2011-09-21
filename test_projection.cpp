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

#include <CoSupport/DataTypes/Projection.hpp>

#include <map>
#include <vector>

typedef CoSupport::DataTypes::Projection<std::vector<std::string> &, std::vector<size_t> const &> TVector;
typedef std::map<TVector, int>            TMap;

int main(int argc, char *argv[]) {
  std::vector<std::string> v1;
  v1.push_back("a");
  v1.push_back("b");
  v1.push_back("c");
  v1.push_back("d");
  std::vector<size_t> v2;
  v2.push_back(3);
  v2.push_back(0);
  v2.push_back(2);
  v2.push_back(1);
  TVector vector = proj(v1, v2);
  
  std::cout << vector.size() << std::endl;
  
  for(TVector::const_iterator iter = vector.begin();
      iter != vector.end();
      ++iter) {
    if (iter != vector.begin())
      std::cout << ", ";
    std::cout << *iter;
  }
  std::cout << std::endl;


/*TVector vector;
  TMap    map;

  vector.push_back(1);
  vector.push_back(2);
  vector.insert(vector.begin(), 0);
  vector.insert(vector.begin(), -1);

  std::cout << vector.size() << std::endl;

  map.insert(std::make_pair(vector, 13));
  for(TVector::iterator iter = vector.begin();
      iter != vector.end();
      ++iter) {
    if (iter != vector.begin())
      std::cout << ", ";
    std::cout << *iter;
    int v = *iter;
    iter = vector.insert(vector.erase(iter), 2*v);
    map.insert(std::make_pair(vector, 13));
  }
  std::cout << std::endl;
  for(TVector::const_iterator iter = vector.begin();
      iter != vector.end();
      ++iter) {
    if (iter != vector.begin())
      std::cout << ", ";
    std::cout << *iter;
  }
  std::cout << std::endl;
  vector.erase(vector.begin(), vector.end());
  assert(vector.empty());
  map.insert(std::make_pair(vector, 13)); */
  return 0;
}
