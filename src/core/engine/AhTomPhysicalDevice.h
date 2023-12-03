#pragma once

#include "types/AhTomTypes.h"

#include <vulkan/vulkan.h>

namespace Core::Engine
{
	class AhTomWindowSurface;
	class AhTomPhysicalDevice
	{
	public:
		explicit AhTomPhysicalDevice(const VkInstance& instance, const AhTomWindowSurface& windowSurface);

		Types::QueueFamilyIndices findQueueFamilies() const;
		Types::SwapChainSupportDetails querySwapChainSupport() const;

		VkPhysicalDevice physicalDevice() const;

	private:
		void pickPhysicalDevice(const VkInstance& instance);
		inline bool isDeviceSuitable(VkPhysicalDevice device);
		inline bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		Types::QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
		Types::SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;


		VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
		const AhTomWindowSurface& mWindowSurface;
	};
}