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
    ./assetLoaders/ImageLoader.hpp
    ./assetLoaders/ModelLoader.hpp
    ./assetLoaders/ObjLoader.hpp
    ./assetLoaders/STLLoader.hpp
    ./glWrappers/FrameBuffer.hpp
    ./glWrappers/FrameBufferManager.hpp
    ./glWrappers/Attachment.hpp
    ./glWrappers/AttachmentManager.hpp
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
    ./appdelegate/AppDelegate.hpp
)

# Define source files for the static library
set(SOURCES
    ./ecco/EccoObject.cpp
    ./sceneModel/SceneNode.cpp
    ./sceneModel/SceneGraph.cpp
    ./ecco/EccoManager.cpp
    ./glWrappers/FrameBufferManager.cpp
    ./glWrappers/AttachmentManager.cpp
    ./appdelegate/AppDelegate.cpp
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
add_executable(test ./pullIn.cpp)

# Link the executable with the static library
target_link_libraries(test RenderingEngine)

# Output message
message(STATUS "CMake configured successfully!")
