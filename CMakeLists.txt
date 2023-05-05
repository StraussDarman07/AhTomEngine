cmake_minimum_required(VERSION 3.26)

set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project (AhTomEngine VERSION 1.0.0)

include_directories(src/Core)

#Source Files
set(ALL_SOURCE_FILES 
    "main.cpp"    
)

set(ENGINE_MODULE_FILES 
	"src/Core/AhtomEngine.ixx"
    "src/Core/Types/EngineTypes.ixx" 
    "src/Core/Types/AhtomDevice.ixx" 
    "src/Core/Types/AhtomInstance.ixx"  
    "src/Core/Types/AhtomWindow.ixx"
    "src/Core/Misc/VulkanDebug.ixx"
    "src/Core/Misc/VulkanHelper.ixx"
    "src/Core/Misc/VulkanDevice.ixx" "src/Core/Types/AhtomSwapChain.ixx")

message("Modules: ${ENGINE_MODULE_FILES}")
configure_file(main.h.in main.h)
add_executable(${PROJECT_NAME} ${ALL_SOURCE_FILES})

target_sources(${PROJECT_NAME}
  PUBLIC
    FILE_SET modules TYPE CXX_MODULES FILES
    ${ENGINE_MODULE_FILES}
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
    add_custom_command(TARGET AhTomEngine POST_BUILD
                                COMMAND cmd /c ${CMAKE_SOURCE_DIR}/compile_shader.bat
                                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
else()
    add_custom_command(TARGET AhTomEngine POST_BUILD
                                COMMAND ${CMAKE_SOURCE_DIR}/compile_shader.sh
                                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

add_custom_command(TARGET AhTomEngine POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy_directory
                            ${CMAKE_SOURCE_DIR}/shaders/ $<TARGET_FILE_DIR:AhTomEngine>/shaders/)

add_custom_command(TARGET AhTomEngine POST_BUILD
                            COMMAND ${CMAKE_COMMAND} -E copy_directory
                            ${CMAKE_SOURCE_DIR}/textures/ $<TARGET_FILE_DIR:AhTomEngine>/textures/)