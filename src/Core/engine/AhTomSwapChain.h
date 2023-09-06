#pragma once

#include "AhTomPhysicalDevice.h"
#include "AhTomWindowSurface.h"
#include "AhTomLogicalDevice.h"

#include "types/AhTomTypes.h"

#include <vulkan/vulkan.h>
#include <vector>

struct GLFWwindow;

namespace Core::Engine
{
	struct AhTomSwapChainRequirements
	{
		const AhTomPhysicalDevice& physicalDevice;
		const AhTomWindowSurface& surface;
		const AhTomLogicalDevice& logicalDevice;

		GLFWwindow* window;
	};

	class AhTomSwapChain
	{
	public:
		explicit AhTomSwapChain(AhTomSwapChainRequirements requirements);

		void destroy(const AhTomLogicalDevice& logicalDevice);

		VkSwapchainKHR getSwapChain();

	private:
		void createSwapChain(AhTomSwapChainRequirements requirements);

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

		VkSwapchainKHR mSwapChain;
		std::vector<VkImage> mSwapChainImages;
		VkFormat mSwapChainImageFormat;
		VkExtent2D mSwapChainExtent;

	};
}