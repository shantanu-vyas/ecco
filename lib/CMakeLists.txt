cmake_minimum_required(VERSION 3.10)
project(MyRenderingEngine CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER clang++)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLM REQUIRED glm)
pkg_check_modules(GLFW REQUIRED glfw3)

# Include directories
include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/lib
)

# Collect all header and source files
file(GLOB_RECURSE HEADERS "lib/**/*.hpp")
file(GLOB_RECURSE SOURCES "lib/**/*.set")

# Set build directory
set(BUILD_DIR "${CMAKE_SOURCE_DIR}/../build")
if (NOT EXISTS ${BUILD_DIR})
    file(MAKE_DIRECTORY ${BUILD_DIR})
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

# Create library target
add_library(RenderingEngine STATIC ${SOURCES} ${HEADERS})



# Link libraries
target_link_libraries(RenderingEngine
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLM_LIBRARIES}
    ${GLFW_LIBRARIES}
)

# Output message
message(STATUS "CMake configured successfully!")
