//
// Created by stach on 03/12/2023.
//

#pragma once

#include "AhTomLogicalDevice.h"
#include "AhTomSwapChain.h"

namespace Core::Engine {

    struct AhTomGraphicsPipelineRequirements
    {
        const AhTomLogicalDevice& logicalDevice;
        const AhTomSwapChain& swapChain;
    };

    class AhTomGraphicsPipeline {
    public:
        explicit AhTomGraphicsPipeline(AhTomGraphicsPipelineRequirements requirements);

        void destroy(const AhTomLogicalDevice& logicalDevice);

    private:
        void createRenderPass(AhTomGraphicsPipelineRequirements requirements);
        void createGraphicsPipeline(AhTomGraphicsPipelineRequirements requirements);

        VkRenderPass mRenderPass;
        VkPipeline mGraphicsPipeline;
        VkPipelineLayout mPipelineLayout;
    };
}