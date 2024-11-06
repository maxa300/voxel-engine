# Minimum CMake version and project name
cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Define the path to your source files
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/glad.c")
file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/include/*.h")

# Add executable (including headers for IDEs, though they are not compiled)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS}
        include/Camera.h
        include/stb_image.h)

# Find SFML components including audio
find_package(SFML 2.6 COMPONENTS system window graphics audio REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Include directories (add src, include, and Assets if needed)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/Assets
    ${PROJECT_SOURCE_DIR}/textures
)

# Link SFML, GLAD, and OpenGL
target_link_libraries(${PROJECT_NAME} sfml-system sfml-window sfml-graphics sfml-audio OpenGL::GL)

# Copy Assets directory to the build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets
)
