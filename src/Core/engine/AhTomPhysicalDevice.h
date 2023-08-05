#pragma once

#include "types/AhTomTypes.h"

#include <vulkan/vulkan.h>

namespace Core::Engine
{
	class AhTomPhysicalDevice
	{
	public:
		explicit AhTomPhysicalDevice(const VkInstance& instance);

		Types::QueueFamilyIndices findQueueFamilies() const;

		operator VkPhysicalDevice() const;

	private:
		void pickPhysicalDevice(const VkInstance& instance);
		inline bool isDeviceSuitable(VkPhysicalDevice device);
		Types::QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;

		VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	};
}