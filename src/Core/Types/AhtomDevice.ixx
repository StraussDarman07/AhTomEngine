module;

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <set>

export module AhtomDevice;

import VulkanDevice;
import VulkanDebug;

export class AhtomDevice
{
public:
	AhtomDevice(VkInstance instance, VkSurfaceKHR surface);

	void createLogicalDevice(VkSurfaceKHR surface);

	VkPhysicalDevice getPhysicalDevice() const;
	VkDevice getLogicalDevice() const;

private:
	VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	VkDevice mLogicalDevice;
	VkQueue mPresentQueue;
	VkQueue mGraphicsQueue;

};

AhtomDevice::AhtomDevice(VkInstance instance, VkSurfaceKHR surface)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPU's with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);

	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	for (const VkPhysicalDevice& device : devices)
	{
		if (Device::isDeviceSuitable(device, surface))
		{
			mPhysicalDevice = device;
			break;
		}
	}

	if (mPhysicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find a sutiable GPU");
	}
}

void AhtomDevice::createLogicalDevice(VkSurfaceKHR surface)
{
	Device::QueueFamilyIndices indices = Device::findQueueFamilies(mPhysicalDevice, surface);

	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	std::set<uint32_t> unique_queue_families = { indices.graphicsFamily.value(), indices.graphicsFamily.value() };

	float queuePriority = 1.0f;

	for (uint32_t queue_family : unique_queue_families)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
		queueCreateInfo.queueCount = 1;

		queueCreateInfo.pQueuePriorities = &queuePriority;

		queue_create_infos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures device_features{};
	device_features.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.pQueueCreateInfos = queue_create_infos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());

	createInfo.pEnabledFeatures = &device_features;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(Device::deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = Device::deviceExtensions.data();

	if (Debug::enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(Debug::validationLayers.size());
		createInfo.ppEnabledLayerNames = Debug::validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mLogicalDevice) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(mLogicalDevice, indices.graphicsFamily.value(), 0, &mGraphicsQueue);
	vkGetDeviceQueue(mLogicalDevice, indices.presentFamily.value(), 0, &mPresentQueue);
}

VkPhysicalDevice AhtomDevice::getPhysicalDevice() const
{
	return mPhysicalDevice;
}

VkDevice AhtomDevice::getLogicalDevice() const
{
	return mLogicalDevice;
}
