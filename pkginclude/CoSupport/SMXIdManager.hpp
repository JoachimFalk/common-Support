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

#ifndef INCLUDED_COSUPPORT_SMX_ID_MANAGER
#define INCLUDED_COSUPPORT_SMX_ID_MANAGER

#include "XML/xerces_support.hpp"
#include "String/convert.hpp"
#include "Math/string_hash.hpp"

#include "commondefs.h"

#include <set>
#include <map>
#include <limits>

#include <sysc/kernel/sc_object.h>

namespace CoSupport {

/// @brief Identifier for elements
typedef uint32_t SMXId;

/**
 * @brief Helper struct for (de)serializing Ids
 */
struct COSUPPORT_ATTRIBUTE_DEPRECATED SMXIdSer {
  /// @brief Constructor
  SMXIdSer(SMXId id);

  /// @brief Returns Id
  operator SMXId() const;

  /// @brief The Id
  SMXId id;
};

namespace String {

/// @brief Converts Id to string
template<>
std::string asStr<SMXIdSer>(const SMXIdSer& id);

/// @brief Converts string to Id
template<>
SMXIdSer strAs<SMXIdSer>(const std::string &s);

} // namespace String

/// @brief Output operator for SMXIdSer
std::ostream& operator<<(std::ostream& out, const SMXIdSer& id);

class COSUPPORT_ATTRIBUTE_DEPRECATED SMXIdManager {
public:
  /// @brief Returns single instance of SMXIdManager
  static SMXIdManager& getInstance();

  /// @brief Add node with autogenerated Id
  SMXId addNode(XML::Xerces::XN::DOMNode *n);

  /// @brief Add node with Id
  void addNode(XML::Xerces::XN::DOMNode *n, SMXId id);

  /// @brief Delete node
  void delNode(XML::Xerces::XN::DOMNode* n);

  /// @brief Delete node with the specified id
  void delNode(SMXId id);
  
  /// @brief Lookup node by Id
  XML::Xerces::XN::DOMNode* getNode(SMXId id) const;

  /// @brief Lookup Id by node
  SMXId getId(XML::Xerces::XN::DOMNode* n) const;

  /// @brief Add reference to Id
  SMXId addNRef(XML::Xerces::XN::DOMAttr* n);

  /// @brief Add reference to Id
  void addNRef(XML::Xerces::XN::DOMAttr* n, SMXId id);

  /// @brief Delete reference
  void delNRef(XML::Xerces::XN::DOMAttr* n);

  /// @brief Delete reference of the specified Id
  void delNRef(SMXId id, XML::Xerces::XN::DOMAttr* n);
  
  /// @brief Nodes which reference an Id
  typedef std::set<XML::Xerces::XN::DOMAttr*> NRef;

  /// @brief Lookup referencing nodes by Id
  const NRef* getNRef(SMXId id) const;
  
  /// @brief Convenience typedef
  typedef sc_core::sc_object SCObj;

  /// @brief Add object with autogenerated Id
  SMXId addObj(SCObj* obj, size_t index = 0);

  /// @brief Add object with Id
  void addObj(SCObj* obj, SMXId id, size_t index);

  /// @brief Delete object
  void delObj(const SCObj* obj);

  /// @brief Delete object with the Id of the specified index
  void delObj(const SCObj* obj, size_t index);

  /// @brief Delete object with the specified Id
  void delObj(SMXId id);

  /// @brief Lookup object by Id
  SCObj* getObj(SMXId id) const;

  /// @brief Lookup Id by object
  SMXId getId(const SCObj* obj, size_t index = 0) const;

  /// @brief Add anonymous object
  SMXId addAnon();
  
  /// @brief Analyze node (recursive)
  void analyze(XML::Xerces::XN::DOMNode *n);

  void anonToNamed();

private:
  
  /// @brief Invisible default constructor
  SMXIdManager();

  /// @brief Disabled copy constructor
  SMXIdManager(const SMXIdManager&);

  /// @brief Disabled assign operator
  SMXIdManager& operator=(const SMXIdManager&);
  
  /// @brief Number of bits in Id type (>= 2)
  static const size_t bits = std::numeric_limits<SMXId>::digits;

  /// @brief Offset for anonynmous Ids
  const SMXId offAnon;

  /// @brief Offset for named Ids
  const SMXId offName;
  
  /// @brief Hash function used for generating Ids for named objects
  FNV<bits - 1> hashName;

  /// @brief Value type of IdMap
  struct IdMapEntry {
    /// @brief Node defining the Id
    XML::Xerces::XN::DOMNode* node;
    
    /// @brief Attribute nodes referencing the Id
    NRef nref;

    /// @brief Corresponding SystemC object
    SCObj* obj;

    /// @brief Supports multiple Ids for same object
    size_t index;

    /// @brief Default constructor
    IdMapEntry();
  };

  /// @brief Maps a specific Id to its node
  typedef std::map<SMXId, IdMapEntry> IdMap;

  /// @brief Id -> object lookup map
  IdMap idMap;

  /// @brief Calculates a new anonymous Id
  SMXId calcAnonId() const;

  /// @brief Calculates a new named Id for object
  SMXId calcNameIdObj(const char* name) const;
  
  /// @brief Calculates a new named Id for node
  SMXId calcNameIdNode(const XMLCh* name) const;
};

} // namespace CoSupport

#endif // INCLUDED_COSUPPORT_SMX_ID_MANAGER
