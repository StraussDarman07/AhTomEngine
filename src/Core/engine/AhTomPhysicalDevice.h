#pragma once

#include "types/AhTomTypes.h"

#include <vulkan/vulkan.h>

namespace Core::Engine
{
	class AhTomPhysicalDevice
	{
	public:
		explicit AhTomPhysicalDevice(const VkInstance& instance);

	private:
		void pickPhysicalDevice(const VkInstance& instance);
		inline bool isDeviceSuitable(VkPhysicalDevice device);
		Types::QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
	};
}