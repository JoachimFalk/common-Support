# -*- tab-width:8; indent-tabs-mode:nil; c-basic-offset:2; -*-
# vim: set sw=2 ts=8 sts=2 et:
#
# Copyright (c)
#   2015 FAU -- Joachim Falk <joachim.falk@fau.de>
#   2016 FAU -- Joachim Falk <joachim.falk@fau.de>
#   2020 FAU -- Joachim Falk <joachim.falk@fau.de>
# 
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 2 of the License, or (at your option) any
# later version.
# 
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.

##                                               -*- Autoconf -*-
## Process this file with autoconf to produce a configure script.

AC_INIT(
  [HscdSupport Devel Branch], [1.0],
  [joachim.falk@fau.de],
  [HscdSupport--devel-])

AC_PREREQ(2.57)
ACJF_INIT

AC_CONFIG_SRCDIR([src/cosupport-string/cpp/TemplateProcessor.cpp])
AC_CONFIG_AUX_DIR([../BuildSystem])
AC_CONFIG_MACRO_DIR([../BuildSystem/m4])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST

dnl Checks for programs.
ACJF_CHECK_CROSSTOOL([CC], [gcc])
ACJF_CHECK_CROSSTOOL([CXX], [c++])
ACJF_CHECK_CROSSTOOL([AR], [ar])
ACJF_CHECK_CROSSTOOL([RANLIB], [ranlib])
ACJF_CHECK_CROSSTOOL([NM], [nm])

ACJF_ENABLE_DIALECTS_C99_CXX11

AM_INIT_AUTOMAKE([subdir-objects])
AC_PROG_CXXCPP
AC_PROG_CPP
AC_PROG_LIBTOOL
PKG_PROG_PKG_CONFIG

AC_PATH_PROG([BASH], [bash], [/bin/bash])
AC_SUBST([BASH])

dnl AC_DISABLE_STATIC
# We need AC_ENABLE_SHARED for the libsgx python bindings
AC_ENABLE_SHARED

ACJF_ARG_DEBUG([yes], [], [-DNDEBUG])
if test x"$enable_debug" = x"yes"; then
  AC_DEFINE([COSUPPORT_ENABLE_DEBUG], [yes], [Defined if debug support is enabled])
fi


AC_CONFIG_HEADER([config.h src/cosupport-base/headers/CoSupport/cosupport_config.h])

DX_HTML_FEATURE(ON)
DX_CHM_FEATURE(OFF)
DX_CHI_FEATURE(OFF)
DX_MAN_FEATURE(ON)
DX_RTF_FEATURE(OFF)
DX_XML_FEATURE(OFF)
DX_PDF_FEATURE(ON)
DX_PS_FEATURE(OFF)
DX_INIT_DOXYGEN("AC_PACKAGE_NAME", [doxygen.cfg], [doxygen-doc])

dnl check for exe extension
AC_EXEEXT

# Checks for header files.
# AC_FUNC_ALLOCA
# AC_HEADER_STDC

AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([cstddef stddef.h])
AC_LANG_POP

# create defines appropriate for pkginclude/CoSupport/cosupport_config.h
if test x"$ac_cv_header_cstddef" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_CSTDDEF], [1], [Define to 1 if you have the <cstddef> header file.])
fi
if test x"$ac_cv_header_stddef_h" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_STDDEF_H], [1], [Define to 1 if you have the <stddef.h> header file.])
fi

AC_CACHE_CHECK([wether the C++ compiler supports nullptr],
 [acjf_cv_cxx11_nullptr_support],
 [AC_LANG_PUSH([C++])
  acjf_var_CXXFLAGS="${CXXFLAGS}"
  CXXFLAGS="${acjf_var_CXXFLAGS} ${CXX11FLAGS}"
  AC_COMPILE_IFELSE(
   [AC_LANG_PROGRAM([[
#if defined(HAVE_CSTDDEF)
# include <cstddef>
#elif defined(HAVE_STDDEF_H)
# include <stddef.h>
#else
# include <stdlib.h>
#endif
]], [[
std::nullptr_t mynull = nullptr;
]], [])],
   [AC_MSG_RESULT([yes]); acjf_cv_cxx11_nullptr_support="yes";],
   [AC_MSG_RESULT([no]); acjf_cv_cxx11_nullptr_support="no";])
  CXXFLAGS="${acjf_var_CXXFLAGS}"
  AC_LANG_POP])
