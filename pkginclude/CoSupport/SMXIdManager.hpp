#ifndef INCLUDED_COSUPPORT_SMX_ID_MANAGER
#define INCLUDED_COSUPPORT_SMX_ID_MANAGER

#include "XML/xerces_support.hpp"
#include "String/convert.hpp"
#include "Math/string_hash.hpp"

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
struct SMXIdSer {
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

class SMXIdManager {
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
  static const SMXId offAnon = 0 << (bits - 1);

  /// @brief Offset for named Ids
  static const SMXId offName = 1 << (bits - 1);
  
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