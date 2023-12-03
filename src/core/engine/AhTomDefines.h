#pragma once

#include <vulkan/vulkan.h>

#include <array>

namespace Core::Engine
{
	constexpr std::array DEVICE_EXTENSIONS =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
}