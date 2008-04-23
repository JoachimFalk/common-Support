#include <cosupport/SMXIdManager.hpp>
#include <cassert>
#include <sstream>

namespace CoSupport {

namespace X = Xerces;
namespace XXN = Xerces::XN;

struct InvalidConversion {};

SMXIdSer::SMXIdSer(SMXId id) :
  id(id)
{}

SMXIdSer::operator SMXId() const
{ return id; }

template<>
std::string asStr<SMXIdSer>(const SMXIdSer& id) {
  std::ostringstream out;
  out << "id" << id;
  return out.str();
}

template<>
SMXIdSer strAs<SMXIdSer>(const std::string &s) {
  if(s.size() < 3 || s[0] != 'i' || s[1] != 'd')
    throw InvalidConversion();

  SMXId ret = 0;
  
  for(size_t i = 2; i < s.size(); ++i) {
    if(s[i] < '0' || s[i] > '9')
      throw InvalidConversion();
    ret = ret * 10 + (s[i] - '0');
  }

  return ret;
}

SMXIdManager::IdMapEntry::IdMapEntry() :
  node(0),
  obj(0),
  index(0)
{}

SMXIdManager::SMXIdManager()
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
  
  IdMapEntry& entry = idMap[id];

  if(entry.node) {
    assert(entry.node == n);
  }
  else {
    //assert(entry.nref.empty()); <- see addNRef
    entry.node = n;
  }
}

SMXId SMXIdManager::addNRef(Xerces::XN::DOMAttr* n) {
  SMXId id = X::getNodeValueAs<SMXIdSer>(n);
  addNRef(n, id);
  return id;
}

void SMXIdManager::addNRef(Xerces::XN::DOMAttr* n, SMXId id) {
  IdMapEntry& entry = idMap[id];

  //assert(entry.node); <- may not yet exist...
  entry.nref.insert(n);
}

void SMXIdManager::analyze(Xerces::XN::DOMNode *n) {

  if(XXN::DOMNamedNodeMap* attrs = n->getAttributes()) {
    for(size_t i = 0; i < attrs->getLength(); ++i) {
      
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
  if(!obj) return 0;

  // if user tries to add same object more than once...
  SMXId id = getId(obj, index);
  if(id) return id;

  // calculate new named id and add object
  id = calcNameIdObj(obj->name());
  addObj(obj, id, index);

  return id;
}

void SMXIdManager::addObj(SCObj* obj, SMXId id, size_t index) {
  if(!obj) return;

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
  if(!obj) return;
  
  // could try to hash name and lookup entry and compare obj's
  // before searching...

  for(IdMap::iterator idIter = idMap.lower_bound(offName);
      idIter != idMap.end();
      ++idIter)
  {
    IdMapEntry& entry = idIter->second;

    if(entry.obj == obj) {
      entry.obj = 0;
      if(!entry.node) idMap.erase(idIter);
    }
  }
}

void SMXIdManager::delObj(const SCObj* obj, size_t index) {
  if(!obj) return;

  // could try to hash name and lookup entry and compare obj's
  // before searching...

  for(IdMap::iterator idIter = idMap.lower_bound(offName);
      idIter != idMap.end();
      ++idIter)
  {
    IdMapEntry& entry = idIter->second;

    if(entry.obj == obj && entry.index == index) {
      entry.obj = 0;
      if(!entry.node) idMap.erase(idIter);
      break;
    }
  }
}

void SMXIdManager::delObj(SMXId id) {
  IdMap::iterator idIter = idMap.find(id);
  if(idIter == idMap.end()) return;

  IdMapEntry& entry = idIter->second;
  
  entry.obj = 0;
  if(!entry.node) idMap.erase(idIter);
}

SMXIdManager::SCObj* SMXIdManager::getObj(SMXId id) const {
  SCObj* obj = 0;
  IdMap::const_iterator iter = idMap.find(id);
  
  if(iter != idMap.end())
    obj = iter->second.obj;

  return obj;
}

SMXId SMXIdManager::getId(const SCObj* obj, size_t index) const {

  for(IdMap::const_iterator idIter = idMap.lower_bound(offName);
      idIter != idMap.end();
      ++idIter)
  {
    const IdMapEntry& entry = idIter->second;

    if(entry.obj == obj && entry.index == index)
      return idIter->first;
  }

  return 0;
}

void SMXIdManager::anonToNamed() {

  IdMap::iterator idIter, idNext;

  for(idIter = idMap.begin();
      idIter != idMap.upper_bound(offName);
      idIter = idNext)
  {
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

const SMXId SMXIdManager::offAnon;
const SMXId SMXIdManager::offName;

} // namespace CoSupport
