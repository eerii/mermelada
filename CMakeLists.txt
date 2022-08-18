# CONFIGURATION
# ------------------------------------------------------------------------------
# CMake version
cmake_minimum_required(VERSION 3.16...3.24)

# Project description
project(fresa VERSION 0.5
              DESCRIPTION "a tiny game engine"
              LANGUAGES CXX C)

# C++20 support
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output bin directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# Use static over shared libraries
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
# ------------------------------------------------------------------------------

# COMPILER
# ------------------------------------------------------------------------------
# Preprocessor definitions (conditional compilation)
add_compile_definitions(
  $<$<CONFIG:DEBUG>:FRESA_ENABLE_TESTS>
  FRESA_CONFIG_FILE="config.h"
)
# ------------------------------------------------------------------------------

# INCLUDE
# ------------------------------------------------------------------------------
# Header directories
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list "${PROJECT_SOURCE_DIR}/fresa/*.h" "${PROJECT_SOURCE_DIR}/game/*.h")
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

# Get header directories list
HEADER_DIRECTORIES(header_dir_list)
list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories")

# Get the source and header files
file(GLOB_RECURSE header_list CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/fresa/*.h"
        "${PROJECT_SOURCE_DIR}/game/*.h")
file(GLOB_RECURSE source_list CONFIGURE_DEPENDS
        "${PROJECT_SOURCE_DIR}/fresa/*.cpp"
        "${PROJECT_SOURCE_DIR}/game/*.cpp"
        "${PROJECT_SOURCE_DIR}/main.cpp")
list(LENGTH header_list header_list_count)
list(LENGTH source_list source_list_count)
message(STATUS "[INFO] Found ${header_list_count} header files (.h) and ${source_list_count} source files (.cpp)")
# ------------------------------------------------------------------------------

# TARGETS
# ------------------------------------------------------------------------------
# Add targets
add_executable(fresa ${source_list} ${header_list})
target_include_directories(fresa PUBLIC ${header_dir_list})

# Preprocessor macros
target_compile_definitions(fresa PUBLIC $<$<CONFIG:DEBUG>:FRESA_DEBUG>)
# ------------------------------------------------------------------------------

# REFLECTION
# ------------------------------------------------------------------------------
# Find python
find_package(Python3 COMPONENTS Interpreter REQUIRED)

# Reflect the project using the reflection_gen.py script
MACRO(REFLECT_HEADERS)
    FOREACH(header ${header_list})
        GET_FILENAME_COMPONENT(header_name ${header} NAME)
        file(READ ${header} file_contents)
        string(FIND "${file_contents}" "#include \"reflection_${header_name}\"" has_reflection)
        if(NOT ${has_reflection} EQUAL -1)
            message(STATUS "[INFO] Reflecting ${header_name}")
            add_custom_command(
                OUTPUT ${PROJECT_SOURCE_DIR}/generated/reflection/reflection_${header_name}
                COMMAND ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/fresa/reflection/reflection_gen.py ${header}
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                DEPENDS ${header}
            )
            add_custom_target(
                ${header_name}
                DEPENDS ${PROJECT_SOURCE_DIR}/generated/reflection/reflection_${header_name}
            )
            add_dependencies(fresa ${header_name})
        endif()
    ENDFOREACH()
ENDMACRO()

if(${Python3_FOUND})
    REFLECT_HEADERS()
else()
    message(STATUS "[INFO] Python3 not found, reflection targets will not be generated")
endif()

# Merge reflection headers into header list
target_include_directories(fresa PUBLIC "${PROJECT_SOURCE_DIR}/generated/reflection")
# ------------------------------------------------------------------------------

# LIBRARIES
# ------------------------------------------------------------------------------
# fmt
target_include_directories(fresa PUBLIC ${PROJECT_SOURCE_DIR}/deps/fmt/include)
target_sources(fresa PUBLIC ${PROJECT_SOURCE_DIR}/deps/fmt/src/format.cc)
# range-v3 (used while compilers get support of c++20 std::ranges)
add_subdirectory(deps/ranges)
target_link_libraries(fresa PUBLIC range-v3::range-v3)
# jthread (used while compilers get support of c++20 std::jthread)
target_include_directories(fresa PUBLIC ${PROJECT_SOURCE_DIR}/deps/jthread/source)
# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(deps/glfw)
target_link_libraries(fresa PUBLIC glfw)
# glad
set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/deps/glad/")
add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)
glad_add_library(glad_vulkan LOADER LANGUAGE CXX API vulkan:core=1.1)
target_link_libraries(fresa PUBLIC glad_vulkan)
# ------------------------------------------------------------------------------

# INSTALL
# ------------------------------------------------------------------------------
# Install directory
set(INSTALL_DIR ${PROJECT_SOURCE_DIR}/bin)

# Install targets
install(TARGETS fresa DESTINATION ${INSTALL_DIR})

# Create package
set(PACKAGE_NAME "fresa")
set(PACKAGE_VERSION "0.5")
set(PACKAGE_DESCRIPTION "a tiny game engine")

set(CPACK_MONOLITHIC_INSTALL 1)
include(CPack)
# ------------------------------------------------------------------------------