if test x"$acjf_cv_cxx11_nullptr_support" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_STD_NULLPTR_T], [1], [Define to 1 if you have the <cstddef> header defining the std::nullptr_t data type.])
fi

# These tests are all for compatibility-glue/integertypes.h
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h sys/types.h])

# create defines appropriate for pkginclude/CoSupport/cosupport_config.h
if test x"$ac_cv_header_stddef_h" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_STDDEF_H], [1], [Define to 1 if you have the <stddef.h> header file.])
fi
if test x"$ac_cv_header_stdint_h" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_STDINT_H], [1], [Define to 1 if you have the <stdint.h> header file.])
fi
if test x"$ac_cv_header_stdlib_h" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_STDLIB_H], [1], [Define to 1 if you have the <stdlib.h> header file.])
fi
if test x"$ac_cv_header_sys_types_h" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_SYS_TYPES_H], [1], [Define to 1 if you have the <sys/types.h> header file.])
fi

acjf_cosupport_a_type_is_missing=no
ACJF_CHECK_TYPE(int8_t, [stddef.h stdint.h stdlib.h sys/types.h])
if test x"$ac_cv_type_int8_t" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_INT8_T], [1], [Define to 1 if you have the int8_t type.])
else
  acjf_cosupport_a_type_is_missing=yes
fi
ACJF_CHECK_TYPE(uint8_t, [stddef.h stdint.h stdlib.h sys/types.h])
if test x"$ac_cv_type_uint8_t" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_UINT8_T], [1], [Define to 1 if you have the uint8_t type.])
else
  acjf_cosupport_a_type_is_missing=yes
fi
ACJF_CHECK_TYPE(int16_t, [stddef.h stdint.h stdlib.h sys/types.h])
if test x"$ac_cv_type_int16_t" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_INT16_T], [1], [Define to 1 if you have the int16_t type.])
else
  acjf_cosupport_a_type_is_missing=yes
fi
ACJF_CHECK_TYPE(uint16_t, [stddef.h stdint.h stdlib.h sys/types.h])
if test x"$ac_cv_type_uint16_t" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_UINT16_T], [1], [Define to 1 if you have the uint16_t type.])
else
  acjf_cosupport_a_type_is_missing=yes
fi
ACJF_CHECK_TYPE(int32_t, [stddef.h stdint.h stdlib.h sys/types.h])
if test x"$ac_cv_type_int32_t" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_INT32_T], [1], [Define to 1 if you have the int32_t type.])
else
  acjf_cosupport_a_type_is_missing=yes
fi
ACJF_CHECK_TYPE(uint32_t, [stddef.h stdint.h stdlib.h sys/types.h])
if test x"$ac_cv_type_uint32_t" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_UINT32_T], [1], [Define to 1 if you have the uint32_t type.])
else
  acjf_cosupport_a_type_is_missing=yes
fi
ACJF_CHECK_TYPE(int64_t, [stddef.h stdint.h stdlib.h sys/types.h])
if test x"$ac_cv_type_int64_t" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_INT64_T], [1], [Define to 1 if you have the int64_t type.])
else
  acjf_cosupport_a_type_is_missing=yes
fi
ACJF_CHECK_TYPE(uint64_t, [stddef.h stdint.h stdlib.h sys/types.h])
if test x"$ac_cv_type_uint64_t" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_UINT64_T], [1], [Define to 1 if you have the uint64_t type.])
else
  acjf_cosupport_a_type_is_missing=yes
