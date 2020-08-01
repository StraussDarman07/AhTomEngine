// TestProject.cpp : Defines the entry point for the application.
//
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include "main.h"
#include "src/Core/AhtomEngine.h"

int main()
{
    std::cout << "AhTom Engine Version " << ENGINE_VERSION_MAJOR << "."
        << ENGINE_VERSION_MINOR << std::endl;

    Core::AhTomEngine* engine = new Core::AhTomEngine(ENGINE_VERSION_MAJOR, ENGINE_VERSION_MINOR, ENGINE_VERSION_PATCH);

    try {

        engine->run();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
