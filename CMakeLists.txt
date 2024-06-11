cmake_minimum_required(VERSION 3.10)
project(OpenGLExample)

set(CMAKE_CXX_STANDARD 17)  # Use C++17 standard
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Set the paths to GLEW and GLFW
set(GLEW_DIR "C:/Libraries/glew-2.1.0")
set(GLFW_DIR "C:/Libraries/glfw-3.4.bin.WIN64")

# Include directories for GLEW and GLFW
include_directories(${GLEW_DIR}/include ${GLFW_DIR}/include ${OPENGL_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/external/nlohmann_json/include)

# Library directories for GLEW and GLFW
link_directories(${GLEW_DIR}/lib/Release/x64 ${GLFW_DIR}/lib-vc2019)

# Add source files
set(SOURCES
    src/main.cpp
    src/window.cpp
    src/shader.cpp
    src/renderer.cpp
    src/file/pathutils.cpp
    src/file/jsonloader.cpp
)

# Add executable
add_executable(OpenGLExample ${SOURCES})

# Link libraries
target_link_libraries(OpenGLExample ${OPENGL_LIBRARIES} glew32 glfw3)

# Copy DLLs to the output directory
add_custom_command(TARGET OpenGLExample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${GLEW_DIR}/bin/Release/x64/glew32.dll"
    $<TARGET_FILE_DIR:OpenGLExample>)
add_custom_command(TARGET OpenGLExample POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${GLFW_DIR}/lib-vc2019/glfw3.dll"
    $<TARGET_FILE_DIR:OpenGLExample>)