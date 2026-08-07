cmake_minimum_required(VERSION 3.10)

project("VQ_Engine" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -gdwarf-4 -O0")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)

add_subdirectory(3rdparty/glm)
add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/glad)
add_subdirectory(3rdparty/googletest)

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
add_subdirectory(${CMAKE_SOURCE_DIR}/examples)
