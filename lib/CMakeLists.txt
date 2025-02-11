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

# Manually specify source and header files
set(HEADERS
    ./assetLoaders/ImageLoader.hpp
    ./assetLoaders/ModelLoader.hpp
    ./assetLoaders/ObjLoader.hpp
    ./assetLoaders/STLLoader.hpp
    ./glWrappers/FrameBuffer.hpp
    ./glWrappers/FrameBufferManager.hpp
    ./glWrappers/Texture.hpp
    ./glWrappers/TextureManager.hpp
    ./glWrappers/VAOModel.hpp
    ./pipeline/BasePipline.hpp
    ./pipeline/DefferedGeometry.hpp
    ./pipeline/DefferedLighting.hpp
    ./pipeline/FinalComposition.hpp
    ./pipeline/ForwardRendering.hpp
    ./pipeline/PostProcessing.hpp
    ./sceneModel/Camera.hpp
    ./sceneModel/Model.hpp
    ./sceneModel/ModelManager.hpp
    ./sceneModel/SceneGraph.hpp
    ./sceneModel/SceneNode.hpp
    ./shaderManager/Shader.hpp
    ./shaderManager/ShaderProgram.hpp
    ./shaderManager/ShaderProgramManager.hpp
    ./ecco/EccoObject.hpp
    ./ecco/EccoOutcome.hpp
)

set(SOURCES
    ./pullIn.cpp
    ./sceneModel/SceneNode.cpp
    ./sceneModel/SceneGraph.cpp
    ./ecco/EccoManager.cpp
    ./glWrappers/FrameBufferManager.cpp
)

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
    # ${FREEGLUT_LIBRARIES}
)

# Output message
message(STATUS "CMake configured successfully!")
