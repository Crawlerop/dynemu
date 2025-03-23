cmake_minimum_required(VERSION 3.10.0)
project(qmgdec VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 OLD)
endif()

# 01 - Boost first
set(BOOST_INCLUDEDIR "${PROJECT_SOURCE_DIR}/externals/dynarmic/externals/ext-boost")
find_package(Boost 1.57 REQUIRED)

# 02 - Compile flags
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_compile_options("$<$<C_COMPILER_ID:Clang>:-DNOMINMAX>")
add_compile_options("$<$<CXX_COMPILER_ID:Clang>:-DNOMINMAX>")


# 03 - Dynarmic
set(DYNARMIC_TESTS OFF CACHE BOOL "")
set(DYNARMIC_FRONTENDS "A32;A64" CACHE STRING "")
set(DYNARMIC_USE_PRECOMPILED_HEADERS ON CACHE BOOL "")
add_subdirectory("${PROJECT_SOURCE_DIR}/externals/dynarmic")

# 04 - My code
add_subdirectory(src)

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
