#include "AhTomLogicalDevice.h"

#include "core/vulkan/VkValidationLayerSupport.h"
#include "types/AhTomTypes.h"

#include <set>

Core::Engine::AhTomLogicalDevice::AhTomLogicalDevice(const AhTomPhysicalDevice& physicalDevice)
{
	createLogicalDevice(physicalDevice);
}

void Core::Engine::AhTomLogicalDevice::destroy()
{
	vkDestroyDevice(mDevice, nullptr);
}

VkQueue Core::Engine::AhTomLogicalDevice::getGraphicsQueue()
{
	return mGraphicsQueue;
}

void Core::Engine::AhTomLogicalDevice::createLogicalDevice(const AhTomPhysicalDevice& physicalDevice)
{
	Types::QueueFamilyIndices indices = physicalDevice.findQueueFamilies();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };


	float queuePriority = 1.0f;

	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = 0;

	if (Vulkan::ENABLE_VALIDATION_LAYERS)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(Vulkan::VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = Vulkan::VALIDATION_LAYERS.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice.physicalDevice(), &createInfo, nullptr, &mDevice) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(mDevice, indices.graphicsFamily.value(), 0, &mGraphicsQueue);
	vkGetDeviceQueue(mDevice, indices.presentFamily.value(), 0, &mPresentQueue);
}
