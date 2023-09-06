#pragma once

#include "types/AhTomTypes.h"
#include "vulkan/VkDebugSupport.h"

#include <vulkan/vulkan.h>

struct GLFWwindow;
namespace Core
{
	namespace Engine
	{
		class AhTomPhysicalDevice;
		class AhTomLogicalDevice;
		class AhTomWindowSurface;
		class AhTomSwapChain;
	}
	class AhTomEngine
	{
	public:
		AhTomEngine(int major, int minor, int patch);
		void run();

	private:
		void initWindow();
		void initVulkan();
		void mainLoop();
		void cleanup();

		void createInstance();

		Vulkan::VkDebugger mDebugger;

		GLFWwindow* mWindow;
		VkInstance mInstance;
		const Types::Version mVersion;

		std::unique_ptr<Engine::AhTomWindowSurface> mWindowSurface;
		std::unique_ptr<Engine::AhTomPhysicalDevice> mPhysicalDevice;
		std::unique_ptr<Engine::AhTomLogicalDevice> mLogicalDevice;
		std::unique_ptr<Engine::AhTomSwapChain> mSwapChain;
	};
}