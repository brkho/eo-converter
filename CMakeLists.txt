cmake_minimum_required(VERSION 3.0)
project(eo-converter)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(lib/assimp)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
endif()

include_directories(include/
                    lib/assimp/include/)

file(GLOB_RECURSE PROJECT_HEADERS include/*.h)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cc)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("include" FILES ${PROJECT_HEADERS})
source_group("src" FILES ${PROJECT_SOURCES})
source_group("lib" FILES ${VENDORS_SOURCES})

add_definitions(-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_CONFIGS} ${VENDORS_SOURCES})

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

target_link_libraries(${PROJECT_NAME} assimp)
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY /${CMAKE_BINARY_DIR}/bin/)
