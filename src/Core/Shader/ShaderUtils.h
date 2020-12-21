#include <stdexcept>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Core::Shader
{
	static VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice* device)
	{
		VkShaderModuleCreateInfo create_info{};

		create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		create_info.codeSize = code.size();
		create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());


		VkShaderModule shader_module;

		if(vkCreateShaderModule(*device, &create_info, nullptr, &shader_module) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create shader module!");
		}

		return shader_module;
	}
}
