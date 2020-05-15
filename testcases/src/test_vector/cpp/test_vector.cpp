// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2011 FAU -- Joachim Falk <joachim.falk@fau.de>
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

#include <iostream>
#include <cassert>

#include <map>

#include <CoSupport/DataTypes/Vector.hpp>
#include <CoSupport/Streams/stl_output_for_vector.hpp>


typedef CoSupport::DataTypes::Vector<int> TVector;
typedef std::map<TVector, int>            TMap;

int main(int argc, char *argv[]) {
  TVector vector;
  vector.push_back(1);
  vector.push_back(2);
  std::cout << "vector: " << vector << ", vector.size(): " << vector.size() << std::endl;
  assert(vector.size() == 2 && vector[0] == 1 && vector[1] == 2);
  vector.insert(vector.begin(), 0);
  std::cout << "vector: " << vector << ", vector.size(): " << vector.size() << std::endl;
  assert(vector.size() == 3 && vector[0] == 0 && vector[1] == 1 && vector[2] == 2);
  vector.insert(vector.begin(), -1);
  std::cout << "vector: " << vector << ", vector.size(): " << vector.size() << std::endl;
  assert(vector.size() == 4 && vector[0] ==-1 && vector[1] == 0 && vector[2] == 1 && vector[3] == 2);
  vector.pop_back();
  std::cout << "vector: " << vector << ", vector.size(): " << vector.size() << std::endl;
  assert(vector.size() == 3 && vector[0] ==-1 && vector[1] == 0 && vector[2] == 1);
  vector.erase(--vector.end()-1);
  std::cout << "vector: " << vector << ", vector.size(): " << vector.size() << std::endl;
  assert(vector.size() == 2 && vector[0] ==-1 && vector[1] == 1);
  assert(vector.front() == -1);
  assert(vector.back() == 1);
  vector.insert(++vector.begin(), 2, 55);
  std::cout << "vector: " << vector << ", vector.size(): " << vector.size() << std::endl;
  assert(vector.size() == 4 && vector[0] ==-1 && vector[1] == 55 && vector[2] == 55 && vector[3] == 1);

  TMap map;
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
  map.insert(std::make_pair(vector, 13));
  return 0;
}
