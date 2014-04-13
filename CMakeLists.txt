###############################################################################
#
# http://github.com/breese/aware
#
# Copyright (C) 2013 Bjorn Reese <breese@users.sourceforge.net>
#
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)
#
###############################################################################

# Debug build
#   cmake -DCMAKE_BUILD_TYPE=Debug .

###############################################################################

cmake_minimum_required(VERSION 2.8)
project(aware)

add_definitions(-g -Wall)
#add_definitions(-fmax-errors=1) # gcc
#add_definitions(-ferror-limit=1) # clang

if (NOT DEFINED AWARE_BACKEND)
  set(AWARE_BACKEND "avahi")
endif()

###############################################################################
# Boost package
###############################################################################

# FIXME: Probably ok to use older version
find_package(Boost 1.49.0 COMPONENTS unit_test_framework system thread)
if (NOT ${Boost_FOUND})
  message(FATAL_ERROR "Boost not found (or too old)")
endif()
include_directories(BEFORE ${Boost_INCLUDE_DIR})
set(EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})

###############################################################################
# Avahi package
###############################################################################

if(AWARE_BACKEND STREQUAL "avahi")
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(AVAHI avahi-client)

  if(true)
    if (${DISABLE_AVAHI})
      message(STATUS "AVAHI Disabled by request")
    else()
      if (NOT ${AVAHI_FOUND})
        message(FATAL_ERROR "Please install the avahi-client package")
      else()
        set(USE_AVAHI TRUE)
        include_directories(${AVAHI_INCLUDE_DIRS})
        # This is a workaround to get path of each library
        foreach(lib ${AVAHI_LIBRARIES})
            find_library(${lib}_LIBRARY NAMES ${lib} PATHS ${AVAHI_LIBRARY_DIRS})
            list(APPEND AVAHI_LIBS ${${lib}_LIBRARY})
        endforeach()
        link_directories(${AVAHI_LIBRARY_DIRS})
        set(EXTRA_LIBS ${EXTRA_LIBS} ${AVAHI_LIBS})
      endif()
    endif()
  else()
    # FIXME: Hack for debugging
    set(AVAHI_BASE_DIR /home/breese/src/aware/avahi-0.6.31)
    set(AVAHI_INCLUDE_DIRS ${AVAHI_BASE_DIR})
    set(AVAHI_LIBRARY_DIRS ${AVAHI_BASE_DIR}/avahi-common/.libs ${AVAHI_BASE_DIR}/avahi-client/.libs)
    set(AVAHI_LIBS avahi-common avahi-client)
    include_directories(${AVAHI_INCLUDE_DIRS})
    link_directories(${AVAHI_LIBRARY_DIRS})
    set(EXTRA_LIBS ${EXTRA_LIBS} ${AVAHI_LIBS})
  endif()
elseif(AWARE_BACKEND STREQUAL "android")
  find_package(Java REQUIRED)
  include(UseJava)
  find_package(JNI REQUIRED)
  include_directories(BEFORE ${JNI_INCLUDE_DIRS})
endif()

###############################################################################
# Aware package
###############################################################################

set(AWARE_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

include_directories(BEFORE include)
link_directories(${AWARE_LIB_DIR})
set(LIBRARY_OUTPUT_PATH lib)

set(AWARE_SRC
  src/contact.cpp
  src/detail/native_socket.cpp
)

set(AWARE_AVAHI_SOURCE
  src/avahi/monitor_socket.cpp
  src/avahi/announce_socket.cpp
  src/avahi/detail/announcer.cpp
  src/avahi/detail/browser.cpp
  src/avahi/detail/client.cpp
  src/avahi/detail/error.cpp
  src/avahi/detail/poller.cpp
)

set(AWARE_ANDROID_SOURCE
  src/android/monitor_socket.cpp
  src/android/detail/client.cpp
  src/android/detail/aware_jni.cpp
)

set(AWARE_ANDROID_JAVA_SOURCE
  src/android/java/dk/xpg/aware/Aware.java
  src/android/java/dk/xpg/aware/AwareContact.java
  src/android/java/dk/xpg/aware/AwareAdapter.java
)

if(AWARE_BACKEND STREQUAL "avahi")
  list(APPEND AWARE_SRC ${AWARE_AVAHI_SOURCE})
elseif(AWARE_BACKEND STREQUAL "android")
  add_jar(AwareAndroid ${AWARE_ANDROID_JAVA_SOURCE})

  list(APPEND AWARE_SRC ${AWARE_ANDROID_SOURCE})

  add_library(AwareAndroidJNI SHARED 
        src/android/jni/AwareAndroid.cpp
    )
  target_link_libraries(AwareAndroidJNI aware)
else()
  message(FATAL_ERROR "'${AWARE_BACKEND}' is an invalid value for AWARE_BACKEND")
endif()

add_library(aware STATIC ${AWARE_SRC})
target_link_libraries(aware ${EXTRA_LIBS})
set_target_properties(aware PROPERTIES COMPILE_FLAGS "-fPIC")

###############################################################################
# Examples
###############################################################################

add_executable(zannounce EXCLUDE_FROM_ALL
  example/zannounce.cpp
)
set_target_properties(zannounce PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)
target_link_libraries(zannounce aware)

add_executable(zmonitor EXCLUDE_FROM_ALL
  example/zmonitor.cpp
)
set_target_properties(zmonitor PROPERTIES RUNTIME_OUTPUT_DIRECTORY bin)
target_link_libraries(zmonitor aware)

add_subdirectory(test)
