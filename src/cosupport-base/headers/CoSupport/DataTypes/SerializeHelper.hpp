// -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
// vim: set sw=2 ts=8 sts=2 et:
/*
 * Copyright (c)
 *   2010 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
 *   2020 FAU -- Joachim Falk <joachim.falk@fau.de>
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option) any
 * later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

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
