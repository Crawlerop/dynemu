add_library(dynemu OBJECT libdynemu.cpp)
add_dependencies(dynemu dynarmic mcl)
target_link_libraries(dynemu PUBLIC dynarmic mcl)

set_property(TARGET dynemu PROPERTY POSITION_INDEPENDENT_CODE 1)

# shared and static libraries built from the same object files
add_library(dynemu_shared SHARED $<TARGET_OBJECTS:dynemu>)
add_library(dynemu_static STATIC $<TARGET_OBJECTS:dynemu>)

add_dependencies(dynemu_shared dynarmic mcl)
target_link_libraries(dynemu_shared PUBLIC dynarmic mcl)

add_dependencies(dynemu_static dynarmic mcl)
target_link_libraries(dynemu_static PUBLIC dynarmic mcl)