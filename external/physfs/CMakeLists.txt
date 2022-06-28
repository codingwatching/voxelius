cmake_minimum_required(VERSION 3.10)
project(physfs LANGUAGES C VERSION 3.0.2)

add_library(physfs STATIC
    "${CMAKE_CURRENT_LIST_DIR}/src/physfs_archiver_7z.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/physfs_archiver_dir.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/physfs_archiver_zip.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/physfs_byteorder.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/physfs_platform_posix.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/physfs_platform_unix.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/physfs_platform_windows.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/physfs_unicode.c"
    "${CMAKE_CURRENT_LIST_DIR}/src/physfs.c")
target_compile_definitions(physfs PRIVATE PHYSFS_SUPPORTS_ZIP=1 PHYSFS_SUPPORTS_7Z=1)
target_include_directories(physfs PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
target_include_directories(physfs PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src")

# WIN32: thread API is present in <Windows.h>
if(UNIX AND NOT WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(physfs PUBLIC Threads::Threads)
endif()

# Make sure we don't reference non-existent code.
# What the fuck? Why not use ifdef? Annoying!
target_compile_definitions(physfs PRIVATE
    PHYSFS_SUPPORTS_GRP=0
    PHYSFS_SUPPORTS_WAD=0
    PHYSFS_SUPPORTS_HOG=0
    PHYSFS_SUPPORTS_MVL=0
    PHYSFS_SUPPORTS_QPAK=0
    PHYSFS_SUPPORTS_SLB=0
    PHYSFS_SUPPORTS_ISO9660=0
    PHYSFS_SUPPORTS_VDF=0)
