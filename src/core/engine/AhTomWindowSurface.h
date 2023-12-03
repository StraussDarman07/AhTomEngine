#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace Core::Engine
{
	class AhTomWindowSurface
	{
	public:
		explicit AhTomWindowSurface(const VkInstance& instance, GLFWwindow* window);

		void destroy(const VkInstance& instance);

		VkSurfaceKHR surface() const;
	private:
		void createSurface(const VkInstance& instance, GLFWwindow* window);

		VkSurfaceKHR mSurface;
	};
}