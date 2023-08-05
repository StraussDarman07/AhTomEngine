#include "AhTomPhysicalDevice.h"

#include <stdexcept>
#include <vector>

Core::Engine::AhTomPhysicalDevice::AhTomPhysicalDevice(const VkInstance& instance)
{
	pickPhysicalDevice(instance);
}

void Core::Engine::AhTomPhysicalDevice::pickPhysicalDevice(const VkInstance& instance)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (isDeviceSuitable(device))
		{
			mPhysicalDevice = device;
			break;
		}
	}

	if (mPhysicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

bool Core::Engine::AhTomPhysicalDevice::isDeviceSuitable(VkPhysicalDevice device)
{
	Types::QueueFamilyIndices indices = findQueueFamilies(device);

	return indices.isComplete();
}

Types::QueueFamilyIndices Core::Engine::AhTomPhysicalDevice::findQueueFamilies(VkPhysicalDevice device)
{
	Types::QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());


	for (size_t i = 0; i < queueFamilies.size() && !indices.isComplete(); i++)
	{
		const auto& queueFamily = queueFamilies[i];

		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}
	}

	// Logic to find queue family indices to populate struct with
	return indices;
}
