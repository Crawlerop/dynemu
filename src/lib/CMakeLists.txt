add_definitions(-DEXPORT_DLL)

add_library(dynemu libdynemu.cpp sys_emulator.cpp)
add_library(dynemu::Dynemu ALIAS dynemu)
set_target_properties(dynemu PROPERTIES
                      VERSION ${Dynemu_VERSION}
                      SOVERSION ${Dynemu_VERSION_MAJOR})
add_dependencies(dynemu dynarmic mcl)
target_link_libraries(dynemu PUBLIC dynarmic mcl)

target_include_directories(dynemu PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>")
target_compile_features(dynemu PUBLIC cxx_std_20)

# set(EXPORT_HEADERS
#     "libdynemu.h"
#     "sys_emulator.hpp"
# )

# set_property(TARGET dynemu PROPERTY POSITION_INDEPENDENT_CODE 1)
# set_target_properties(dynemu PROPERTIES PUBLIC_HEADER "${EXPORT_HEADERS}")

# # shared and static libraries built from the same object files
# add_library(dynemu_shared SHARED $<TARGET_OBJECTS:dynemu>)
# add_library(dynemu_static STATIC $<TARGET_OBJECTS:dynemu>)

# set_property(TARGET dynemu_shared PROPERTY OUTPUT_NAME dynemu)
# set_property(TARGET dynemu_static PROPERTY OUTPUT_NAME dynemu)

# add_dependencies(dynemu_shared dynarmic mcl)
# target_link_libraries(dynemu_shared PUBLIC dynarmic mcl)

# add_dependencies(dynemu_static dynarmic mcl)
# target_link_libraries(dynemu_static PUBLIC dynarmic mcl)

# install(TARGETS dynemu_shared LIBRARY DESTINATION .)
# install(TARGETS dynemu_static ARCHIVE DESTINATION .)