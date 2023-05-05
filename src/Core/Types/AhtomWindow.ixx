module;

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include <stdexcept>
#include <string>


export module AhtomWindow;

import EngineTypes;
import VulkanDevice;

export class AhtomWindow
{
public:
	AhtomWindow(std::string applicationName, Version version);

	void frameBufferResized();
	void createSurface(VkInstance instance);

	VkSurfaceKHR getSurface() const;

private:
	bool mFrameBufferResized = false;
	GLFWwindow* mWindow = nullptr;
	VkSurfaceKHR mSurface;

};

static void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	auto* app = static_cast<AhtomWindow*>(glfwGetWindowUserPointer(window));
	app->frameBufferResized();
}

AhtomWindow::AhtomWindow(std::string applicationName, Version version)
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	std::string windowName = applicationName + version.toString();

	mWindow = glfwCreateWindow(Device::WIDTH, Device::HEIGHT, windowName.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(mWindow, this);
	glfwSetFramebufferSizeCallback(mWindow, framebufferResizeCallback);
}

void AhtomWindow::frameBufferResized()
{
	mFrameBufferResized = true;
}

void AhtomWindow::createSurface(VkInstance instance)
{
	if (glfwCreateWindowSurface(instance, mWindow, nullptr, &mSurface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}

VkSurfaceKHR AhtomWindow::getSurface() const
{
	return mSurface;
}
