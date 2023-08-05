#include "AhTomLogicalDevice.h"

#include "core/vulkan/VkValidationLayerSupport.h"
#include "types/AhTomTypes.h"


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

	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

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

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &mDevice) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(mDevice, indices.graphicsFamily.value(), 0, &mGraphicsQueue);
}
