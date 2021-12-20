# CONFIGURATION
# ------------------------------------------------------------------------------
# CMake version
cmake_minimum_required(VERSION 3.15...3.22)

# Project description
project(fresa VERSION 0.3
              DESCRIPTION "a tiny game engine"
              LANGUAGES CXX)
 
if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

# C++20 support
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Look for shared libraries at the same folder
if(LINUX)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath='$ORIGIN'")
endif()

# Emscripten flags
if(EMSCRIPTEN)
    set(EMSCRIPTEN_FLAGS "${EMSCRIPTEN_FLAGS} -s MAX_WEBGL_VERSION=2")
    set(EMSCRIPTEN_FLAGS "${EMSCRIPTEN_FLAGS} --shell-file \"${PROJECT_SOURCE_DIR}/layout.html\"")
    set(CMAKE_EXECUTABLE_SUFFIX .html)

    # Add resources to web bundle
    set(EMSCRIPTEN_FLAGS "${EMSCRIPTEN_FLAGS} --preload-file \"${PROJECT_SOURCE_DIR}/res\"")
endif()
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

file(GLOB_RECURSE game_header_list CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/game/*.h")
file(GLOB_RECURSE game_source_list CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/game/*.cpp")
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

    set(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIRS}/..)
    message(STATUS "[INFO] SDL2 Include Dir ${SDL2_INCLUDE_DIR}")
    include_directories(${SDL2_INCLUDE_DIR})

    set(SDL2_LIBRARY_DIR ${SDL2_INCLUDE_DIR}/../lib)
    message(STATUS "[INFO] SDL2 Libraries ${SDL2_LIBRARIES}")
    link_libraries(${SDL2_LIBRARIES})

    file(GLOB_RECURSE sdl2_library_list CONFIGURE_DEPENDS "${SDL2_LIBRARY_DIR}/libSDL2-2*.dylib" "${SDL2_LIBRARY_DIR}/libSDL2-2*.so.0" "${SDL2_LIBRARY_DIR}/x64/SDL2.dll")
    message(STATUS "[INFO] SDL2 Copy Dynamic Libraries ${sdl2_library_list}")
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
if(APPLE)
    find_library(FoundationLib CoreFoundation)
    link_libraries(${FoundationLib})
endif()
# ------------------------------------------------------------------------------

if(EMSCRIPTEN)
# WEB TARGET
# ------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/web")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EMSCRIPTEN_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EMSCRIPTEN_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EMSCRIPTEN_FLAGS}")

add_executable(fresa-web main.cpp ${source_list} ${game_source_list} ${header_list} ${game_header_list} ${stb_header_list})
target_compile_definitions(fresa-web PUBLIC USE_OPENGL)
# ------------------------------------------------------------------------------
else()

if(NOT NO_OPENGL)
# ------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/opengl")
add_executable(fresa-opengl main.cpp ${source_list} ${game_source_list} ${header_list} ${game_header_list} ${stb_header_list})
target_compile_definitions(fresa-opengl PUBLIC USE_OPENGL)

# Libraries
find_package(OpenGL REQUIRED)
target_link_libraries(fresa-opengl PUBLIC OpenGL::GL)
if(NOT APPLE) # MacOS doesn't need this
    find_package(GLEW REQUIRED)
    message(STATUS "[INFO] GLEW Include Dir ${GLEW_INCLUDE_DIRS}")
    target_include_directories(fresa-opengl PUBLIC ${GLEW_INCLUDE_DIRS})
    message(STATUS "[INFO] GLEW Libraries ${GLEW_LIBRARIES}")
    target_link_libraries(fresa-opengl PUBLIC ${GLEW_LIBRARIES})

    file(GLOB_RECURSE glew_library_list CONFIGURE_DEPENDS "${GLEW_INCLUDE_DIRS}/../lib/libGLEW.so.${GLEW_VERSION_MAJOR}.${GLEW_VERSION_MINOR}")
    message(STATUS "[INFO] GLEW Copy Dynamic Libraries ${glew_library_list}")
    add_custom_command(TARGET fresa-opengl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${glew_library_list} $<TARGET_FILE_DIR:fresa-opengl>)

    file(GLOB_RECURSE opengl_library_list CONFIGURE_DEPENDS "${OPENGL_INCLUDE_DIR}/../lib/libOpenGL.so.0")
    message(STATUS "[INFO] OpenGL Copy Dynamic Libraries ${opengl_library_list}")
    add_custom_command(TARGET fresa-opengl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${opengl_library_list} $<TARGET_FILE_DIR:fresa-opengl>)
endif()

# Copy dynamic libraries
add_custom_command(TARGET fresa-opengl POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${sdl2_library_list} $<TARGET_FILE_DIR:fresa-opengl>)

# Add resources to output folder
add_custom_command(TARGET fresa-opengl POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:fresa-opengl>/res)

# ------------------------------------------------------------------------------
endif()

if(NOT NO_VULKAN)
# VULKAN TARGET
# ------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/vulkan")
add_executable(fresa-vulkan main.cpp ${source_list} ${game_source_list} ${header_list} ${game_header_list} ${stb_header_list} "${PROJECT_SOURCE_DIR}/deps/vma/include/vk_mem_alloc.h")
target_compile_definitions(fresa-vulkan PUBLIC USE_VULKAN)

# Libraries
find_package(Vulkan)
if(Vulkan_FOUND) # Vulkan SDK installed on the system
    target_link_libraries(fresa-vulkan PUBLIC Vulkan::Vulkan)
elseif(DEFINED ENV{VULKAN_SDK}) # CI defined the Vulkan SDK
    message(STATUS "[INFO] Vulkan SDK found at ${ENV{VULKAN_SDK}}")
    set(VULKAN_SDK "${ENV{VULKAN_SDK}}")

    find_path(vulkan_include_list vulkan/vulkan.h PATHS "${VULKAN_SDK}/Include" "${VULKAN_SDK}/include" NO_DEFAULT_PATH)
    message(STATUS "[INFO] Vulkan Include Dirs ${vulkan_include_list}")
    target_include_directories(fresa-vulkan PUBLIC ${vulkan_include_list})

    find_library(vulkan_library_list NAMES vulkan vulkan-1 PATHS "${VULKAN_SDK}/lib" "${VULKAN_SDK}/Lib" NO_DEFAULT_PATH)
    message(STATUS "[INFO] Vulkan Libraries ${vulkan_library_list}")
    add_custom_command(TARGET fresa-vulkan POST_BUILD 
                       COMMAND ${CMAKE_COMMAND} -E copy_if_different ${vulkan_library_list} $<TARGET_FILE_DIR:fresa-vulkan>)
else()
    message(FATAL_ERROR "Vulkan SDK not found")
endif()

target_include_directories(fresa-vulkan SYSTEM PUBLIC "${PROJECT_SOURCE_DIR}/deps/vma/include") # Warnings disabled

# Copy dynamic libraries
add_custom_command(TARGET fresa-vulkan POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${sdl2_library_list} $<TARGET_FILE_DIR:fresa-vulkan>)

# Add resources to output folder
add_custom_command(TARGET fresa-vulkan POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:fresa-vulkan>/res)
# ------------------------------------------------------------------------------
endif()

endif()