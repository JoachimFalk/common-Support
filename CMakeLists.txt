cmake_minimum_required(VERSION 2.4)

project(COSUPPORT)

# Make sure the compiler can find include files from our Hello library.
include_directories(${COSUPPORT_SOURCE_DIR}/pkginclude)

# Make sure the linker can find the Hello library once it is built.
link_directories(${COSUPPORT_BINARY_DIR})

add_library(cosupport-xerces SHARED
  SMXIdManager.cpp)
target_link_libraries(cosupport-xerces
  cosupport-initializer
  xerces-c)

add_library(cosupport-streams SHARED
  DebugOStream.cpp
  FilterOStream.cpp
  SocketTCPClient.cpp)
target_link_libraries(cosupport-streams
  boost_system
  boost_iostreams)

add_library(cosupport-string SHARED
  TemplateProcessor.cpp
  UniquePool.cpp)

add_library(cosupport-systemc SHARED
  par_manager.cpp
  PerformanceEvaluation.cpp)
target_link_libraries(cosupport-systemc
  systemc)

add_library(cosupport-smartptr SHARED
  RefCountObject.cpp)

add_library(cosupport-initializer SHARED
  BasicInitializer.cpp)
target_link_libraries(cosupport-initializer
  boost_thread)

add_executable(test_event_and_list
  test_event_and_list.cpp)
target_link_libraries(test_event_and_list
  cosupport-systemc)

add_executable(test_nested_event_lists
  test_nested_event_lists.cpp)
target_link_libraries(test_nested_event_lists
  cosupport-systemc
  cosupport-streams)

#install(
#  TARGETS
#    cosupport-xerces
#    cosupport-streams
#    cosupport-string
#    cosupport-systemc
#    cosupport-smartptr
#    cosupport-initializer
#    test_event_and_list
#    test_nested_event_lists)
