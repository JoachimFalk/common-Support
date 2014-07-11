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
#include <stdint.h>

#include <map>

#include <CoSupport/Math/Tuple/POVector.hpp>
#include <CoSupport/Streams/stl_output_for_vector.hpp>

typedef CoSupport::Math::Tuple::POVector<int>::type                 T1Vector;
typedef CoSupport::Math::Tuple::POVector<unsigned long long>::type  T2Vector;
typedef std::map<T1Vector, int>                                     T1Map;
typedef std::map<T2Vector, int>                                     T2Map;

int main(int argc, char *argv[]) {
  // Testing vector interface of T1Vector
  {
    T1Vector t1vector1;
    t1vector1.push_back(1);
    t1vector1.push_back(2);
    std::cout << "t1vector1: " << t1vector1 << ", t1vector1.size(): " << t1vector1.size() << std::endl;
    assert(t1vector1.size() == 2 && t1vector1[0] ==  1 && t1vector1[1] == 2);
    t1vector1.insert(t1vector1.begin(), 0);
    std::cout << "t1vector1: " << t1vector1 << ", t1vector1.size(): " << t1vector1.size() << std::endl;
    assert(t1vector1.size() == 3 && t1vector1[0] ==  0 && t1vector1[1] == 1 && t1vector1[2] == 2);
    t1vector1.insert(t1vector1.begin(), 33);
    std::cout << "t1vector1: " << t1vector1 << ", t1vector1.size(): " << t1vector1.size() << std::endl;
    assert(t1vector1.size() == 4 && t1vector1[0] == 33 && t1vector1[1] == 0 && t1vector1[2] == 1 && t1vector1[3] == 2);
    t1vector1.pop_back();
    std::cout << "t1vector1: " << t1vector1 << ", t1vector1.size(): " << t1vector1.size() << std::endl;
    assert(t1vector1.size() == 3 && t1vector1[0] == 33 && t1vector1[1] == 0 && t1vector1[2] == 1);
    t1vector1.erase(--t1vector1.end()-1);
    std::cout << "t1vector1: " << t1vector1 << ", t1vector1.size(): " << t1vector1.size() << std::endl;
    assert(t1vector1.size() == 2 && t1vector1[0] == 33 && t1vector1[1] == 1);
    assert(t1vector1.front() == 33);
    assert(t1vector1.back() == 1);
    t1vector1.insert(++t1vector1.begin(), 2, 55);
    std::cout << "t1vector1: " << t1vector1 << ", t1vector1.size(): " << t1vector1.size() << std::endl;
    assert(t1vector1.size() == 4 && t1vector1[0] ==33 && t1vector1[1] == 55 && t1vector1[2] == 55 && t1vector1[3] == 1);
  }
  // Testing mod and div of PO vectors
  {
    struct {
      int i[4], j[4], k;
    } testData[] = {
      { { 33, 55, 55,  1}, {  1,  2,  3,  4},  0 },
      { { 33, 55, 55, 13}, {  1,  2,  3,  4},  3 },
      { { -1, -5,  0,-13}, {  5, 10, 20, 11}, -2 },
      { { 13, -5,  0, -9}, {  5, 10, 20, 11}, -1 },
      { { 13,  1, 21, 12}, {  5, 10, 20, 11},  0 },
      { { 13, 33, 21, 12}, {  5, 10, 20, 11},  1 },
      { { 13, 33, 77, 25}, {  5, 10, 20, 11},  2 },
      { { 13, 33, 77, 25}, { -5, 10, 20, 11}, -3 },
      { { 13, 33, 77, 25}, { -5, 10,-20, 11}, -4 },
      { { 13, 33, 77, 25}, { -5,-10,-20,-11}, -4 },
      { { -7, 11, 33,-27}, { -3, -7,  6,  5}, -6 },
      { { -7, 11, 33, -1}, { -3, -7,  6,  5}, -2 },
      { { -7,-11, 33, 11}, { -3, -7,  6,  5},  1 },
      { { -7,-21, 33, 11}, { -3, -7,  6,  5},  2 },
    };

    for (size_t n = 0; n < sizeof(testData)/sizeof(testData[0]); ++n) {
      T1Vector i(4), j(4);
      for (size_t x = 0; x < i.size(); ++x) {
        i[x] = testData[n].i[x];
        j[x] = testData[n].j[x];
      }
      int      k = div(i, j);
      T1Vector m = mod(i, j);
      std::cout << "div(" << i << ", " << j << "): " << k << std::endl;
      std::cout << "mod(" << i << ", " << j << "): " << m << std::endl;
      assert(k == testData[n].k);
      assert(m == i-j*k);
      {
        bool errorZero  = false;
        bool errorBound = true;
        for (size_t x = 0; x < i.size(); ++x) {
          if (!((j[x] > 0 && m[x] >= 0) || (j[x] < 0 && m[x] <= 0)))
            errorZero = true;
          if ((j[x] > 0 && m[x] < j[x]) || (j[x] < 0 && m[x] > j[x]))
            errorBound = false;
        }
        assert(!errorZero); assert(!errorBound);
      }
      {
        m = i-j*(k-1);
        bool errorZero  = false;
        bool errorBound = true;
        for (size_t x = 0; x < i.size(); ++x) {
          if (!((j[x] > 0 && m[x] >= 0) || (j[x] < 0 && m[x] <= 0)))
            errorZero = true;
          if ((j[x] > 0 && m[x] < j[x]) || (j[x] < 0 && m[x] > j[x]))
            errorBound = false;
        }
        assert(!errorZero); assert(errorBound);
      }
      {
        m = i-j*(k+1);
        bool errorZero  = false;
        bool errorBound = true;
        for (size_t x = 0; x < i.size(); ++x) {
          if (!((j[x] > 0 && m[x] >= 0) || (j[x] < 0 && m[x] <= 0)))
            errorZero = true;
          if ((j[x] > 0 && m[x] < j[x]) || (j[x] < 0 && m[x] > j[x]))
            errorBound = false;
        }
        assert(errorZero); assert(!errorBound);
      }
    }
  }
  // Testing normal operations on PO vectors
  {
    T1Vector t1vector1(4);
    t1vector1[0] = 33;
    t1vector1[1] = 55;
    t1vector1[2] = 55;
    t1vector1[3] = 1;

    T1Vector t1vector2(4);
    t1vector2[0] = 1;
    t1vector2[1] = 2;
    t1vector2[2] = 3;
    t1vector2[3] = 4;

    std::cout << "supremum(" << t1vector1 << ", " << t1vector2 << "): " << supremum(t1vector1, t1vector2) << std::endl;
    std::cout << "infimum(" << t1vector1 << ", " << t1vector2 << "): " << infimum(t1vector1, t1vector2) << std::endl;

    std::cout << "supremum(" << t1vector1 << ", " << 19 << "): " << supremum(t1vector1, 19) << std::endl;
    std::cout << "infimum(" << t1vector1 << ", " << 19 << "): " << infimum(t1vector1, 19) << std::endl;
  
    std::cout << "supremum(" << 3 << ", " << t1vector2 << "): " << supremum(3, t1vector2) << std::endl;
    std::cout << "infimum(" << 3 << ", " << t1vector2 << "): " << infimum(3, t1vector2) << std::endl;
  
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
  }
  // Testing vector projections
  {
    std::vector<size_t> projIdx;
    projIdx.push_back(2);
    projIdx.push_back(1);
    CoSupport::Math::Tuple::POVector<size_t>::type flummy;
    flummy.push_back(77);
    flummy.push_back(13);
  
    T1Vector t1vector1(4);
    t1vector1[0] = 24;
    t1vector1[1] = 35;
    t1vector1[2] = 11;
    t1vector1[3] = -19;

    std::cout << "proj(" << t1vector1 << ", " << projIdx << "): " << proj(t1vector1, projIdx) << std::endl;
    proj(t1vector1, projIdx) = flummy;
    std::cout << "proj(" << t1vector1 << ", " << projIdx << "): " << proj(t1vector1, projIdx) << std::endl;
    assert(t1vector1[2] == 77 && t1vector1[1] == 13);
    proj(t1vector1, projIdx) = 0;
    std::cout << "proj(" << t1vector1 << ", " << projIdx << "): " << proj(t1vector1, projIdx) << std::endl;
    assert(t1vector1[2] == 0 && t1vector1[1] == 0);
  
    std::vector<int> idx;
    idx.push_back(3);
    idx.push_back(2);
    idx.push_back(1);
    idx.push_back(0);

    T2Vector t2vector1 = proj(t1vector1, idx);
    t2vector1 = proj(t1vector1, static_cast<std::vector<int> const &>(idx));
    t2vector1 = proj(static_cast<T1Vector const &>(t1vector1), idx);
    t2vector1 = proj(static_cast<T1Vector const &>(t1vector1),
                     static_cast<std::vector<int> const &>(idx));

    assert(proj(proj(t1vector1, idx), idx) == t1vector1 && "proj(proj(t1vector1, idx), idx) == t1vector1");

    CoSupport::Math::Tuple::POVector<std::string>::type v1;
    v1.push_back("a");
    v1.push_back("b");
    v1.push_back("c");
    v1.push_back("d");
    std::vector<size_t> v2;
    v2.push_back(2);
    v2.push_back(1);
    std::vector<std::string> v3;
    v3.push_back("foo");
    v3.push_back("bar");

    proj(v1,v2) = v3;
    assert(v1[0] == "a");
    assert(v1[1] == "bar");
    assert(v1[2] == "foo");
    assert(v1[3] == "d");

    proj(v1,v2) = "flummy";
    assert(v1[0] == "a");
    assert(v1[1] == "flummy");
    assert(v1[2] == "flummy");
    assert(v1[3] == "d");
  }
  {
    T1Vector t1vector1;
    for (int i = 0; i <= 7; ++i)
      t1vector1.push_back((static_cast<uint64_t>(i+42)*2147483647) % 3571);
    T1Map t1map;
//  t1map.insert(std::make_pair(t1vector1, 13));
    for(T1Vector::iterator iter = t1vector1.begin();
        iter != t1vector1.end();
        ++iter) {
      if (iter != t1vector1.begin())
        std::cout << ", ";
      std::cout << *iter;
      int v = *iter;
      iter = t1vector1.insert(t1vector1.erase(iter), 2*v);
      t1map.insert(std::make_pair(t1vector1, 13));
      *iter = *iter / 3;
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
    for (T1Map::const_iterator iter = t1map.begin();
         iter != t1map.end();
         ++iter) {
      for (T1Map::const_iterator jter = t1map.begin();
           jter != t1map.end();
           ++jter) {
        std::cout << iter->first << " <= " << jter->first << " " << (iter->first <= jter->first) << std::endl;
        assert((iter->first <= jter->first) == (iter == jter));

        std::cout << iter->first << " >= " << jter->first << " " << (iter->first >= jter->first) << std::endl;
        assert((iter->first >= jter->first) == (iter == jter));

        std::cout << iter->first << " <  " << jter->first << " " << (iter->first <  jter->first) << std::endl;
        assert((iter->first <  jter->first) == false);

        std::cout << iter->first << " >  " << jter->first << " " << (iter->first >  jter->first) << std::endl;
        assert((iter->first >  jter->first) == false);

        std::cout << iter->first << " == " << jter->first << " " << (iter->first == jter->first) << std::endl;
        assert((iter->first == jter->first) == (iter == jter));

        std::cout << iter->first << " != " << jter->first << " " << (iter->first != jter->first) << std::endl;
        assert((iter->first != jter->first) == (iter != jter));
      }
    }
    t1vector1.erase(t1vector1.begin(), t1vector1.end());
    assert(t1vector1.empty());
    t1map.insert(std::make_pair(t1vector1, 13));
    for (T1Map::const_iterator iter = t1map.begin();
         iter != t1map.end();
         ++iter) {
      std::cout << iter->first << std::endl;
    }
  }
  return 0;
}
