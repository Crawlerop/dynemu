include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if (NOT DEFINED Dynemu_INSTALL_CMAKEDIR)
   set(Dynemu_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/dynemu"
       CACHE STRING "Path to Dynemu CMake files")
endif ()

install(TARGETS dynemu EXPORT Dynemu_Targets
        RUNTIME COMPONENT Dynemu_Runtime
        LIBRARY COMPONENT Dynemu_Runtime
        NAMELINK_COMPONENT Dynemu_Development
        ARCHIVE COMPONENT Dynemu_Development
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(DIRECTORY "${Dynemu_SOURCE_DIR}/include/" "${Dynemu_BINARY_DIR}/include/"
        TYPE INCLUDE
        COMPONENT Dynemu_Development)

if (BUILD_SHARED_LIBS)
    set(type shared)
else ()
    set(type static)
endif ()

install(EXPORT Dynemu_Targets
        DESTINATION "${Dynemu_INSTALL_CMAKEDIR}"
        NAMESPACE Dynemu::
        FILE Dynemu-${type}-targets.cmake
        COMPONENT Dynemu_Development)

write_basic_package_version_file(
    DynemuConfigVersion.cmake
    COMPATIBILITY SameMajorVersion)

install(FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/DynemuConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/DynemuConfigVersion.cmake"
        DESTINATION "${Dynemu_INSTALL_CMAKEDIR}"
        COMPONENT Dynemu_Development)

# TODO: add additional CPack variables here
include(CPack)