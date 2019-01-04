project( voxstuff )
cmake_minimum_required( VERSION 3.2 )

#set(SDL2_PATH "extern/sdl")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${voxstuff_SOURCE_DIR}/cmake")

set (CMAKE_CXX_STANDARD 14)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories( "extern/eigen" )
include_directories( "src" )

file (GLOB_RECURSE blah RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.cpp" "src/*.hpp")

foreach(source IN LISTS blah)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

add_executable( main ${blah} )

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(main ${SDL2_LIBRARY})

add_definitions(-DGLEW_STATIC)
set(glew-cmake_BUILD_SHARED OFF)
set(glew-cmake_BUILD_MULTI_CONTEXT OFF)
add_subdirectory(extern/glew)
target_link_libraries(main libglew_static)
include_directories(extern/glew/include)
