#ifndef INCLUDED_COSUPPORT_SERIALIZE_HELPER
#define INCLUDED_COSUPPORT_SERIALIZE_HELPER


namespace CoSupport { namespace DataTypes {

  /**
   * Byte class for proper dumping
   */
  template<class T>
  struct SerializeHelper {

    typedef SerializeHelper<T> this_type;
    
    /// @brief Constructor
    SerializeHelper(const T& val)
      : val(val)
    {}

    SerializeHelper()
    {}

    operator const T&() const{
      return val;
    }

    operator T&() {
      return val;
    }

#if 0
    bool operator!=(const T& val) const {
      return val != this->val;
    }

    bool operator!=(const this_type& a) const {
      return a.val != this->val;
    }
#endif

    /// @brief The Id
    T val;
  };

  template<class T>
  class SerializeHelperPrintTraits{
  public:
    typedef T type;  
  };

  template<>
  class SerializeHelperPrintTraits<unsigned char>{
  public:
    typedef unsigned int type;  
  };

  template<>
  class SerializeHelperPrintTraits<signed char>{
  public:
    typedef unsigned int type;  
  };

  /// @brief Output operator for SMXIdSer
  template<class T>
  std::ostream& operator<<(std::ostream& out, const SerializeHelper<T>& val){
    return out << static_cast<typename SerializeHelperPrintTraits<T>::type >(val);
  }

  typedef SerializeHelper<unsigned char> Byte;


}} // namespace CoSupport::DataTypes

#endif // INCLUDED_COSUPPORT_SERIALIZE_HELPER
