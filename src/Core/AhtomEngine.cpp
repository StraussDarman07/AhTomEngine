#include "AhtomEngine.h"
#include "Misc/VulkanHelper.h"

#include <iostream>
#include <set>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "Misc/VulkanDebug.h"
#include "Misc/VulkanDevice.h"

#define GLFW_INCLUDE_VULKAN

#define println(x) std::cout << x << std::endl

#define throwEx(x) throw std::runtime_error(x)

Core::AhTomEngine::AhTomEngine(int versionMajor, int versionMinor, int versionPatch)
{
	this->versionMajor = versionMajor;
	this->versionMinor = versionMinor;
	this->versionPatch = versionPatch;
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

	//Disable creating OpenGL Context
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//Disalbe resizing of the Window
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	std::string windowName = applicationName + " v" + std::to_string(versionMajor) + "." + std::to_string(versionMinor);

	window = glfwCreateWindow(Device::WIDTH, Device::HEIGHT, windowName.c_str(), nullptr, nullptr);
}

void Core::AhTomEngine::initVulkan()
{
	createInstance();
	Debug::setupDebugMessenger(instance, &debugMessenger);
	createSurface();
	pickPhysicalDevice();
	createLogicalDevice();
	createSwapChain();
}

void Core::AhTomEngine::createInstance()
{
	if(Debug::enableValidationLayers && !Debug::checkValidationLayerSupport())
	{
		throwEx("validation layers requested, but not available");
	}
	
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = applicationName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(versionMajor, versionMinor, versionPatch);
	appInfo.pEngineName = applicationName.c_str();
	appInfo.engineVersion = VK_MAKE_VERSION(versionMajor, versionMinor, versionPatch);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if(Debug::enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(Debug::validationLayers.size());
		createInfo.ppEnabledLayerNames = Debug::validationLayers.data();

		Debug::populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debugCreateInfo);
	}
	else
	{
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	const std::vector<const char*> glfwExtensions = getRequiredExtension();

	createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
	createInfo.ppEnabledExtensionNames = glfwExtensions.data();


	if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throwEx("failed to create instance!");
	}

	uint32_t extensionCount = 0;

	//Get the count of the extensions first in order to allocate an vector big enough
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);
	
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	bool allRequiredExternsionExist = true;

	for (int i = 0; i < glfwExtensions.size(); ++i)
	{
		bool foundExtension = false;

		std::string currentExtension = glfwExtensions[i];
		
		for (const auto& extension : extensions)
		{
			if(extension.extensionName == currentExtension)
			{
				foundExtension = true;
				break;
			}
		}

		allRequiredExternsionExist &= foundExtension;
	}

	if(!allRequiredExternsionExist)
	{
		println("Not all required extensions are present");
	}
	else
	{
		println("All required extensions are present");
	}

}



void Core::AhTomEngine::pickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if(deviceCount == 0)
	{
		throwEx("failed to find GPU's with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);

	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for(const VkPhysicalDevice &device : devices)
	{
		if(Device::isDeviceSuitable(device, surface))
		{
			physicalDevice = device;
			break;
		}
	}

	if(physicalDevice == VK_NULL_HANDLE)
	{
		throwEx("failed to find a sutiable GPU");
	}
}


std::vector<const char*> Core::AhTomEngine::getRequiredExtension()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (Debug::enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}



void Core::AhTomEngine::createLogicalDevice()
{
	Device::QueueFamilyIndices indices = Device::findQueueFamilies(physicalDevice, surface);

	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	std::set<uint32_t> unique_queue_families = { indices.graphicsFamily.value(), indices.graphicsFamily.value() };

	float queuePriority = 1.0f;

	for(uint32_t queue_family : unique_queue_families)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;

		queueCreateInfo.pQueuePriorities = &queuePriority;

		queue_create_infos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures device_features{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.pQueueCreateInfos = queue_create_infos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());

	createInfo.pEnabledFeatures = &device_features;
	
	createInfo.enabledExtensionCount = static_cast<uint32_t>(Device::deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = Device::deviceExtensions.data();

	if(Debug::enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(Debug::validationLayers.size());
		createInfo.ppEnabledLayerNames = Debug::validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
	{
		throwEx("failed to create logical device!");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

void Core::AhTomEngine::createSurface()
{
	if(glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
	{
		throwEx("failed to create window surface!");
	}
}

void Core::AhTomEngine::createSwapChain()
{
	const Device::SwapChainSupportDetails swap_chain_support = Device::querySwapChainSupport(physicalDevice, surface);

	VkSurfaceFormatKHR surface_format = Device::chooseSwapSurfaceFormat(swap_chain_support.formats);

	VkPresentModeKHR present_mode = Device::chooseSwapPresentMode(swap_chain_support.present_modes);

	VkExtent2D extent = Device::chooseSwapExtent(swap_chain_support.capabilities);

	uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;

	if(swap_chain_support.capabilities.maxImageArrayLayers > 0 && image_count > swap_chain_support.capabilities.maxImageCount)
	{
		image_count = swap_chain_support.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = surface;

	create_info.minImageCount = image_count;
	create_info.imageFormat = surface_format.format;
	create_info.imageColorSpace = surface_format.colorSpace;
	create_info.imageExtent = extent;
	create_info.imageArrayLayers = 1;
	//Use VK_IMAGE_USAGE_TRANSFER_DST_BIT if post-processing is needed
	create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	Device::QueueFamilyIndices indices = Device::findQueueFamilies(physicalDevice, surface);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = 2;
		create_info.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		create_info.queueFamilyIndexCount = 0; // Optional
		create_info.pQueueFamilyIndices = nullptr; // Optional
	}

	create_info.preTransform = swap_chain_support.capabilities.currentTransform;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	create_info.presentMode = present_mode;
	create_info.clipped = VK_TRUE;

	create_info.oldSwapchain = VK_NULL_HANDLE;

	if(vkCreateSwapchainKHR(device, &create_info, nullptr, &swapChain) != VK_SUCCESS)
	{
		throwEx("failed to create swap chain");
	}
}


void Core::AhTomEngine::mainLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
}

void Core::AhTomEngine::cleanup()
{
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	
	vkDestroyDevice(device, nullptr);
	
	if(Debug::enableValidationLayers)
	{
		Misc::DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	vkDestroySurfaceKHR(instance, surface, nullptr);
	
	vkDestroyInstance(instance, nullptr);
	
	glfwDestroyWindow(window);

	glfwTerminate();
}
