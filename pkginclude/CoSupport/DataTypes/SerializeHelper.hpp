#ifndef INCLUDED_COSUPPORT_SERIALIZE_HELPER
#define INCLUDED_COSUPPORT_SERIALIZE_HELPER


namespace CoSupport { namespace DataTypes {

  /**
   * Byte class for proper dumping
   */
  template<class T>
  struct SerializeHelper {
    /// @brief Constructor
    SerializeHelper(const T& val)
      : val(val)
    {}

    operator const T&() const{
      return val;
    }

    operator T&() {
      return val;
    }

    /// @brief The Id
    T val;
  };

  template<class T>
  class SerializeHelperPrintTraits{
    typedef T type;  
  };

  template<>
  class SerializeHelperPrintTraits<unsigned char>{
    typedef unsigned int type;  
  };

  template<>
  class SerializeHelperPrintTraits<signed char>{
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
