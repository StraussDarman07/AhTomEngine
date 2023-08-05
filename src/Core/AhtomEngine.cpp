#include "AhtomEngine.h"

#include "engine/AhTomPhysicalDevice.h"
#include "engine/AhTomLogicalDevice.h"

#include "vulkan/VkValidationLayerSupport.h"
#include "vulkan/VKExtensionSupport.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

constexpr char ENGINE_NAME[] = "AhTomEngine";



Core::AhTomEngine::AhTomEngine(int major, int minor, int patch) :
	mWindow(nullptr),
	mPhysicalDevice(nullptr),
	mLogicalDevice(nullptr),
	mVersion(Types::Version{major, minor, patch})
{
}

void Core::AhTomEngine::run()
{
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void Core::AhTomEngine::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	mWindow = glfwCreateWindow(WIDTH, HEIGHT, std::format("{} {}", ENGINE_NAME, mVersion.toString()).c_str(), nullptr, nullptr);
}

void Core::AhTomEngine::initVulkan()
{
	createInstance();

	mDebugger.setup(mInstance);

	mPhysicalDevice = std::make_unique<Engine::AhTomPhysicalDevice>(mInstance);

	mLogicalDevice = std::make_unique<Engine::AhTomLogicalDevice>(*mPhysicalDevice.get());
}

void Core::AhTomEngine::mainLoop()
{
	while (!glfwWindowShouldClose(mWindow))
	{
		glfwPollEvents();
	}
}

void Core::AhTomEngine::cleanup()
{
	mLogicalDevice->destroy();

	mDebugger.cleanup(mInstance);

	vkDestroyInstance(mInstance, nullptr);

	glfwDestroyWindow(mWindow);

	glfwTerminate();
}

void Core::AhTomEngine::createInstance()
{
	if (Vulkan::ENABLE_VALIDATION_LAYERS && !Vulkan::checkValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "DEBUG DEMO";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = ENGINE_NAME;
	appInfo.engineVersion = VK_MAKE_VERSION(mVersion.major, mVersion.minor, mVersion.patch);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = Vulkan::getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (Vulkan::ENABLE_VALIDATION_LAYERS)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(Vulkan::VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = Vulkan::VALIDATION_LAYERS.data();

		Vulkan::VkDebugger::populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;

	}

	VkResult result = vkCreateInstance(&createInfo, nullptr, &mInstance);

	if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create instance!");
	}
}
