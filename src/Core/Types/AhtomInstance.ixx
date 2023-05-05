module;

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <GLFW/glfw3.h>

export module AhtomInstance;

import VulkanDebug;
import EngineTypes;

export class AhtomInstance
{
public:
	AhtomInstance(std::string applicationName, Version version);

	VkInstance getVkInstance() const;

private:
	std::vector<const char*> getRequiredExtension();
	
	VkDebugUtilsMessengerEXT debugMessenger;
	VkInstance mInstance;
};

AhtomInstance::AhtomInstance(std::string applicationName, Version version)
{
	if (Debug::enableValidationLayers && !Debug::checkValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = applicationName.c_str();
	appInfo.applicationVersion = VK_MAKE_VERSION(version.major, version.minor, version.patch);
	appInfo.pEngineName = applicationName.c_str();
	appInfo.engineVersion = VK_MAKE_VERSION(version.major, version.minor, version.patch);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (Debug::enableValidationLayers)
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


	if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create instance!");
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
			if (extension.extensionName == currentExtension)
			{
				foundExtension = true;
				break;
			}
		}

		allRequiredExternsionExist &= foundExtension;
	}

	if (!allRequiredExternsionExist)
	{
		std::cout << "Not all required extensions are present" << std::endl;
	}
	else
	{
		std::cout << "All required extensions are present" << std::endl;
	}

	Debug::setupDebugMessenger(mInstance, &debugMessenger);
}

VkInstance AhtomInstance::getVkInstance() const
{
	return mInstance;
}

std::vector<const char*> AhtomInstance::getRequiredExtension()
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