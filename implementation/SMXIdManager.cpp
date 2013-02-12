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

#include <CoSupport/SMXIdManager.hpp>
#include <cassert>
#include <sstream>

namespace CoSupport {

namespace String {

  template<>
  std::string asStr<SMXIdSer>(const SMXIdSer& id)
    { std::ostringstream out; out << id; return out.str(); }

  template<>
  SMXIdSer strAs<SMXIdSer>(const std::string &s) {
    if (s.size() < 3 || s[0] != 'i' || s[1] != 'd')
      throw InvalidConversionTo<SMXIdSer>();
    
    SMXId ret = 0;
    
    for (size_t i = 2; i < s.size(); ++i) {
      if (s[i] < '0' || s[i] > '9')
        throw InvalidConversionTo<SMXIdSer>();
      ret = ret * 10 + (s[i] - '0');
    }
    
    return ret;
  }

} // namespace String

namespace X = XML::Xerces;
namespace XXN = XML::Xerces::XN;

using String::asStr;
using String::strAs;

using String::InvalidConversion;

SMXIdSer::SMXIdSer(SMXId id) :
  id(id)
  {}

SMXIdSer::operator SMXId() const
  { return id; }

std::ostream& operator<<(std::ostream& out, const SMXIdSer& id)
  { return out << "id" << id.id; }

SMXIdManager::IdMapEntry::IdMapEntry() :
  node(0),
  obj(0),
  index(0)
  {}

SMXIdManager::SMXIdManager() :
  offAnon(0 << (bits - 1)),
  offName(1 << (bits - 1))
  {}

SMXIdManager& SMXIdManager::getInstance() {
  static SMXIdManager instance;
  return instance;
}

SMXId SMXIdManager::addNode(XXN::DOMNode *n) {
  SMXId id = addAnon(); // TODO: node might have name
  X::setAttrValueFrom(n, "id", CoSupport::asStr<SMXIdSer>(id));
  addNode(n, id);
  return id;
}

void SMXIdManager::addNode(XXN::DOMNode *n, SMXId id) {
  assert(n->getNodeType() == XXN::DOMNode::ELEMENT_NODE);
  
  IdMapEntry &entry = idMap[id];
  
  if (entry.node) {
    assert(entry.node == n);
  } else {
    //assert(entry.nref.empty()); <- see addNRef
    entry.node = n;
  }
}

SMXId SMXIdManager::addNRef(XXN::DOMAttr* n) {
  SMXId id = X::getNodeValueAs<SMXIdSer>(n);
  addNRef(n, id);
  return id;
}

void SMXIdManager::addNRef(XXN::DOMAttr* n, SMXId id) {
  IdMapEntry& entry = idMap[id];

  //assert(entry.node); <- may not yet exist...
  entry.nref.insert(n);
}

void SMXIdManager::analyze(XXN::DOMNode *n) {

  if (XXN::DOMNamedNodeMap* attrs = n->getAttributes()) {
    for (size_t i = 0; i < attrs->getLength(); ++i) {
      
      XXN::DOMAttr* attr = static_cast<XXN::DOMAttr*>(attrs->item(i));
      assert(attr);
      
      try {
        SMXId id = X::getNodeValueAs<SMXIdSer>(attr);
        
        if(attr->getNodeName() == X::XStr("id"))
          addNode(n, id);
        else
          addNRef(attr, id);
      }
      catch(InvalidConversion) {
        // attribute contains no valid Id => ignore it
      }
    }
  }

  for(XXN::DOMNode* child = n->getFirstChild();
      child;
      child = child->getNextSibling())
  {
    if(child->getNodeType() == XXN::DOMNode::ELEMENT_NODE)
      analyze(child);
  }
}

XXN::DOMNode* SMXIdManager::getNode(SMXId id) const {
  XXN::DOMNode* n = 0;
  IdMap::const_iterator iter = idMap.find(id);
  
  if(iter != idMap.end())
    n = iter->second.node;

  return n;
}

SMXId SMXIdManager::getId(XXN::DOMNode* n) const {
  
  for(IdMap::const_iterator idIter = idMap.begin();
      idIter != idMap.end();
      ++idIter)
  {
    if(idIter->second.node == n)
      return idIter->first;
  }
  
  return 0;
}

const SMXIdManager::NRef* SMXIdManager::getNRef(SMXId id) const {
  const NRef* retval = 0;
  IdMap::const_iterator iter = idMap.find(id);

  if(iter != idMap.end())
    retval = &iter->second.nref;
    
  return retval;
}

void SMXIdManager::delNode(XXN::DOMNode* n)
  { delNode(X::getAttrValueAs<SMXIdSer>(n, "id")); }

void SMXIdManager::delNode(SMXId id) {
  IdMap::iterator iter = idMap.find(id);
  if(iter == idMap.end()) return;

  IdMapEntry& entry = iter->second;

  for(NRef::const_iterator iter = entry.nref.begin();
      iter != entry.nref.end();
      ++iter)
  {
    XXN::DOMAttr* attr = *iter;
    assert(attr);

    XXN::DOMElement* owner = attr->getOwnerElement();
    assert(owner);

    // remove reference attribute from referencing tag
    owner->removeAttributeNode(attr)->release();
  }

  entry.node = 0;
  if(!entry.obj) idMap.erase(iter);
}

void SMXIdManager::delNRef(XXN::DOMAttr* n)
  { delNRef(X::getNodeValueAs<SMXIdSer>(n), n); }

void SMXIdManager::delNRef(SMXId id, XXN::DOMAttr* n) {
  IdMap::iterator iter = idMap.find(id);
  if(iter == idMap.end()) return;

  IdMapEntry& entry = iter->second;
  assert(entry.node);

  // remove the reference from the referencing nodes
  entry.nref.erase(n);
}

SMXId SMXIdManager::calcAnonId() const {
  SMXId id = 1;

  for(IdMap::const_iterator i = idMap.lower_bound(id);
      i != idMap.end();
      ++i)
  {
    // we can stop searching if gap in Id sequence
    if(i->first > id)
      break;
    id = i->first + 1;
  }

  return id;
}

SMXId SMXIdManager::calcNameIdObj(const char* name) const {
  SMXId id = hashName(name) + offName;

  for(IdMap::const_iterator i = idMap.lower_bound(id);
      i != idMap.end();
      ++i)
  {
    // we can stop searching if either a) entry has
    // no object or b) if gap in Id sequence
    if(!i->second.obj || i->first > id)
      break;
    id = i->first + 1;
  }

  return id;
}

SMXId SMXIdManager::calcNameIdNode(const XMLCh* name) const {
  SMXId id = hashName(name) + offName;

  for(IdMap::const_iterator i = idMap.lower_bound(id);
      i != idMap.end();
      ++i)
  {
    // we can stop searching if either a) entry has
    // no node or b) if gap in Id sequence
    if(!i->second.node || i->first > id)
      break;
    id = i->first + 1;
  }

  return id;
}

SMXId SMXIdManager::addAnon() {
  SMXId id = calcAnonId();
  idMap[id];
  return id;
}

SMXId SMXIdManager::addObj(SCObj* obj, size_t index) {
  assert(obj != NULL);
  
  // if user tries to add same object more than once...
  SMXId id = getId(obj, index);
  if (id) return id;
  
  // calculate new named id and add object
  id = calcNameIdObj(obj->name());
  addObj(obj, id, index);
  
  return id;
}

void SMXIdManager::addObj(SCObj* obj, SMXId id, size_t index) {
  assert(obj != NULL);
  
  IdMapEntry& entry = idMap[id];
  
  if(entry.obj) {
    assert(entry.obj == obj);
    assert(entry.index == index);
  }
  else {
    entry.obj = obj;
    entry.index = index;
  }
}

void SMXIdManager::delObj(const SCObj* obj) {
  assert(obj != NULL);
  
  // could try to hash name and lookup entry and compare obj's
  // before searching...
  
  for(IdMap::iterator idNext, idIter = idMap.lower_bound(offName);
      idIter != idMap.end();
      idIter = idNext)
  {
    ++(idNext = idIter);
    IdMapEntry& entry = idIter->second;

    if(entry.obj == obj) {
      entry.obj = 0;
      if(!entry.node) idMap.erase(idIter);
    }
  }
}

void SMXIdManager::delObj(const SCObj* obj, size_t index) {
  assert(obj != NULL);
  
  // could try to hash name and lookup entry and compare obj's
  // before searching...
  
  for (IdMap::iterator idIter = idMap.lower_bound(offName);
       idIter != idMap.end();
       ++idIter) {
    IdMapEntry& entry = idIter->second;
    
    if (entry.obj == obj && entry.index == index) {
      entry.obj = NULL;
      if(!entry.node) idMap.erase(idIter);
      break;
    }
  }
}

void SMXIdManager::delObj(SMXId id) {
  IdMap::iterator idIter = idMap.find(id);
  
  if (idIter == idMap.end())
    return;
  
  IdMapEntry &entry = idIter->second;
  
  entry.obj = NULL;
  if (!entry.node)
    idMap.erase(idIter);
}

SMXIdManager::SCObj* SMXIdManager::getObj(SMXId id) const {
  SCObj                  *obj  = NULL;
  IdMap::const_iterator   iter = idMap.find(id);
  
  if (iter != idMap.end())
    obj = iter->second.obj;
  
  return obj;
}

SMXId SMXIdManager::getId(const SCObj* obj, size_t index) const {
  for (IdMap::const_iterator idIter = idMap.lower_bound(offName);
       idIter != idMap.end();
       ++idIter) {
    const IdMapEntry& entry = idIter->second;
    
    if (entry.obj == obj && entry.index == index)
      return idIter->first;
  }
  return 0;
}

void SMXIdManager::anonToNamed() {
  IdMap::iterator idIter, idNext;
  
  for (idIter = idMap.begin();
       idIter != idMap.upper_bound(offName);
       idIter = idNext) {
    ++(idNext = idIter);
    
    IdMapEntry& entry = idIter->second;
    
    assert(!entry.obj);
    assert(entry.node);
    
    XXN::DOMNamedNodeMap* attrs = entry.node->getAttributes();
    assert(attrs);
    
    XXN::DOMNode* name = attrs->getNamedItem(X::XStr("name"));
    if(!name) continue;
    
    XXN::DOMNode* id = attrs->getNamedItem(X::XStr("id"));
    assert(id);
    
    std::cerr << "node " << X::XStr(entry.node->getNodeName()) << " ("
              << X::XStr(id->getNodeValue()) << ") has name: "
              << X::XStr(name->getNodeValue()) << std::endl;
    
    SMXId idNew = calcNameIdNode(name->getNodeValue());
    std::cerr << "will get Id: " << idNew << std::endl;
    
    // update node
    X::setNodeValueFrom<SMXIdSer>(id, idNew);
    
    // update references
    for(NRef::iterator rIter = entry.nref.begin();
        rIter != entry.nref.end();
        ++rIter)
    {
      X::setNodeValueFrom<SMXIdSer>(*rIter, idNew);
    }
    
    // create new entry
    IdMapEntry& entryNew = idMap[idNew];
    entryNew.node = entry.node;
    entryNew.nref = entry.nref;
    
    // delete old entry
    idMap.erase(idIter);
  }
}

} // namespace CoSupport
