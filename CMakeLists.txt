cmake_minimum_required(VERSION 3.10)
project(voxelius LANGUAGES CXX VERSION 0.0.1)

cmake_policy(SET CMP0079 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

#add_subdirectory(external/bitsery)
#add_subdirectory(external/enet)
add_subdirectory(external/entt)
#add_subdirectory(external/glad)
#add_subdirectory(external/glfw)
add_subdirectory(external/glm)
#add_subdirectory(external/imgui)
add_subdirectory(external/spdlog)
#add_subdirectory(external/stb)
add_subdirectory(external/thread_pool)
add_subdirectory(external/toml)

add_subdirectory(common)
add_subdirectory(game)
