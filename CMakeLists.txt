cmake_minimum_required(VERSION 3.10)
project(MyRenderingEngine CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_BUILD_TYPE Debug)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLM REQUIRED glm)
pkg_check_modules(GLFW REQUIRED glfw3)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# pkg_check_modules(FREEGLUT REQUIRED freeglut)

# Include directories
include_directories(
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    # ${FREEGLUT_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/lib
)

# Manually specify header files (for IDE indexing)
set(HEADERS
    ./lib/assetLoaders/ImageLoader.hpp
    ./lib/assetLoaders/ModelLoader.hpp
    ./lib/assetLoaders/ObjLoader.hpp
    ./lib/assetLoaders/STLLoader.hpp
    ./lib/glWrappers/FrameBuffer.hpp
    ./lib/glWrappers/FrameBufferManager.hpp
    ./lib/glWrappers/Attachment.hpp
    ./lib/glWrappers/AttachmentManager.hpp
    ./lib/glWrappers/Texture.hpp
    ./lib/glWrappers/TextureManager.hpp
    ./lib/glWrappers/VAOModel.hpp
    ./lib/pipeline/BasePipline.hpp
    ./lib/pipeline/DefferedGeometry.hpp
    ./lib/pipeline/DefferedLighting.hpp
    ./lib/pipeline/FinalComposition.hpp
    ./lib/pipeline/ForwardRendering.hpp
    ./lib/pipeline/PostProcessing.hpp
    ./lib/sceneModel/Camera.hpp
    ./lib/sceneModel/Model.hpp
    ./lib/sceneModel/ModelManager.hpp
    ./lib/sceneModel/SceneGraph.hpp
    ./lib/sceneModel/SceneNode.hpp
    ./lib/shaderManager/Shader.hpp
    ./lib/shaderManager/ShaderProgram.hpp
    ./lib/shaderManager/ShaderProgramManager.hpp
    ./lib/ecco/EccoObject.hpp
    ./lib/ecco/EccoOutcome.hpp
    ./lib/appdelegate/AppDelegate.hpp
)

# Define source files for the static library
set(SOURCES
    ./lib/ecco/EccoObject.cpp
    ./lib/sceneModel/SceneNode.cpp
    ./lib/sceneModel/SceneGraph.cpp
    ./lib/ecco/EccoManager.cpp
    ./lib/glWrappers/FrameBufferManager.cpp
    ./lib/glWrappers/AttachmentManager.cpp
    ./lib/appdelegate/AppDelegate.cpp
    ./lib/shaderManager/Shader.cpp
    ./lib/shaderManager/ShaderProgram.cpp
)

# Set build directory
# set(BUILD_DIR "${CMAKE_SOURCE_DIR}/../build")
# if (NOT EXISTS ${BUILD_DIR})
#     file(MAKE_DIRECTORY ${BUILD_DIR})
# endif()
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR})
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

# Create static library target
add_library(RenderingEngine STATIC ${SOURCES})

# Link libraries to the static library
target_link_libraries(RenderingEngine
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLM_LIBRARIES}
    ${GLFW_LIBRARIES}
    # ${FREEGLUT_LIBRARIES}
)

# Define the executable for pullIn.cpp
add_executable(test ./lib/pullIn.cpp)

# Link the executable with the static library
target_link_libraries(test RenderingEngine)

# Output message
message(STATUS "CMake configured successfully!")
