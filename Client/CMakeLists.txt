cmake_minimum_required(VERSION 2.6)

project(NetController)
INCLUDE(FindPkgConfig)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

option(USE_BOOST_THREAD "Use Boost's thread library for sleeping instead of standard library")
if (USE_BOOST_THREAD)
  add_definitions(-DUSE_BOOST_THREAD)
  find_package(Boost REQUIRED COMPONENTS thread)
endif (USE_BOOST_THREAD)

option(ASIO_STANDALONE "Use the standalone version of ASIO")
if (ASIO_STANDALONE)
  add_definitions(-DASIO_STANDALONE)
elseif (NOT USE_BOOST_THREAD)
  find_package(Boost REQUIRED)
  add_definitions(-DBOOST_SYSTEM_NO_DEPRECATED -DBOOST_ERROR_CODE_HEADER_ONLY)
endif (ASIO_STANDALONE)

file(GLOB SOURCES *.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${Boost_LIBRARIES})
