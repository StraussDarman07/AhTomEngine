#ifndef AHTOMENGINE_H
#define AHTOMENGINE_H

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "GLFW/glfw3.h"

namespace Core
{

	
	class AhTomEngine {

	public:
		
		AhTomEngine(int versionMajor, int versionMinor, int versionPatch);
		
		void run();

	private:

		VkDebugUtilsMessengerEXT debugMessenger;

		int versionMajor;

		int versionMinor;

		int versionPatch;
		
		std::string applicationName = "AhTomEngine";
		
		GLFWwindow* window = nullptr;

		VkInstance instance = nullptr;

		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

		VkDevice device;

		VkQueue graphicsQueue;

		VkSurfaceKHR surface;

		VkQueue presentQueue;

		VkSwapchainKHR swapChain;

		std::vector<VkImage> swapChainImages;

		VkFormat swapChainImageFormat;

		VkExtent2D swapChainExtent;

		std::vector<VkImageView> swapChainImageViews;

		void initWindow();

		void initVulkan();

		void createInstance();

		void createLogicalDevice();

		void createSurface();

		void createSwapChain();
		
		void createImageViews();
		
		void createGraphicsPipeline();

		std::vector<const char*> getRequiredExtension();

		void mainLoop();

		void pickPhysicalDevice();

		void cleanup();
	};
}
#endif // AHTOMENGINE_H
