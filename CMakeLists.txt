# CONFIGURATION
# ------------------------------------------------------------------------------
# CMake version
cmake_minimum_required(VERSION 3.15...3.24)

# Project description
project(fresa VERSION 0.4
              DESCRIPTION "a tiny game engine"
              LANGUAGES CXX)

# C++20 support
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# ------------------------------------------------------------------------------

# COMPILER
# ------------------------------------------------------------------------------
# Preprocessor definitions (conditional compilation)
add_compile_definitions(
  $<$<CONFIG:DEBUG>:ENABLE_TESTS>
)
# ------------------------------------------------------------------------------

# INCLUDE
# ------------------------------------------------------------------------------
# Header directories
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.h)
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
file(GLOB_RECURSE header_list CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/fresa/*.h")
file(GLOB_RECURSE source_list CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/fresa/*.cpp" "${PROJECT_SOURCE_DIR}/main.cpp")
list(LENGTH header_list header_list_count)
list(LENGTH source_list source_list_count)
message(STATUS "[INFO] Found ${header_list_count} header files (.h) and ${source_list_count} source files (.cpp)")
# ------------------------------------------------------------------------------

# TARGET
# ------------------------------------------------------------------------------
# Add target
add_executable(fresa ${source_list} ${header_list})
target_include_directories(fresa PUBLIC ${header_dir_list})

# Preprocessor macros
target_compile_definitions(fresa PUBLIC $<$<CONFIG:DEBUG>:DEBUG>)
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
# ------------------------------------------------------------------------------

# INSTALL
# ------------------------------------------------------------------------------
# Install directory
set(INSTALL_DIR ${PROJECT_SOURCE_DIR}/bin)

# Install targets
install(TARGETS fresa DESTINATION ${INSTALL_DIR})

# Create package
set(PACKAGE_NAME "fresa")
set(PACKAGE_VERSION "0.4")
set(PACKAGE_DESCRIPTION "a tiny game engine")

set(CPACK_MONOLITHIC_INSTALL 1)
include(CPack)
# ------------------------------------------------------------------------------