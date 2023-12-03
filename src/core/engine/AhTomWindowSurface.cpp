#include "AhTomWindowSurface.h"

#include <stdexcept>

Core::Engine::AhTomWindowSurface::AhTomWindowSurface(const VkInstance& instance, GLFWwindow* window)
{
	createSurface(instance, window);
}

void Core::Engine::AhTomWindowSurface::destroy(const VkInstance& instance)
{
	vkDestroySurfaceKHR(instance, mSurface, nullptr);
}

VkSurfaceKHR Core::Engine::AhTomWindowSurface::surface() const
{
	return mSurface;
}

void Core::Engine::AhTomWindowSurface::createSurface(const VkInstance& instance, GLFWwindow* window)
{
	if (glfwCreateWindowSurface(instance, window, nullptr, &mSurface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}
