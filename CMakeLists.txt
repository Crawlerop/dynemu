cmake_minimum_required(VERSION 3.10.0)
project(Dynemu VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

if (NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
    NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
endif ()

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 OLD)
endif()

# Variables
if (DEFINED Dynemu_SHARED_LIBS)
    set(BUILD_SHARED_LIBS "${Dynemu_SHARED_LIBS}")
endif ()

# 01 - Boost first
set(BOOST_INCLUDEDIR "${PROJECT_SOURCE_DIR}/externals/dynarmic/externals/ext-boost")
find_package(Boost 1.57 REQUIRED)

# 02 - Compile flags
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>" "$<$<C_COMPILER_ID:MSVC>:/std:c++20>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>" "$<$<CXX_COMPILER_ID:MSVC>:/std:c++20>")

# if ( MSVC )
#   set_target_properties( dynarmic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_INSTALL_PREFIX} )
#   set_target_properties( dynarmic PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_INSTALL_PREFIX} )

#   set_target_properties( mcl PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_INSTALL_PREFIX} )
#   set_target_properties( mcl PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_INSTALL_PREFIX} )
# endif ()

add_definitions(-D_CRT_SECURE_NO_WARNINGS -DNOMINMAX)

# 03 - Externals
add_subdirectory(externals)

# 04 - My code
add_subdirectory(src)

# 05 - Generate Include
include(GenerateExportHeader)
generate_export_header(dynemu EXPORT_FILE_NAME include/dynemu/export.h)
target_compile_definitions(
    dynemu PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:DYNEMU_STATIC_DEFINE>")
target_include_directories(
    dynemu PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

# 06 - Package
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
option(Dynemu_INCLUDE_PACKAGING "Include packaging rules for Dynemu" "${is_top_level}")
if (Dynemu_INCLUDE_PACKAGING)
    add_subdirectory(pack)
endif ()

#add_library(dynarmic STATIC IMPORTED)

# set_target_properties(dynarmic PROPERTIES
#     IMPORTED_LOCATION "deps/dynarmic/"
#     IMPORTED_IMPLIB "${DEPENDENCIES_DIR}/allegro/lib/allegro.lib"
#     INTERFACE_INCLUDE_DIRECTORIES "${DEPENDENCIES_DIR}/allegro/include"
# )

# # include_directories("${PROJECT_SOURCE_DIR}/deps/dynarmic/include")

# # target_link_libraries(dynarmic INTERFACE dynarmic::dynarmic)
# # # The dynarmic package's cmake files are helpfully completely silent
# # # so we have to inform the user of its status ourselves
# # if(TARGET dynarmic::dynarmic)
# #     message(STATUS "dynarmic: found him!")
# #     include_directories("${PROJECT_SOURCE_DIR}/deps/dynarmic/include")
# # endif()

#include_directories("${PROJECT_SOURCE_DIR}/ld/ext-boost")

#add_executable(qmgdec qmgtest.cpp)
#target_link_libraries(qmgdec PUBLIC dynarmic::dynarmic)

#add_executable(ifgdec ifgtest.cpp)
#target_link_libraries(ifgdec PUBLIC dynarmic::dynarmic)

# target_sources(sources
#     PUBLIC
#         FILE_SET CXX_MODULES
#         FILES
#             main.ixx
#     PRIVATE
#         main.cpp
# )

# target_compile_features(sources PUBLIC cxx_std_20)
