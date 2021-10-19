#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "GLFW/glfw3.h"

namespace Core
{

	
	class AhTomEngine {

	public:
		bool framebufferResized = false;
		
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

		VkRenderPass renderPass;

		VkDescriptorSetLayout descriptorSetLayout;
		
		VkPipelineLayout pipelineLayout;

		std::vector<VkImage> swapChainImages;

		VkFormat swapChainImageFormat;

		VkExtent2D swapChainExtent;

		VkPipeline graphicsPipeline;

		VkCommandPool commandPool;

		VkBuffer vertexBuffer;

		VkDeviceMemory vertexBufferMemory;
		
		VkBuffer indexBuffer;

		VkDeviceMemory indexBufferMemory;
		
		VkDescriptorPool descriptorPool;

		VkImage textureImage;
		
		VkDeviceMemory textureImageMemory;

		VkImageView textureImageView;
		
		VkSampler textureSampler;

		std::vector<VkBuffer> uniformBuffers;
		
		std::vector<VkDeviceMemory> uniformBuffersMemory;

		std::vector<VkImageView> swapChainImageViews;

		std::vector<VkFramebuffer> swapChainFramebuffers;

		std::vector<VkCommandBuffer> commandBuffers;
		
		std::vector<VkDescriptorSet> descriptorSets;

		const int MAX_FRAMES_IN_FLIGHT = 2;
		
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;

		size_t currentFrame = 0;


		void initWindow();

		void initVulkan();

		void createInstance();

		void createLogicalDevice();

		void createSurface();

		void createSwapChain();
		
		void createImageViews();

		void createDescriptorSetLayout();
		
		void createGraphicsPipeline();

		void createRenderPass();

		void createFramebuffers();

		void createCommandPool();
		
		void createVertexBuffer();

		void createUniformBuffers();

		void createDescriptorPool();

		void createIndexBuffer();
		
		void createCommandBuffers();

		void createSyncObjects();

		void recreateSwapChain();
		
		void cleanupSwapChain();

		void updateUniformBuffer(uint32_t currentImage);

		void createDescriptorSets();

		void createTextureImage();

		void createTextureImageView();

		void createTextureSampler();

		std::vector<const char*> getRequiredExtension();

		void mainLoop();

		void drawFrame();

		void pickPhysicalDevice();

		void cleanup();
	};
}