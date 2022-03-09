# CONFIGURATION
# ------------------------------------------------------------------------------
# CMake version
cmake_minimum_required(VERSION 3.15...3.22)

# Project description
project(fresa VERSION 0.3
              DESCRIPTION "a tiny game engine"
              LANGUAGES CXX)
 
if(UNIX AND NOT APPLE AND NOT EMSCRIPTEN)
    set(LINUX TRUE)
endif()

# C++20 support
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Look for shared libraries at the same folder
if(LINUX)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath='$ORIGIN'")
    set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
endif()

# Emscripten flags
if(EMSCRIPTEN)
    set(EMSCRIPTEN_FLAGS "${EMSCRIPTEN_FLAGS} -s MAX_WEBGL_VERSION=2")
    set(EMSCRIPTEN_FLAGS "${EMSCRIPTEN_FLAGS} --shell-file \"${PROJECT_SOURCE_DIR}/layout.html\"")
    set(CMAKE_EXECUTABLE_SUFFIX .html)

    # Add resources to web bundle
    set(EMSCRIPTEN_FLAGS "${EMSCRIPTEN_FLAGS} --preload-file \"${PROJECT_SOURCE_DIR}/res@\"")
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

# Compile shaders
file(GLOB_RECURSE shader_list CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/res/*.frag" "${PROJECT_SOURCE_DIR}/res/*.vert")
list(LENGTH shader_list shader_list_count)
message(STATUS "[INFO] Found ${shader_list_count} shaders.")

find_program(GLSLC glslc)
if(NOT GLSLC)
    message(STATUS "[WARNING] GLSLC not found, not compiling shaders.")
else()
    foreach(shader IN LISTS shader_list)
    add_custom_command(
        OUTPUT ${shader}.spv
        COMMAND ${GLSLC} ${shader} -o ${shader}.spv
        DEPENDS ${shader})
    list(APPEND spv_shaders ${shader}.spv)
    endForeach()
    add_custom_target(shaders ALL DEPENDS ${spv_shaders})
endif()

# ------------------------------------------------------------------------------

# COMMON
# ------------------------------------------------------------------------------
# Preprocessor macros (GUI is disabled since it is not implemented yet)
add_compile_definitions(
  $<$<CONFIG:DEBUG>:DEBUG>
  $<$<CONFIG:DEBUG>:LOG_LEVEL=5>
  $<$<CONFIG:RELEASE>:DISABLE_GUI>
)

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
    if(LINUX)
        include_directories(${SDL2_INCLUDE_DIRS}) # Temporary fix
    endif()

    set(SDL2_LIBRARY_DIR ${SDL2_INCLUDE_DIR}/../lib)
    message(STATUS "[INFO] SDL2 Libraries ${SDL2_LIBRARIES}")
    link_libraries(${SDL2_LIBRARIES})

    file(GLOB_RECURSE sdl2_library_list CONFIGURE_DEPENDS "${SDL2_LIBRARY_DIR}/libSDL2-2*.dylib" "${SDL2_LIBRARY_DIR}/libSDL2-2*.so.0")
    message(STATUS "[INFO] SDL2 Copy Dynamic Libraries ${sdl2_library_list}")
endif()

# STB
include_directories("${PROJECT_SOURCE_DIR}/deps/stb")
file(GLOB_RECURSE stb_header_list CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/deps/stb/*.h")

# GLM
include_directories("${PROJECT_SOURCE_DIR}/deps/glm")

# imGui
if(NOT EMSCRIPTEN)
    set(IMGUI_DIR "${PROJECT_SOURCE_DIR}/deps/imgui")
    add_library(imgui STATIC)

    target_sources(imgui PRIVATE
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp)
        
    # ImGui needs SDL included using the subdirectory SDL2
    target_include_directories(imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends ${SDL2_INCLUDE_DIR}/SDL2)
    
    target_link_libraries(imgui PUBLIC ${SDL2_LIBRARIES})
endif()

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
target_compile_definitions(fresa-web PUBLIC USE_OPENGL $<$<CONFIG:DEBUG>:DISABLE_GUI>)

if(GLSLC)
add_dependencies(fresa-web shaders)
endif()
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
    if(glew_library_list)
    add_custom_command(TARGET fresa-opengl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${glew_library_list} $<TARGET_FILE_DIR:fresa-opengl>)
    endif()

    file(GLOB_RECURSE opengl_library_list CONFIGURE_DEPENDS "${OPENGL_INCLUDE_DIR}/../lib/libOpenGL.so.0")
    message(STATUS "[INFO] OpenGL Copy Dynamic Libraries ${opengl_library_list}")
    if(opengl_library_list)
    add_custom_command(TARGET fresa-opengl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${opengl_library_list} $<TARGET_FILE_DIR:fresa-opengl>)
    endif()
endif()

# imGui
target_sources(imgui PRIVATE ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)
target_link_libraries(imgui PUBLIC OpenGL::GL)
target_link_libraries(fresa-opengl PUBLIC $<$<NOT:$<CONFIG:RELEASE>>:imgui>)

# Copy dynamic libraries
if(sdl2_library_list)
add_custom_command(TARGET fresa-opengl POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${sdl2_library_list} $<TARGET_FILE_DIR:fresa-opengl>)
endif()

# Add resources to output folder
add_custom_command(TARGET fresa-opengl POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:fresa-opengl>/res)

# Shaders
if(GLSLC)
add_dependencies(fresa-opengl shaders)
endif()
# ------------------------------------------------------------------------------
endif()

if(NOT NO_VULKAN)
# VULKAN TARGET
# ------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/vulkan")
add_executable(fresa-vulkan main.cpp ${source_list} ${game_source_list} ${header_list} ${game_header_list} ${stb_header_list} "${PROJECT_SOURCE_DIR}/deps/vma/include/vk_mem_alloc.h")
target_compile_definitions(fresa-vulkan PUBLIC USE_VULKAN)

# Libraries
find_package(Vulkan REQUIRED)
target_link_libraries(fresa-vulkan PUBLIC Vulkan::Vulkan)
target_include_directories(fresa-vulkan SYSTEM PUBLIC "${PROJECT_SOURCE_DIR}/deps/vma/include") # Warnings disabled

# imGui
target_sources(imgui PRIVATE ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp)
target_link_libraries(imgui PUBLIC Vulkan::Vulkan)
target_link_libraries(fresa-vulkan PUBLIC $<$<NOT:$<CONFIG:RELEASE>>:imgui>)

# Copy dynamic libraries
set(vulkan_library_list ${Vulkan_LIBRARIES})
message(STATUS "[INFO] Vulkan Copy Dynamic Libraries ${vulkan_library_list}")
if(vulkan_library_list)
add_custom_command(TARGET fresa-vulkan POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${vulkan_library_list} $<TARGET_FILE_DIR:fresa-vulkan>)
endif()

if(sdl2_library_list)
add_custom_command(TARGET fresa-vulkan POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different ${sdl2_library_list} $<TARGET_FILE_DIR:fresa-vulkan>)
endif()

# Add resources to output folder
add_custom_command(TARGET fresa-vulkan POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res $<TARGET_FILE_DIR:fresa-vulkan>/res)

# Shaders
if(GLSLC)
add_dependencies(fresa-opengl shaders)
endif()
# ------------------------------------------------------------------------------
endif()

endif()
