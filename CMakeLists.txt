# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
cmake_minimum_required (VERSION 3.9)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project (AhTomEngine VERSION 1.0.0)

#includes
include_directories(src)

#Source Files
file(GLOB_RECURSE SOURCES 
    src/*.h
    src/*.cpp
)

set(ALL_SOURCE_FILES ${SOURCES})

configure_file(main.h.in main.h)
add_executable(${PROJECT_NAME} "main.cpp" ${ALL_SOURCE_FILES})


# GLFW3 Include
set(GLFW3 "${PROJECT_SOURCE_DIR}/submodules/glfw")
add_subdirectory("${GLFW3}")
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

# Vulkan Include
find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)

# GLM Include
set(GLM_DIR "${PROJECT_SOURCE_DIR}/submodules/glm/cmake/glm")
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLM_LIBRARY_DIRS})


target_include_directories(${PROJECT_NAME} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

add_custom_command(TARGET AhTomEngine PRE_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy_directory
                            ${CMAKE_SOURCE_DIR}/shaders/ $<TARGET_FILE_DIR:AhTomEngine>/shaders/)