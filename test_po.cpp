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

#include <CoSupport/Math/Tuple/POVector.hpp>

typedef CoSupport::Math::Tuple::POVector<int>::type                 T1Vector;
typedef CoSupport::Math::Tuple::POVector<unsigned long long>::type  T2Vector;
typedef std::map<T1Vector, int>                                     T1Map;
typedef std::map<T2Vector, int>                                     T2Map;

int main(int argc, char *argv[]) {
  T1Vector t1vector1;
  t1vector1.push_back(1);
  t1vector1.push_back(2);
  std::cout << "t1vector1: " << t1vector1 << ", t1vector1.size(): " << t1vector1.size() << std::endl;
  assert(t1vector1.size() == 2 && t1vector1[0] == 1 && t1vector1[1] == 2);
  t1vector1.insert(t1vector1.begin(), 0);
  std::cout << "t1vector1: " << t1vector1 << ", t1vector1.size(): " << t1vector1.size() << std::endl;
  assert(t1vector1.size() == 3 && t1vector1[0] == 0 && t1vector1[1] == 1 && t1vector1[2] == 2);
  t1vector1.insert(t1vector1.begin(), 33);
  std::cout << "t1vector1: " << t1vector1 << ", t1vector1.size(): " << t1vector1.size() << std::endl;
  assert(t1vector1.size() == 4 && t1vector1[0] ==33 && t1vector1[1] == 0 && t1vector1[2] == 1 && t1vector1[3] == 2);
  t1vector1.pop_back();
  std::cout << "t1vector1: " << t1vector1 << ", t1vector1.size(): " << t1vector1.size() << std::endl;
  assert(t1vector1.size() == 3 && t1vector1[0] ==33 && t1vector1[1] == 0 && t1vector1[2] == 1);
  t1vector1.erase(--t1vector1.end()-1);
  std::cout << "t1vector1: " << t1vector1 << ", t1vector1.size(): " << t1vector1.size() << std::endl;
  assert(t1vector1.size() == 2 && t1vector1[0] ==33 && t1vector1[1] == 1);
  assert(t1vector1.front() == 33);
  assert(t1vector1.back() == 1);
  t1vector1.insert(++t1vector1.begin(), 2, 55);
  std::cout << "t1vector1: " << t1vector1 << ", t1vector1.size(): " << t1vector1.size() << std::endl;
  assert(t1vector1.size() == 4 && t1vector1[0] ==33 && t1vector1[1] == 55 && t1vector1[2] == 55 && t1vector1[3] == 1);
  
  T1Vector t1vector2(4);
  t1vector2[0] = 1;
  t1vector2[1] = 2;
  t1vector2[2] = 3;
  t1vector2[3] = 4;

  std::cout << "div(" << t1vector1 << ", " << t1vector2 << "): " << div(t1vector1, t1vector2) << std::endl;
  assert(div(t1vector1, t1vector2) == 0);
  std::cout << "mod(" << t1vector1 << ", " << t1vector2 << "): " << mod(t1vector1, t1vector2) << std::endl;
  assert(mod(t1vector1, t1vector2) == t1vector1);
  t1vector1.pop_back(); t1vector1.push_back(13);
  std::cout << "div(" << t1vector1 << ", " << t1vector2 << "): " << div(t1vector1, t1vector2) << std::endl;
  assert(div(t1vector1, t1vector2) == 3);
  std::cout << "mod(" << t1vector1 << ", " << t1vector2 << "): " << mod(t1vector1, t1vector2) << std::endl;
  assert(mod(t1vector1, t1vector2) == t1vector1 - 3*t1vector2);
  
  std::cout << supremum(t1vector1, t1vector2) << std::endl;
  std::cout << infimum(t1vector1, t1vector2) << std::endl;
  
  T1Vector t1vector3 = t1vector1 + t1vector2;
  T1Vector t1vector4 = t1vector3 - t1vector1;
  
  assert(t1vector4 == t1vector2 && "t1vector4 == t1vector2");
  t1vector3 -= t1vector1;
  assert(t1vector3 == t1vector2 && "t1vector3 == t1vector2");
  t1vector3 += 13;
  
  assert(t1vector3 >= 13 && "t1vector3 >= 13");
  assert(13 <= t1vector3 && "13 <= t1vector3");
  assert(t1vector3 >  13 && "t1vector3 >  13");
  assert(13 <  t1vector3 && "13 <  t1vector3");
  
  assert(t1vector1+t1vector1 == 2*t1vector1 && "t1vector1+t1vector1 = 2*t1vector1");
  assert(t1vector1+t1vector1 == t1vector1*2 && "t1vector1+t1vector1 = t1vector1*2");
  
  t1vector2 = t1vector1;
  t1vector2 *= 4;
  assert(t1vector2/2 == t1vector1*2 && "t1vector2/2 == t1vector1*2");
  t1vector2 /= 2;
  assert(t1vector2 == t1vector1*2 && "t1vector2 == t1vector1*2");
  
  std::cout << t1vector1.size() << std::endl;

  t1vector3 = -t1vector1;
  assert(t1vector3 + t1vector1 == 0);
  
  std::vector<int> idx;
  idx.push_back(3);
  idx.push_back(2);
  idx.push_back(1);
  idx.push_back(0);
  
  t1vector4 = proj(t1vector1, idx);
  t1vector4 = proj(t1vector1, static_cast<std::vector<int> const &>(idx));
  t1vector4 = proj(static_cast<T1Vector const &>(t1vector1), idx);
  t1vector4 = proj(static_cast<T1Vector const &>(t1vector1),
                   static_cast<std::vector<int> const &>(idx));
  
  assert(proj(proj(t1vector1, idx), idx) == t1vector1 && "proj(proj(t1vector1, idx), idx) == t1vector1");
  
  T1Map t1map;
  t1map.insert(std::make_pair(t1vector1, 13));
  for(T1Vector::iterator iter = t1vector1.begin();
      iter != t1vector1.end();
      ++iter) {
    if (iter != t1vector1.begin())
      std::cout << ", ";
    std::cout << *iter;
    int v = *iter;
    iter = t1vector1.insert(t1vector1.erase(iter), 2*v);
    t1map.insert(std::make_pair(t1vector1, 13));
  }
  std::cout << std::endl;
  for(T1Vector::const_iterator iter = t1vector1.begin();
      iter != t1vector1.end();
      ++iter) {
    if (iter != t1vector1.begin())
      std::cout << ", ";
    std::cout << *iter;
  }
  std::cout << std::endl;
  t1vector1.erase(t1vector1.begin(), t1vector1.end());
  assert(t1vector1.empty());
  t1map.insert(std::make_pair(t1vector1, 13));
  for (T1Map::const_iterator iter = t1map.begin();
       iter != t1map.end();
       ++iter) {
    std::cout << iter->first << std::endl;
  }
  return 0;
}