fi
ACJF_CHECK_TYPE(ssize_t, [stddef.h stdint.h stdlib.h sys/types.h])
if test x"$ac_cv_type_ssize_t" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_SSIZE_T], [1], [Define to 1 if you have the ssize_t type.])
else
  acjf_cosupport_a_type_is_missing=yes
fi
ACJF_CHECK_TYPE(size_t, [stddef.h stdint.h stdlib.h sys/types.h])
if test x"$ac_cv_type_size_t" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_SIZE_T], [1], [Define to 1 if you have the size_t type.])
else
  acjf_cosupport_a_type_is_missing=yes
fi
ACJF_CHECK_TYPE(long long)
if test x"$ac_cv_type_long_long" = x"yes"; then
  AC_DEFINE([COSUPPORT_HAVE_LONG_LONG], [1], [Define to 1 if you have the long long type.])
fi

if test x"$acjf_cosupport_a_type_is_missing" = x"yes"; then
  # A short int must not be larger than an int.
  # An int must not be larger than a long int.
  # A short int must be at least 16 bits long.
  # An int must be at least 16 bits long.
  # A long int must be at least 32 bits long.
  # A long long int must be at least 64 bits long.
  #
  # The standard does not require that any of these sizes be necessarily
  # different. It is perfectly valid, for example, if all four types are 64
  # bits long.
  
  AC_CHECK_HEADERS([limits.h])
  
  AC_COMPUTE_INT([acjf_cosupport_bits_char],      [sizeof(char)*CHAR_BIT],
   [#ifdef HAVE_LIMITS_H
    # include <limits.h>
    #endif
    #ifndef CHAR_BIT
    # define CHAR_BIT 8
    #endif])
  AC_COMPUTE_INT([acjf_cosupport_bits_short],     [sizeof(short)*CHAR_BIT],
   [#ifdef HAVE_LIMITS_H
    # include <limits.h>
    #endif
    #ifndef CHAR_BIT
    # define CHAR_BIT 8
    #endif])
  AC_COMPUTE_INT([acjf_cosupport_bits_int],       [sizeof(int)*CHAR_BIT],
   [#ifdef HAVE_LIMITS_H
    # include <limits.h>
    #endif
    #ifndef CHAR_BIT
    # define CHAR_BIT 8
    #endif])
  AC_COMPUTE_INT([acjf_cosupport_bits_long],      [sizeof(long)*CHAR_BIT],
   [#ifdef HAVE_LIMITS_H
    # include <limits.h>
    #endif
    #ifndef CHAR_BIT
    # define CHAR_BIT 8
    #endif])
  AC_COMPUTE_INT([acjf_cosupport_bits_long_long], [sizeof(long long)*CHAR_BIT],
   [#ifdef HAVE_LIMITS_H
    # include <limits.h>
    #endif
    #ifndef CHAR_BIT
    # define CHAR_BIT 8
    #endif])
  
  #echo $acjf_cosupport_bits_char
  #echo $acjf_cosupport_bits_short
  #echo $acjf_cosupport_bits_int
  #echo $acjf_cosupport_bits_long
  #echo $acjf_cosupport_bits_long_long
  
  if test x"$ac_cv_type_int8_t" != x"yes"; then
    if test x"$acjf_cosupport_bits_char" = x"8"; then
      AC_DEFINE([COSUPPORT_REPLTYP_INT8_T], [signed char], [Define to the replacement type for int8_t if this type is missing.])
    else
      AC_MSG_ERROR([Can't find a replacement type for int8_t])
    fi
  fi
  if test x"$ac_cv_type_uint8_t" != x"yes"; then
    if test x"$acjf_cosupport_bits_char" = x"8"; then
      AC_DEFINE([COSUPPORT_REPLTYP_UINT8_T], [unsigned char], [Define to the replacement type for uint8_t if this type is missing.])
    else
      AC_MSG_ERROR([Can't find a replacement type for uint8_t])
    fi
  fi
  if test x"$ac_cv_type_int16_t" != x"yes"; then
    if test x"$acjf_cosupport_bits_char" = x"16"; then
      AC_DEFINE([COSUPPORT_REPLTYP_INT16_T], [signed char], [Define to the replacement type for int16_t if this type is missing.])
    elif test x"$acjf_cosupport_bits_short" = x"16"; then
      AC_DEFINE([COSUPPORT_REPLTYP_INT16_T], [signed short], [Define to the replacement type for int16_t if this type is missing.])
    else
      AC_MSG_ERROR([Can't find a replacement type for int16_t])
    fi
  fi
  if test x"$ac_cv_type_uint16_t" != x"yes"; then
    if test x"$acjf_cosupport_bits_char" = x"16"; then
      AC_DEFINE([COSUPPORT_REPLTYP_UINT16_T], [unsigned char], [Define to the replacement type for uint16_t if this type is missing.])
    elif test x"$acjf_cosupport_bits_short" = x"16"; then
      AC_DEFINE([COSUPPORT_REPLTYP_UINT16_T], [unsigned short], [Define to the replacement type for uint16_t if this type is missing.])
    else
      AC_MSG_ERROR([Can't find a replacement type for uint16_t])
    fi
  fi
  if test x"$ac_cv_type_int32_t" != x"yes"; then
    if test x"$acjf_cosupport_bits_int" = x"32"; then
      AC_DEFINE([COSUPPORT_REPLTYP_INT32_T], [signed int], [Define to the replacement type for int32_t if this type is missing.])
    elif test x"$acjf_cosupport_bits_long" = x"32"; then
      AC_DEFINE([COSUPPORT_REPLTYP_INT32_T], [signed int], [Define to the replacement type for int32_t if this type is missing.])
    else
      AC_MSG_ERROR([Can't find a replacement type for int32_t])
    fi
  fi
  if test x"$ac_cv_type_uint32_t" != x"yes"; then
    if test x"$acjf_cosupport_bits_int" = x"32"; then
      AC_DEFINE([COSUPPORT_REPLTYP_UINT32_T], [unsigned int], [Define to the replacement type for uint32_t if this type is missing.])
    elif test x"$acjf_cosupport_bits_long" = x"32"; then
      AC_DEFINE([COSUPPORT_REPLTYP_UINT32_T], [unsigned long], [Define to the replacement type for uint32_t if this type is missing.])
    else
      AC_MSG_ERROR([Can't find a replacement type for uint32_t])
    fi
  fi
  if test x"$ac_cv_type_int64_t" != x"yes"; then
    if test x"$acjf_cosupport_bits_long" = x"64"; then
      AC_DEFINE([COSUPPORT_REPLTYP_INT64_T], [signed long], [Define to the replacement type for int64_t if this type is missing.])
    elif test x"$acjf_cosupport_bits_long_long" = x"64"; then
      AC_DEFINE([COSUPPORT_REPLTYP_INT64_T], [signed long long], [Define to the replacement type for int64_t if this type is missing.])
    else
      AC_MSG_ERROR([Can't find a replacement type for int64_t])
    fi
  fi
  if test x"$ac_cv_type_uint64_t" != x"yes"; then
    if test x"$acjf_cosupport_bits_long" = x"64"; then
      AC_DEFINE([COSUPPORT_REPLTYP_UINT64_T], [unsigned long], [Define to the replacement type for uint64_t if this type is missing.])
    elif test x"$acjf_cosupport_bits_long_long" = x"64"; then
      AC_DEFINE([COSUPPORT_REPLTYP_UINT64_T], [unsigned long long], [Define to the replacement type for uint64_t if this type is missing.])
    else
      AC_MSG_ERROR([Can't find a replacement type for uint64_t])
    fi
  fi
  
  if test x"$ac_cv_type_size_t" != x"yes" -a x"$ac_cv_type_ssize_t" != x"yes"; then
    # Simply decide on size_t and ssize_t
    AC_DEFINE([COSUPPORT_REPLTYP_SIZE_T], [unsigned long],
      [Define to the replacement type for size_t if this type is missing.])
    AC_DEFINE([COSUPPORT_REPLTYP_SSIZE_T], [signed long],
      [Define to the replacement type for ssize_t if this type is missing.])
  elif test x"$ac_cv_type_size_t" = x"yes" -a x"$ac_cv_type_ssize_t" != x"yes"; then
    # Have to match ssize_t to size_t
    AC_COMPUTE_INT([acjf_cosupport_bits_size_t], [sizeof(size_t)*CHAR_BIT],
     [#ifdef HAVE_LIMITS_H
      # include <limits.h>
      #endif
      #ifndef CHAR_BIT
      # define CHAR_BIT 8
      #endif
      #ifdef HAVE_STDDEF_H
      # include <stddef.h>
      #endif
      #ifdef HAVE_STDINT_H
      # include <stdint.h>
      #endif
      #ifdef HAVE_STDLIB_H
      # include <stdlib.h>
      #endif
      #ifdef HAVE_SYS_TYPES_H
      # include <sys/types.h>
      #endif])
    
    #echo $acjf_cosupport_bits_size_t
    
    case "x$acjf_cosupport_bits_size_t" in
      "x$acjf_cosupport_bits_char")
        acjf_cosupport_repl_ssize_t="signed char";
        ;;
      "x$acjf_cosupport_bits_short")
        acjf_cosupport_repl_ssize_t="short";
        ;;
      "x$acjf_cosupport_bits_int")
        acjf_cosupport_repl_ssize_t="int";
        ;;
      "x$acjf_cosupport_bits_long")
        acjf_cosupport_repl_ssize_t="long";
        ;;
      "x$acjf_cosupport_bits_long_long")
        acjf_cosupport_repl_ssize_t="long long";
        ;;
      *)
        AC_MSG_ERROR([Can not find a replacement type for ssize_t])
        ;;
    esac
    AC_DEFINE_UNQUOTED([COSUPPORT_REPLTYP_SSIZE_T], [$acjf_cosupport_repl_ssize_t],
      [Define to the replacement type for ssize_t if this type is missing.])
  elif test x"$ac_cv_type_size_t" != x"yes" -a x"$ac_cv_type_ssize_t" = x"yes"; then
    # Have to match size_t to ssize_t
    AC_COMPUTE_INT([acjf_cosupport_bits_ssize_t], [sizeof(ssize_t)*CHAR_BIT],
     [#ifdef HAVE_LIMITS_H
      # include <limits.h>
      #endif
      #ifndef CHAR_BIT
      # define CHAR_BIT 8
      #endif
      #ifdef HAVE_STDDEF_H
      # include <stddef.h>
      #endif
      #ifdef HAVE_STDINT_H
      # include <stdint.h>
      #endif
      #ifdef HAVE_STDLIB_H
      # include <stdlib.h>
      #endif
      #ifdef HAVE_SYS_TYPES_H
      # include <sys/types.h>
      #endif])
    
    #echo $acjf_cosupport_bits_ssize_t
    
    case "x$acjf_cosupport_bits_ssize_t" in
      "x$acjf_cosupport_bits_char")
        acjf_cosupport_repl_size_t="unsigned char";
        ;;
      "x$acjf_cosupport_bits_short")
        acjf_cosupport_repl_size_t="unsigned short";
        ;;
      "x$acjf_cosupport_bits_int")
        acjf_cosupport_repl_size_t="unsigned int";
        ;;
      "x$acjf_cosupport_bits_long")
        acjf_cosupport_repl_size_t="unsigned long";
        ;;
      "x$acjf_cosupport_bits_long_long")
        acjf_cosupport_repl_size_t="unsigned long long";
        ;;
      *)
        AC_MSG_ERROR([Can not find a replacement type for size_t])
        ;;
    esac
    AC_DEFINE_UNQUOTED([COSUPPORT_REPLTYP_SIZE_T], [$acjf_cosupport_repl_size_t],
      [Define to the replacement type for size_t if this type is missing.])
  fi
fi

# Checks for typedefs, structures, and compiler characteristics.
dnl ACJF_TYPE_SIZE_T
dnl ACJF_CHECK_STDINT
ACJF_CHECK_VA_COPY

# Checks for libraries.

# This is a hard requirement.
ACJF_CHECK_LIB_BOOST

SUBDIRS=""

ACJF_CHECK_LIB_SYSTEMC(
  [acjf_cosupport_found_pkg_systemc="yes";],
  [acjf_cosupport_found_pkg_systemc="no";])

AM_CONDITIONAL([COSUPPORT_ENABLE_SYSTEMC], [test x"$acjf_cosupport_found_pkg_systemc" = x"yes"])
# This is a define for pkginclude/CoSupport/cosupport_config.h to guarantee
# consistency for Examples and Testcases
if test x"$acjf_cosupport_found_pkg_systemc" = x"yes"; then
  AC_DEFINE([COSUPPORT_ENABLE_SYSTEMC], [yes], [Defined if SystemC support is enabled])
  AC_CONFIG_FILES([
    pkgconfig/libcosupport-systemc.pc
    src/cosupport-systemc/headers/Makefile
  ])
  SUBDIRS="$SUBDIRS src/cosupport-systemc/headers"
fi

ACJF_CHECK_LIB_XERCES(
  [acjf_cosupport_found_pkg_xerces="yes";],
  [acjf_cosupport_found_pkg_xerces="no";])

AM_CONDITIONAL([COSUPPORT_ENABLE_XERCES], [test x"$acjf_cosupport_found_pkg_xerces" = x"yes"])
# This is a define for pkginclude/CoSupport/cosupport_config.h to guarantee
# consistency for Examples and Testcases
if test x"$acjf_cosupport_found_pkg_xerces" = x"yes"; then
  AC_DEFINE([COSUPPORT_ENABLE_XERCES], [yes], [Defined if xerces support is enabled])
  AC_CONFIG_FILES([
    pkgconfig/libcosupport-xerces.pc
    src/cosupport-xerces/headers/Makefile
  ])
  SUBDIRS="$SUBDIRS src/cosupport-xerces/headers"
fi

SUBDIRS="$SUBDIRS src/cosupport-allocators/headers"
SUBDIRS="$SUBDIRS src/cosupport-base/headers"
SUBDIRS="$SUBDIRS src/cosupport-initializer/headers"
SUBDIRS="$SUBDIRS src/cosupport-math/headers"
SUBDIRS="$SUBDIRS src/cosupport-path/headers"
SUBDIRS="$SUBDIRS src/cosupport-random/headers"
SUBDIRS="$SUBDIRS src/cosupport-smartptr/headers"
SUBDIRS="$SUBDIRS src/cosupport-streams/headers"
SUBDIRS="$SUBDIRS src/cosupport-string/headers"
SUBDIRS="$SUBDIRS src/cosupport-tracing/headers"
SUBDIRS="$SUBDIRS . testcases"

AC_CONFIG_FILES([
  pkgconfig/libcosupport.pc
  Makefile
  pkgconfig/libcosupport-allocators.pc
  src/cosupport-allocators/headers/Makefile
  pkgconfig/libcosupport-base.pc
  src/cosupport-base/headers/Makefile
  pkgconfig/libcosupport-initializer.pc
  src/cosupport-initializer/headers/Makefile
  pkgconfig/libcosupport-math.pc
  src/cosupport-math/headers/Makefile
  pkgconfig/libcosupport-path.pc
  src/cosupport-path/headers/Makefile
  pkgconfig/libcosupport-random.pc
  src/cosupport-random/headers/Makefile
  pkgconfig/libcosupport-smartptr.pc
  src/cosupport-smartptr/headers/Makefile
  pkgconfig/libcosupport-streams.pc
  src/cosupport-streams/headers/Makefile
  pkgconfig/libcosupport-string.pc
  src/cosupport-string/headers/Makefile
  pkgconfig/libcosupport-tracing.pc
  src/cosupport-tracing/headers/Makefile
  testcases/Makefile
])

AC_SUBST([SUBDIRS])

ACJF_DONE

AC_OUTPUT
