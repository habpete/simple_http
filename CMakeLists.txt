cmake_minimum_required(VERSION 3.2)

project(simple_http VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -Werror")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)
find_package(Boost 1.42.0 REQUIRED COMPONENTS system thread regex)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  link_directories(${Boost_LIBRARY_DIRS})
endif(Boost_FOUND)

set(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

add_executable (main main.cpp internal/service.h internal/service.cpp internal/spec/spec.h internal/parser/parser.h internal/parser/parser.cpp internal/sender/sender.h internal/sender/sender.cpp)
target_link_libraries(main ${USED_LIBS})
