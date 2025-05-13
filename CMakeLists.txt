cmake_minimum_required(VERSION 3.20)

project(
    kepler
    VERSION 1.0
    LANGUAGES CXX
)

set(MAIN_SOURCE "main.cpp")

file(GLOB_RECURSE SRC_SOURCES "src/*.cpp")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(GLU REQUIRED)

add_executable(kepler ${MAIN_SOURCE} ${SRC_SOURCES})

target_link_libraries(kepler PRIVATE glfw)
target_link_libraries(kepler PRIVATE OpenGL)
target_link_libraries(kepler PRIVATE glm::glm)
target_link_libraries(kepler PRIVATE GLU)