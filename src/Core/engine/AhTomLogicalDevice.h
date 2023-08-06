#pragma once

#include "AhTomPhysicalDevice.h"

#include <vulkan/vulkan.h>

namespace Core::Engine
{
	class AhTomLogicalDevice
	{
	public:
		explicit AhTomLogicalDevice(const AhTomPhysicalDevice& physicalDevice);

		void destroy();

		VkQueue getGraphicsQueue();

	private:
		void createLogicalDevice(const AhTomPhysicalDevice& physicalDevice);

		VkDevice mDevice;

		VkQueue mGraphicsQueue;
		VkQueue mPresentQueue;
	};
}