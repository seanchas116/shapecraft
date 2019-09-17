cmake_minimum_required(VERSION 3.0.0)

project(Lattice)

include(vendor/cmrc/CMakeRC.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)

if(MSVC)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive- /experimental:preprocessor")
else(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()

add_definitions(-D_USE_MATH_DEFINES)

find_package(Qt5Widgets REQUIRED)
find_package(Boost REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(catch2 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/_cmrc/include)

set(LIBRARIES Qt5::Widgets)
file(GLOB_RECURSE SHADERS "src/*.fsh" "src/*.vsh")
cmrc_add_resource_library(${PROJECT_NAME}Resources ${SHADERS})

file(GLOB_RECURSE SOURCES "src/lattice/*.cpp" "src/lattice/*.hpp")
file(GLOB_RECURSE TEST_SOURCES "src/lattice/*.test.cpp")
list(REMOVE_ITEM SOURCES ${TEST_SOURCES})
include_directories(src)

add_library(${PROJECT_NAME}Common STATIC ${SOURCES} ${SHADERS})
target_link_libraries(${PROJECT_NAME}Common ${LIBRARIES} ${PROJECT_NAME}Resources)

if(WIN32)
    set(BUNDLE_TYPE WIN32)
elseif(APPLE)
    set(BUNDLE_TYPE MACOSX_BUNDLE)
endif()

add_executable(${PROJECT_NAME} ${BUNDLE_TYPE} "src/main.cpp")
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}Common)
set_target_properties(${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)

add_executable(${PROJECT_NAME}Test "src/main.test.cpp" ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}Test ${PROJECT_NAME}Common)
