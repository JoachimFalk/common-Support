dnl Checks for header files.
dnl AC_HEADER_DIRENT
dnl AC_HEADER_STDC
dnl AC_CHECK_HEADERS(fcntl.h limits.h syslog.h unistd.h)

dnl Checks for typedefs, structures, and compiler characteristics.
ACJF_CHECK_TYPE(long long)
ACJF_TYPE_SIZE_T
ACJF_CHECK_STDINT
ACJF_CHECK_VA_COPY

ACJF_CHECK_LIB_BOOST
ACJF_CHECK_LIB_SYSTEMC

AC_CONFIG_FILES(ACJF_VAR_SUBPROJECT_DIR[Makefile])
subdirs="$subdirs ACJF_VAR_SUBPROJECT_DIR"
