#include "AhTomPhysicalDevice.h"

#include "AhTomWindowSurface.h"
#include "AhTomDefines.h"

#include <stdexcept>
#include <vector>
#include <set>


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

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;

	if (extensionsSupported)
	{
		Types::SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

inline bool Core::Engine::AhTomPhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
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

Types::SwapChainSupportDetails Core::Engine::AhTomPhysicalDevice::querySwapChainSupport() const
{
	return querySwapChainSupport(mPhysicalDevice);
}

Types::SwapChainSupportDetails Core::Engine::AhTomPhysicalDevice::querySwapChainSupport(VkPhysicalDevice device) const
{
	Types::SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mWindowSurface.surface(), &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, mWindowSurface.surface(), &formatCount, nullptr);

	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, mWindowSurface.surface(), &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, mWindowSurface.surface(), &presentModeCount, nullptr);

	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, mWindowSurface.surface(), &presentModeCount, details.presentModes.data());
	}

	return details;
}