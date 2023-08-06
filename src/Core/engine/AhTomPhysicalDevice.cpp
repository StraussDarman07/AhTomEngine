#include "AhTomPhysicalDevice.h"

#include "AhTomWindowSurface.h"

#include <stdexcept>
#include <vector>

Core::Engine::AhTomPhysicalDevice::AhTomPhysicalDevice(const VkInstance& instance, const AhTomWindowSurface& windowSurface) : mWindowSurface(windowSurface)
{
	pickPhysicalDevice(instance);
}

Types::QueueFamilyIndices Core::Engine::AhTomPhysicalDevice::findQueueFamilies() const
{
	return findQueueFamilies(mPhysicalDevice);
}

VkPhysicalDevice Core::Engine::AhTomPhysicalDevice::physicalDevice() const
{
	return mPhysicalDevice;
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

Types::QueueFamilyIndices Core::Engine::AhTomPhysicalDevice::findQueueFamilies(VkPhysicalDevice device) const
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
			indices.graphicsFamily = static_cast<uint32_t>(i);
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, static_cast<uint32_t>(i), mWindowSurface.surface(), &presentSupport);

		if (presentSupport)
		{
			indices.presentFamily = static_cast<uint32_t>(i);
		}
	}

	// Logic to find queue family indices to populate struct with
	return indices;
}
