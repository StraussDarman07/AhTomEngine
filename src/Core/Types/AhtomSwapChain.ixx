module;

#include <optional>
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>

export module AhtomSwapChain;

import VulkanDevice;
import ImageUtils;
import AhtomDevice;

export class AhtomSwapChain
{
public:
	AhtomSwapChain(AhtomDevice device, VkSurfaceKHR surface);
	void createImageViews(AhtomDevice device);

	VkFormat getSwapChainImageFormat() const;
	VkExtent2D getSwapChainExtent() const;

private:
	VkSwapchainKHR swapChain;
	std::vector<VkImage> mSwapChainImages;
	std::vector<VkImageView> mSwapChainImageViews;
	VkFormat mSwapChainImageFormat;
	VkExtent2D mSwapChainExtent;

};

AhtomSwapChain::AhtomSwapChain(AhtomDevice device, VkSurfaceKHR surface)
{
	const Device::SwapChainSupportDetails swap_chain_support = Device::querySwapChainSupport(device.getPhysicalDevice(), surface);

	VkSurfaceFormatKHR surface_format = Device::chooseSwapSurfaceFormat(swap_chain_support.formats);

	VkPresentModeKHR present_mode = Device::chooseSwapPresentMode(swap_chain_support.present_modes);

	VkExtent2D extent = Device::chooseSwapExtent(swap_chain_support.capabilities);

	uint32_t image_count = swap_chain_support.capabilities.minImageCount + 1;

	if (swap_chain_support.capabilities.maxImageArrayLayers > 0 && image_count > swap_chain_support.capabilities.maxImageCount)
	{
		image_count = swap_chain_support.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	create_info.surface = surface;

	create_info.minImageCount = image_count;
	create_info.imageFormat = surface_format.format;
	create_info.imageColorSpace = surface_format.colorSpace;
	create_info.imageExtent = extent;
	create_info.imageArrayLayers = 1;
	//Use VK_IMAGE_USAGE_TRANSFER_DST_BIT if post-processing is needed
	create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	Device::QueueFamilyIndices indices = Device::findQueueFamilies(device.getPhysicalDevice(), surface);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		create_info.queueFamilyIndexCount = 2;
		create_info.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		create_info.queueFamilyIndexCount = 0; // Optional
		create_info.pQueueFamilyIndices = nullptr; // Optional
	}

	create_info.preTransform = swap_chain_support.capabilities.currentTransform;
	create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	create_info.presentMode = present_mode;
	create_info.clipped = VK_TRUE;

	create_info.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device.getLogicalDevice(), &create_info, nullptr, &swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swap chain");
	}

	vkGetSwapchainImagesKHR(device.getLogicalDevice(), swapChain, &image_count, nullptr);
	mSwapChainImages.resize(image_count);
	vkGetSwapchainImagesKHR(device.getLogicalDevice(), swapChain, &image_count, mSwapChainImages.data());

	mSwapChainImageFormat = surface_format.format;
	mSwapChainExtent = extent;
}

void AhtomSwapChain::createImageViews(AhtomDevice device)
{
	mSwapChainImageViews.resize(mSwapChainImages.size());

	for (uint32_t i = 0; i < mSwapChainImages.size(); i++) {
		mSwapChainImageViews[i] = ImageUtils::createImageView(device.getLogicalDevice(), mSwapChainImages[i], mSwapChainImageFormat);
	}
}

VkFormat AhtomSwapChain::getSwapChainImageFormat() const
{
	return mSwapChainImageFormat;
}

VkExtent2D AhtomSwapChain::getSwapChainExtent() const
{
	return mSwapChainExtent;
}

