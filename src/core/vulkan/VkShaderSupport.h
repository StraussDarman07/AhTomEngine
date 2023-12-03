//
// Created by stach on 03/12/2023.
//

#pragma once

#include "core/utils/FileUtils.h"

#include <string>
#include <vulkan/vulkan.h>

namespace Core::Vulkan {
    inline VkShaderModule createShaderModule(const std::string& filepath, const VkDevice& device)
    {
        auto code = Core::Utils::readFile(filepath);

        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }

        return shaderModule;
    }
}