# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
cmake_minimum_required (VERSION 3.9)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project (AhTomEngine VERSION 1.0.0)

set(ALL_SOURCE_FILES ${SOURCES})

configure_file(main.h.in main.h)
add_executable(${PROJECT_NAME} "main.cpp"

    "src/core/AhTomEngine.h"
    "src/core/AhTomEngine.cpp"
    "src/types/AhTomTypes.h"
    
    "src/core/vulkan/VkValidationLayerSupport.h" 
    "src/core/vulkan/VKExtensionSupport.h" 
    "src/core/vulkan/VkDebugSupport.h" "src/core/vulkan/VkDebugSupport.cpp" 
    
    "src/core/engine/AhTomPhysicalDevice.h" "src/core/engine/AhTomPhysicalDevice.cpp" 
    "src/core/engine/AhTomLogicalDevice.h" "src/core/engine/AhTomLogicalDevice.cpp" "src/core/engine/AhTomWindowSurface.h" "src/core/engine/AhTomWindowSurface.cpp")

# Include paths
target_include_directories(${PROJECT_NAME} PRIVATE
    src
)


# GLFW3 Include
set(GLFW3 "${PROJECT_SOURCE_DIR}/submodules/glfw")
add_subdirectory("${GLFW3}")
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

# Vulkan Include
find_package(Vulkan REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)

# GLM Include
set(glm_DIR "${PROJECT_SOURCE_DIR}/submodules/glm/cmake/glm")
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${GLM_LIBRARY_DIRS})

set(STB "${PROJECT_SOURCE_DIR}/submodules/stb")
include_directories("${STB}")


target_include_directories(${PROJECT_NAME} PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )


if (WIN32)                          
    add_custom_command(TARGET AhTomEngine PRE_BUILD
                                COMMAND cmd /c ${CMAKE_SOURCE_DIR}/compile_shader.bat
                                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
else()
    add_custom_command(TARGET AhTomEngine PRE_BUILD
                                COMMAND ${CMAKE_SOURCE_DIR}/compile_shader.sh
                                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

add_custom_command(TARGET AhTomEngine PRE_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy_directory
                            ${CMAKE_SOURCE_DIR}/shaders/ $<TARGET_FILE_DIR:AhTomEngine>/shaders/)

add_custom_command(TARGET AhTomEngine PRE_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy_directory
                            ${CMAKE_SOURCE_DIR}/textures/ $<TARGET_FILE_DIR:AhTomEngine>/textures/)