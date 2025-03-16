cmake_minimum_required(VERSION 3.10)
project(archivelib-sys-refactored CXX)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall")

if (UNIX)
  add_definitions(-DAL_UNIX -DAL_SUN4)
endif ()
add_definitions(-DAL_CUSTOM -DAL_SYMANTEC)

set ( FLAGS "" )

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set ( FLAGS "${FLAGS} -fstack-protector -fsanitize-memory-track-origins -O3" )
  if (NOT CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    # Apple Clang doesn't support this option
    set ( FLAGS "${FLAGS} -fno-stack-array" )
  endif ()
  set ( SANITIZERS "undefined" "address" "bounds" )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set ( FLAGS "${FLAGS} -fstack-protector-strong -fstack-protector-all --coverage -O3 --param ssp-buffer-size=1 -fno-stack-arrays" )
  set ( SANITIZERS "undefined" "address" "bounds" )
endif ()

if (SANITIZERS)
  foreach (_san ${SANITIZERS})
    set ( FLAGS "${FLAGS} -fsanitize=${_san}")
  endforeach ()
endif ()

set ( CMAKE_CXX_FLAGS "${FLAGS}" )
set ( CMAKE_EXE_LINKER_FLAGS "${FLAGS}" )

file (GLOB_RECURSE sources c-lib/*.c c-lib/*.cpp)
file (GLOB_RECURSE sources c-lib/*.c c-lib/*.cpp)

include_directories(c-lib c-lib/include)

add_executable(unalzip c-src/unalzip.cpp ${sources})
add_executable(alzip c-src/alzip.cpp ${sources})
