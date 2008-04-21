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

SMXIdManager::IdMapEntry::IdMapEntry(XXN::DOMNode* node) :
  node(node)
{}

SMXIdManager::SMXIdManager()
{}

SMXIdManager& SMXIdManager::getInstance() {
  static SMXIdManager instance;
  return instance;
}

void SMXIdManager::addNode(XXN::DOMNode *n) {
  SMXId id = getUnusedId();
  X::setAttrValueFrom(n, "id", CoSupport::asStr<SMXIdSer>(id));
  addNode(n, id);
}

void SMXIdManager::addNode(XXN::DOMNode *n, SMXId id) {
  
  assert(n->getNodeType() == XXN::DOMNode::ELEMENT_NODE);

  std::pair<IdMap::iterator,bool> inserted =
    idMap.insert(IdMap::value_type(id, n));
  
  assert(inserted.first != idMap.end());
  IdMapEntry& entry = inserted.first->second;

  // if already inserted -> must be same node
  if(!inserted.second)
    assert(entry.node = n);
}

void SMXIdManager::addNRef(Xerces::XN::DOMAttr* n)
{ addNRef(n, X::getNodeValueAs<SMXIdSer>(n)); }

void SMXIdManager::addNRef(Xerces::XN::DOMAttr* n, SMXId id) {

  std::pair<IdMap::iterator,bool> inserted =
    idMap.insert(IdMap::value_type(id, 0));

  assert(inserted.first != idMap.end());
  IdMapEntry& entry = inserted.first->second;

  // add the reference to the entry
  entry.nref.insert(n);
}

void SMXIdManager::analyze(Xerces::XN::DOMNode *n) {
  
  if(XXN::DOMNamedNodeMap* attrs = n->getAttributes()) {
    for(size_t i = 0; i < attrs->getLength(); ++i) {
      
      XXN::DOMAttr* attr = static_cast<XXN::DOMAttr*>(attrs->item(i));
      assert(attr);
    
      try {
        SMXId id = X::getNodeValueAs<SMXIdSer>(attr);
    
        if(attr->getNodeName() == X::XStr("id")) {
          // an Id attribute adds node
          addNode(n, id);
          std::cerr << "Node '" << X::XStr(n->getNodeName()) << "': "
                    << id << std::endl;
        }
        else {
          // other attribute containing Id adds reference
          addNRef(attr, id);
          std::cerr << "NRef '" << X::XStr(attr->getNodeName()) << "': "
                    << id << std::endl;
        }
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
  XXN::DOMNode *retval = 0;
  IdMap::const_iterator iter = idMap.find(id);
  
  if(iter != idMap.end())
    retval = iter->second.node;
    
  return retval;
}

const SMXIdManager::NRef* SMXIdManager::getNRef(SMXId id) const {
  const NRef* retval = 0;
  IdMap::const_iterator iter = idMap.find(id);

  if(iter != idMap.end())
    retval = &iter->second.nref;
    
  return retval;
}

void SMXIdManager::delNode(XXN::DOMNode* n)
{ delNode(n, X::getAttrValueAs<SMXIdSer>(n, "id")); }

void SMXIdManager::delNode(XXN::DOMNode* n, SMXId id) {
  IdMap::iterator iter = idMap.find(id);
  assert(iter != idMap.end() && iter->second.node == n);

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

  idMap.erase(iter);
}

void SMXIdManager::delNRef(XXN::DOMAttr* n)
{ delNRef(n, X::getNodeValueAs<SMXIdSer>(n)); }

void SMXIdManager::delNRef(XXN::DOMAttr* n, SMXId id) {
  IdMap::iterator iter = idMap.find(id);
  assert(iter != idMap.end());
    
  IdMapEntry& entry = iter->second;

  // remove the reference from the referencing nodes
  entry.nref.erase(n);
}

SMXId SMXIdManager::getUnusedId() const {
  // FIXME: this should be merged with SysteMoC::NGXSync::IdPool
  static const SMXId GENERATED = 0 << 30;
  static const SMXId NAMED     = 1 << 30;
  // search for free id
  SMXId id = GENERATED - 1;
  for(IdMap::const_iterator i = idMap.begin(); i != idMap.end(); ++i) {
    if(i->first > id + 1)
      break;
    id = i->first;
  }
  id++;
  // we should not get into NAMED region
  assert(id < NAMED);
  return id;
}


} // namespace CoSupport
