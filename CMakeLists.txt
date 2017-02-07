project( voxstuff )
cmake_minimum_required( VERSION 3.0 )

set(SDL2_PATH "extern/sdl")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${voxstuff_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories( "extern/eigen" )

file (GLOB blah "src/*.cpp" "src/*.hpp")

add_executable( main ${blah} )

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(main ${SDL2_LIBRARY})