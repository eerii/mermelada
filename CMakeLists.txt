# CONFIGURATION
# ------------------------------------------------------------------------------
# CMake version
cmake_minimum_required(VERSION 3.12...3.22)

# CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/deps")

# Project description
project(fresa VERSION 0.3
              DESCRIPTION "a tiny game engine"
              LANGUAGES CXX)

# C++20 support
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Emscripten flags
if(EMSCRIPTEN)
    set(EMSCRIPTEN_FLAGS "${EMSCRIPTEN_FLAGS} -s MAX_WEBGL_VERSION=2")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
endif()
# ------------------------------------------------------------------------------

# OUTPUT
# ------------------------------------------------------------------------------
# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# Delete old resources
file(GLOB OBSOLETE_FILES "${PROJECT_SOURCE_DIR}/bin/res")
if (OBSOLETE_FILES)
    file (REMOVE_RECURSE ${OBSOLETE_FILES})
endif()

# Add new resources
file(COPY "${PROJECT_SOURCE_DIR}/res" DESTINATION "${PROJECT_SOURCE_DIR}/bin")
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

# Include search directories (Common)
HEADER_DIRECTORIES(header_dir_list)
list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories.")
include_directories(${header_dir_list})

# Get the source and header files
file(GLOB_RECURSE header_list CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/fresa/*.h")
file(GLOB_RECURSE source_list CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/fresa/*.cpp")
# ------------------------------------------------------------------------------

# COMMON
# ------------------------------------------------------------------------------
# Preprocessor macros
add_compile_definitions(DEBUG)
add_compile_definitions(DISABLE_GUI) # Not implemented yet

# SDL2 (The include dir has /.. so it can be included explicitly using #include <SDL2/SDL.h>)
if(EMSCRIPTEN)
    set(EMSCRIPTEN_FLAGS "${EMSCRIPTEN_FLAGS} -s USE_SDL=2")
    include_directories(${SDL2_INCLUDE_DIRS})
    link_libraries(${SDL2_LIBRARIES})
else()
    find_package(SDL2 REQUIRED)
    set(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIRS}/.. CACHE BOOL "" FORCE)
    message(STATUS "[INFO] SDL2 Include Dir ${SDL2_INCLUDE_DIR}")
    include_directories(${SDL2_INCLUDE_DIR})
    link_libraries(${SDL2_LIBRARIES})
endif()

# STB
include_directories("${PROJECT_SOURCE_DIR}/deps/stb")
file(GLOB_RECURSE stb_header_list CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/deps/stb/*.h")

# GLM
include_directories("${PROJECT_SOURCE_DIR}/deps/glm")

# SpirV Cross
add_subdirectory("deps/spirv-cross")
include_directories("${PROJECT_SOURCE_DIR}/deps/spirv_cross")
link_libraries(spirv-cross-core spirv-cross-glsl spirv-cross-cpp spirv-cross-reflect spirv-cross-msl spirv-cross-hlsl spirv-cross-util spirv-cross-c)

# CoreFoundation (Only MacOS)
if (APPLE)
    find_library(FoundationLib CoreFoundation)
    link_libraries(${FoundationLib})
endif()
# ------------------------------------------------------------------------------

if(EMSCRIPTEN)

# WEB TARGET
# ------------------------------------------------------------------------------
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EMSCRIPTEN_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EMSCRIPTEN_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EMSCRIPTEN_FLAGS}")

add_executable(fresa-web main.cpp ${source_list} ${header_list} ${stb_header_list})
target_compile_definitions(fresa-web PUBLIC USE_OPENGL)
# ------------------------------------------------------------------------------

else()

# OPENGL TARGET
# ------------------------------------------------------------------------------
add_executable(fresa-opengl main.cpp ${source_list} ${header_list} ${stb_header_list})
target_compile_definitions(fresa-opengl PUBLIC USE_OPENGL)

# Libraries
find_package(OpenGL REQUIRED)
target_link_libraries(fresa-opengl PUBLIC OpenGL::GL)
# ------------------------------------------------------------------------------

# VULKAN TARGET
# ------------------------------------------------------------------------------
add_executable(fresa-vulkan main.cpp ${source_list} ${header_list} ${stb_header_list} "${PROJECT_SOURCE_DIR}/deps/vma/include/vk_mem_alloc.h")
target_compile_definitions(fresa-vulkan PUBLIC USE_VULKAN)

# Libraries
target_include_directories(fresa-vulkan PRIVATE "${PROJECT_SOURCE_DIR}/deps/vma/include")
target_link_libraries(fresa-vulkan PRIVATE "${PROJECT_SOURCE_DIR}/deps/vulkan.framework")
# ------------------------------------------------------------------------------

endif()