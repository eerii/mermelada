# CLANG PLUGIN
# ------------------------------------------------------------------------------
# Find Clang
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Clang REQUIRED)

# LLVM CMake modules
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)
include(HandleLLVMOptions)

# Create the reflection plugin
add_library(reflection MODULE reflection_plugin.cpp)

# Link the plugin against the Clang libraries
target_link_libraries(reflection ${CLANG_LIBS})

# Include the Clang and LLVM headers
target_include_directories(reflection PRIVATE ${CLANG_INCLUDE_DIRS} ${LLVM_INCLUDE_DIRS})
# ------------------------------------------------------------------------------