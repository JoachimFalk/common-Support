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
#include <CoSupport/Streams/stl_output_for_vector.hpp>

//#include <CoSupport/Math/Tuple/POVector.hpp>

#include <map>
#include <vector>

typedef CoSupport::DataTypes::Projection<std::vector<std::string> &, std::vector<size_t> const &> TVector;

int main(int argc, char *argv[]) {
  {
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

    assert(vector[0] == "d");
    assert(vector[1] == "a");
    assert(vector[2] == "c");
    assert(vector[3] == "b");
  }
  {
    std::vector<std::string> v1;
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
/* This is no longer supported for normal vector projections, only PO<vector> projections
    proj(v1,v2) = "flummy";
    assert(v1[0] == "a");
    assert(v1[1] == "flummy");
    assert(v1[2] == "flummy");
    assert(v1[3] == "d");
 */
  }
  {
    std::vector<size_t> projIdx;
    projIdx.push_back(2);
    projIdx.push_back(1);
    std::vector<size_t> flummy;
    flummy.push_back(77);
    flummy.push_back(13);
  
    std::vector<int> t1vector1(4);
    t1vector1[0] = 24;
    t1vector1[1] = 35;
    t1vector1[2] = 11;
    t1vector1[3] = -19;

    std::cout << "proj(" << t1vector1 << ", " << projIdx << "): " << proj(t1vector1, projIdx) << std::endl;
    proj(t1vector1, projIdx) = flummy;
    std::cout << "proj(" << t1vector1 << ", " << projIdx << "): " << proj(t1vector1, projIdx) << std::endl;
    assert(t1vector1[2] == 77 && t1vector1[1] == 13);
  
    std::vector<int> idx;
    idx.push_back(3);
    idx.push_back(2);
    idx.push_back(1);
    idx.push_back(0);

    std::vector<unsigned long long> t2vector1;
    t2vector1 = proj(t1vector1, static_cast<std::vector<int> const &>(idx));
    t2vector1 = proj(static_cast<std::vector<int> const &>(t1vector1), idx);
    t2vector1 = proj(static_cast<std::vector<int> const &>(t1vector1),
                     static_cast<std::vector<int> const &>(idx));

    assert(std::vector<int>(proj(proj(t1vector1, idx), idx)) == t1vector1 && "proj(proj(t1vector1, idx), idx) == t1vector1");
  }
  return 0;
}
