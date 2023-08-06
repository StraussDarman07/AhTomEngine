#pragma once

#include "AhTomPhysicalDevice.h"

#include "types/AhTomTypes.h"

#include <vulkan/vulkan.h>

namespace Core::Engine
{
	class AhTomSwapChain
	{
	public:
		explicit AhTomSwapChain(const AhTomPhysicalDevice& physicalDevice);

	private:
	};
}