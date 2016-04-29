cmake_minimum_required(VERSION 2.8)

project("fragment_playground")



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(GLEW REQUIRED glew)
pkg_search_module(GL REQUIRED gl)

set(src main.cpp init.cpp shaders.cpp render.cpp)
add_executable(${PROJECT_NAME} ${src})

find_package(Threads)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GL_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${GL_LIBRARIES})

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "includedir=${dir}")
endforeach()
