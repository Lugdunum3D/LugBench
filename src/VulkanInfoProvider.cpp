#include "VulkanInfoProvider.hpp"

VulkanInfoProvider::VulkanInfoProvider(lug::Graphics::Vulkan::PhysicalDeviceInfo *physicalDeviceInfo) : _physicalDeviceInfo(physicalDeviceInfo) {}

nlohmann::json VulkanInfoProvider::getJSONVulkanInfo() {
    nlohmann::json json;

    json["properties"] = {
        {"apiVersion", {
            {"major", lug::Graphics::Vulkan::Version::fromInt(_physicalDeviceInfo->properties.apiVersion).major},
            {"minor", lug::Graphics::Vulkan::Version::fromInt(_physicalDeviceInfo->properties.apiVersion).minor},
            {"patch", lug::Graphics::Vulkan::Version::fromInt(_physicalDeviceInfo->properties.apiVersion).patch}
        }},
        {"driverVersion", {
            {"major", lug::Graphics::Vulkan::Version::fromInt(_physicalDeviceInfo->properties.driverVersion).major},
            {"minor", lug::Graphics::Vulkan::Version::fromInt(_physicalDeviceInfo->properties.driverVersion).minor},
            {"patch", lug::Graphics::Vulkan::Version::fromInt(_physicalDeviceInfo->properties.driverVersion).patch}
        }},
        {"vendorID", _physicalDeviceInfo->properties.vendorID},
        {"deviceID", _physicalDeviceInfo->properties.deviceID},
        {"deviceType", lug::Graphics::Vulkan::API::RTTI::toStr(_physicalDeviceInfo->properties.deviceType)},
        {"deviceName", _physicalDeviceInfo->properties.deviceName},
        {"pipelineCacheUUID", std::vector<uint8_t>(std::begin(_physicalDeviceInfo->properties.pipelineCacheUUID), std::end(_physicalDeviceInfo->properties.pipelineCacheUUID))},
        {"limits", {
            {"maxImageDimension1D", _physicalDeviceInfo->properties.limits.maxImageDimension1D},
            {"maxImageDimension2D", _physicalDeviceInfo->properties.limits.maxImageDimension2D},
            {"maxImageDimension3D", _physicalDeviceInfo->properties.limits.maxImageDimension3D},
            {"maxImageDimensionCube", _physicalDeviceInfo->properties.limits.maxImageDimensionCube},
            {"maxImageArrayLayers", _physicalDeviceInfo->properties.limits.maxImageArrayLayers},
            {"maxTexelBufferElements", _physicalDeviceInfo->properties.limits.maxTexelBufferElements},
            {"maxUniformBufferRange", _physicalDeviceInfo->properties.limits.maxUniformBufferRange},
            {"maxStorageBufferRange", _physicalDeviceInfo->properties.limits.maxStorageBufferRange},
            {"maxPushConstantsSize", _physicalDeviceInfo->properties.limits.maxPushConstantsSize},
            {"maxMemoryAllocationCount", _physicalDeviceInfo->properties.limits.maxMemoryAllocationCount},
            {"maxSamplerAllocationCount", _physicalDeviceInfo->properties.limits.maxSamplerAllocationCount},
            {"bufferImageGranularity", _physicalDeviceInfo->properties.limits.bufferImageGranularity},
            {"sparseAddressSpaceSize", _physicalDeviceInfo->properties.limits.sparseAddressSpaceSize},
            {"maxBoundDescriptorSets", _physicalDeviceInfo->properties.limits.maxBoundDescriptorSets},
            {"maxPerStageDescriptorSamplers", _physicalDeviceInfo->properties.limits.maxPerStageDescriptorSamplers},
            {"maxPerStageDescriptorUniformBuffers", _physicalDeviceInfo->properties.limits.maxPerStageDescriptorUniformBuffers},
            {"maxPerStageDescriptorStorageBuffers", _physicalDeviceInfo->properties.limits.maxPerStageDescriptorStorageBuffers},
            {"maxPerStageDescriptorSampledImages", _physicalDeviceInfo->properties.limits.maxPerStageDescriptorSampledImages},
            {"maxPerStageDescriptorStorageImages", _physicalDeviceInfo->properties.limits.maxPerStageDescriptorStorageImages},
            {"maxPerStageDescriptorInputAttachments", _physicalDeviceInfo->properties.limits.maxPerStageDescriptorInputAttachments},
            {"maxPerStageResources", _physicalDeviceInfo->properties.limits.maxPerStageResources},
            {"maxDescriptorSetSamplers", _physicalDeviceInfo->properties.limits.maxDescriptorSetSamplers},
            {"maxDescriptorSetUniformBuffers", _physicalDeviceInfo->properties.limits.maxDescriptorSetUniformBuffers},
            {"maxDescriptorSetUniformBuffersDynamic", _physicalDeviceInfo->properties.limits.maxDescriptorSetUniformBuffersDynamic},
            {"maxDescriptorSetStorageBuffers", _physicalDeviceInfo->properties.limits.maxDescriptorSetStorageBuffers},
            {"maxDescriptorSetStorageBuffersDynamic", _physicalDeviceInfo->properties.limits.maxDescriptorSetStorageBuffersDynamic},
            {"maxDescriptorSetSampledImages", _physicalDeviceInfo->properties.limits.maxDescriptorSetSampledImages},
            {"maxDescriptorSetStorageImages", _physicalDeviceInfo->properties.limits.maxDescriptorSetStorageImages},
            {"maxDescriptorSetInputAttachments", _physicalDeviceInfo->properties.limits.maxDescriptorSetInputAttachments},
            {"maxVertexInputAttributes", _physicalDeviceInfo->properties.limits.maxVertexInputAttributes},
            {"maxVertexInputBindings", _physicalDeviceInfo->properties.limits.maxVertexInputBindings},
            {"maxVertexInputAttributeOffset", _physicalDeviceInfo->properties.limits.maxVertexInputAttributeOffset},
            {"maxVertexInputBindingStride", _physicalDeviceInfo->properties.limits.maxVertexInputBindingStride},
            {"maxVertexOutputComponents", _physicalDeviceInfo->properties.limits.maxVertexOutputComponents},
            {"maxTessellationGenerationLevel", _physicalDeviceInfo->properties.limits.maxTessellationGenerationLevel},
            {"maxTessellationPatchSize", _physicalDeviceInfo->properties.limits.maxTessellationPatchSize},
            {"maxTessellationControlPerVertexInputComponents", _physicalDeviceInfo->properties.limits.maxTessellationControlPerVertexInputComponents},
            {"maxTessellationControlPerVertexOutputComponents", _physicalDeviceInfo->properties.limits.maxTessellationControlPerVertexOutputComponents},
            {"maxTessellationControlPerPatchOutputComponents", _physicalDeviceInfo->properties.limits.maxTessellationControlPerPatchOutputComponents},
            {"maxTessellationControlTotalOutputComponents", _physicalDeviceInfo->properties.limits.maxTessellationControlTotalOutputComponents},
            {"maxTessellationEvaluationInputComponents", _physicalDeviceInfo->properties.limits.maxTessellationEvaluationInputComponents},
            {"maxTessellationEvaluationOutputComponents", _physicalDeviceInfo->properties.limits.maxTessellationEvaluationOutputComponents},
            {"maxGeometryShaderInvocations", _physicalDeviceInfo->properties.limits.maxGeometryShaderInvocations},
            {"maxGeometryInputComponents", _physicalDeviceInfo->properties.limits.maxGeometryInputComponents},
            {"maxGeometryOutputComponents", _physicalDeviceInfo->properties.limits.maxGeometryOutputComponents},
            {"maxGeometryOutputVertices", _physicalDeviceInfo->properties.limits.maxGeometryOutputVertices},
            {"maxGeometryTotalOutputComponents", _physicalDeviceInfo->properties.limits.maxGeometryTotalOutputComponents},
            {"maxFragmentInputComponents", _physicalDeviceInfo->properties.limits.maxFragmentInputComponents},
            {"maxFragmentOutputAttachments", _physicalDeviceInfo->properties.limits.maxFragmentOutputAttachments},
            {"maxFragmentDualSrcAttachments", _physicalDeviceInfo->properties.limits.maxFragmentDualSrcAttachments},
            {"maxFragmentCombinedOutputResources", _physicalDeviceInfo->properties.limits.maxFragmentCombinedOutputResources},
            {"maxComputeSharedMemorySize", _physicalDeviceInfo->properties.limits.maxComputeSharedMemorySize},
            {"maxComputeWorkGroupCount", std::vector<uint32_t>(std::begin(_physicalDeviceInfo->properties.limits.maxComputeWorkGroupCount), std::end(_physicalDeviceInfo->properties.limits.maxComputeWorkGroupCount))},
            {"maxComputeWorkGroupInvocations", _physicalDeviceInfo->properties.limits.maxComputeWorkGroupInvocations},
            {"maxComputeWorkGroupSize", std::vector<uint32_t>(std::begin(_physicalDeviceInfo->properties.limits.maxComputeWorkGroupSize), std::end(_physicalDeviceInfo->properties.limits.maxComputeWorkGroupSize))},
            {"subPixelPrecisionBits", _physicalDeviceInfo->properties.limits.subPixelPrecisionBits},
            {"subTexelPrecisionBits", _physicalDeviceInfo->properties.limits.subTexelPrecisionBits},
            {"mipmapPrecisionBits", _physicalDeviceInfo->properties.limits.mipmapPrecisionBits},
            {"maxDrawIndexedIndexValue", _physicalDeviceInfo->properties.limits.maxDrawIndexedIndexValue},
            {"maxDrawIndirectCount", _physicalDeviceInfo->properties.limits.maxDrawIndirectCount},
            {"maxSamplerLodBias", _physicalDeviceInfo->properties.limits.maxSamplerLodBias},
            {"maxSamplerAnisotropy", _physicalDeviceInfo->properties.limits.maxSamplerAnisotropy},
            {"maxViewports", _physicalDeviceInfo->properties.limits.maxViewports},
            {"maxViewportDimensions", std::vector<uint32_t>(std::begin(_physicalDeviceInfo->properties.limits.maxViewportDimensions), std::end(_physicalDeviceInfo->properties.limits.maxViewportDimensions))},
            {"viewportBoundsRange", std::vector<float>(std::begin(_physicalDeviceInfo->properties.limits.viewportBoundsRange), std::end(_physicalDeviceInfo->properties.limits.viewportBoundsRange))},
            {"viewportSubPixelBits", _physicalDeviceInfo->properties.limits.viewportSubPixelBits},
            {"minMemoryMapAlignment", _physicalDeviceInfo->properties.limits.minMemoryMapAlignment},
            {"minTexelBufferOffsetAlignment", _physicalDeviceInfo->properties.limits.minTexelBufferOffsetAlignment},
            {"minUniformBufferOffsetAlignment", _physicalDeviceInfo->properties.limits.minUniformBufferOffsetAlignment},
            {"minStorageBufferOffsetAlignment", _physicalDeviceInfo->properties.limits.minStorageBufferOffsetAlignment},
            {"minTexelOffset", _physicalDeviceInfo->properties.limits.minTexelOffset},
            {"maxTexelOffset", _physicalDeviceInfo->properties.limits.maxTexelOffset},
            {"minTexelGatherOffset", _physicalDeviceInfo->properties.limits.minTexelGatherOffset},
            {"maxTexelGatherOffset", _physicalDeviceInfo->properties.limits.maxTexelGatherOffset},
            {"minInterpolationOffset", _physicalDeviceInfo->properties.limits.minInterpolationOffset},
            {"maxInterpolationOffset", _physicalDeviceInfo->properties.limits.maxInterpolationOffset},
            {"subPixelInterpolationOffsetBits", _physicalDeviceInfo->properties.limits.subPixelInterpolationOffsetBits},
            {"maxFramebufferWidth", _physicalDeviceInfo->properties.limits.maxFramebufferWidth},
            {"maxFramebufferHeight", _physicalDeviceInfo->properties.limits.maxFramebufferHeight},
            {"maxFramebufferLayers", _physicalDeviceInfo->properties.limits.maxFramebufferLayers},
            {"framebufferColorSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStr(_physicalDeviceInfo->properties.limits.framebufferColorSampleCounts)},
            {"framebufferDepthSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStr(_physicalDeviceInfo->properties.limits.framebufferDepthSampleCounts)},
            {"framebufferStencilSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStr(_physicalDeviceInfo->properties.limits.framebufferStencilSampleCounts)},
            {"framebufferNoAttachmentsSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStr(_physicalDeviceInfo->properties.limits.framebufferNoAttachmentsSampleCounts)},
            {"maxColorAttachments", _physicalDeviceInfo->properties.limits.maxColorAttachments},
            {"sampledImageColorSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStr(_physicalDeviceInfo->properties.limits.sampledImageColorSampleCounts)},
            {"sampledImageIntegerSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStr(_physicalDeviceInfo->properties.limits.sampledImageIntegerSampleCounts)},
            {"sampledImageDepthSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStr(_physicalDeviceInfo->properties.limits.sampledImageDepthSampleCounts)},
            {"sampledImageStencilSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStr(_physicalDeviceInfo->properties.limits.sampledImageStencilSampleCounts)},
            {"storageImageSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStr(_physicalDeviceInfo->properties.limits.storageImageSampleCounts)},
            {"maxSampleMaskWords", _physicalDeviceInfo->properties.limits.maxSampleMaskWords},
            {"timestampComputeAndGraphics", static_cast<bool>(_physicalDeviceInfo->properties.limits.timestampComputeAndGraphics)},
            {"timestampPeriod", _physicalDeviceInfo->properties.limits.timestampPeriod},
            {"maxClipDistances", _physicalDeviceInfo->properties.limits.maxClipDistances},
            {"maxCullDistances", _physicalDeviceInfo->properties.limits.maxCullDistances},
            {"maxCombinedClipAndCullDistances", _physicalDeviceInfo->properties.limits.maxCombinedClipAndCullDistances},
            {"discreteQueuePriorities", _physicalDeviceInfo->properties.limits.discreteQueuePriorities},
            {"pointSizeRange", std::vector<float>(std::begin(_physicalDeviceInfo->properties.limits.pointSizeRange), std::end(_physicalDeviceInfo->properties.limits.pointSizeRange))},
            {"lineWidthRange", std::vector<float>(std::begin(_physicalDeviceInfo->properties.limits.lineWidthRange), std::end(_physicalDeviceInfo->properties.limits.lineWidthRange))},
            {"pointSizeGranularity", _physicalDeviceInfo->properties.limits.pointSizeGranularity},
            {"lineWidthGranularity", _physicalDeviceInfo->properties.limits.lineWidthGranularity},
            {"strictLines", static_cast<bool>(_physicalDeviceInfo->properties.limits.strictLines)},
            {"standardSampleLocations", static_cast<bool>(_physicalDeviceInfo->properties.limits.standardSampleLocations)},
            {"optimalBufferCopyOffsetAlignment", _physicalDeviceInfo->properties.limits.optimalBufferCopyOffsetAlignment},
            {"optimalBufferCopyRowPitchAlignment", _physicalDeviceInfo->properties.limits.optimalBufferCopyRowPitchAlignment},
            {"nonCoherentAtomSize", _physicalDeviceInfo->properties.limits.nonCoherentAtomSize}
        }},
        {"sparseProperties", {
            {"residencyStandard2DBlockShape", static_cast<bool>(_physicalDeviceInfo->properties.sparseProperties.residencyStandard2DBlockShape)},
            {"residencyStandard2DMultisampleBlockShape", static_cast<bool>(_physicalDeviceInfo->properties.sparseProperties.residencyStandard2DMultisampleBlockShape)},
            {"residencyStandard3DBlockShape", static_cast<bool>(_physicalDeviceInfo->properties.sparseProperties.residencyStandard3DBlockShape)},
            {"residencyAlignedMipSize", static_cast<bool>(_physicalDeviceInfo->properties.sparseProperties.residencyAlignedMipSize)},
            {"residencyNonResidentStrict", static_cast<bool>(_physicalDeviceInfo->properties.sparseProperties.residencyNonResidentStrict)},
        }}
    };

    json["features"] = {
        {"robustBufferAccess", static_cast<bool>(_physicalDeviceInfo->features.robustBufferAccess)},
        {"fullDrawIndexUint32", static_cast<bool>(_physicalDeviceInfo->features.fullDrawIndexUint32)},
        {"imageCubeArray", static_cast<bool>(_physicalDeviceInfo->features.imageCubeArray)},
        {"independentBlend", static_cast<bool>(_physicalDeviceInfo->features.independentBlend)},
        {"geometryShader", static_cast<bool>(_physicalDeviceInfo->features.geometryShader)},
        {"tessellationShader", static_cast<bool>(_physicalDeviceInfo->features.tessellationShader)},
        {"sampleRateShading", static_cast<bool>(_physicalDeviceInfo->features.sampleRateShading)},
        {"dualSrcBlend", static_cast<bool>(_physicalDeviceInfo->features.dualSrcBlend)},
        {"logicOp", static_cast<bool>(_physicalDeviceInfo->features.logicOp)},
        {"multiDrawIndirect", static_cast<bool>(_physicalDeviceInfo->features.multiDrawIndirect)},
        {"drawIndirectFirstInstance", static_cast<bool>(_physicalDeviceInfo->features.drawIndirectFirstInstance)},
        {"depthClamp", static_cast<bool>(_physicalDeviceInfo->features.depthClamp)},
        {"depthBiasClamp", static_cast<bool>(_physicalDeviceInfo->features.depthBiasClamp)},
        {"fillModeNonSolid", static_cast<bool>(_physicalDeviceInfo->features.fillModeNonSolid)},
        {"depthBounds", static_cast<bool>(_physicalDeviceInfo->features.depthBounds)},
        {"wideLines", static_cast<bool>(_physicalDeviceInfo->features.wideLines)},
        {"largePoints", static_cast<bool>(_physicalDeviceInfo->features.largePoints)},
        {"alphaToOne", static_cast<bool>(_physicalDeviceInfo->features.alphaToOne)},
        {"multiViewport", static_cast<bool>(_physicalDeviceInfo->features.multiViewport)},
        {"samplerAnisotropy", static_cast<bool>(_physicalDeviceInfo->features.samplerAnisotropy)},
        {"textureCompressionETC2", static_cast<bool>(_physicalDeviceInfo->features.textureCompressionETC2)},
        {"textureCompressionASTC_LDR", static_cast<bool>(_physicalDeviceInfo->features.textureCompressionASTC_LDR)},
        {"textureCompressionBC", static_cast<bool>(_physicalDeviceInfo->features.textureCompressionBC)},
        {"occlusionQueryPrecise", static_cast<bool>(_physicalDeviceInfo->features.occlusionQueryPrecise)},
        {"pipelineStatisticsQuery", static_cast<bool>(_physicalDeviceInfo->features.pipelineStatisticsQuery)},
        {"vertexPipelineStoresAndAtomics", static_cast<bool>(_physicalDeviceInfo->features.vertexPipelineStoresAndAtomics)},
        {"fragmentStoresAndAtomics", static_cast<bool>(_physicalDeviceInfo->features.fragmentStoresAndAtomics)},
        {"shaderTessellationAndGeometryPointSize", static_cast<bool>(_physicalDeviceInfo->features.shaderTessellationAndGeometryPointSize)},
        {"shaderImageGatherExtended", static_cast<bool>(_physicalDeviceInfo->features.shaderImageGatherExtended)},
        {"shaderStorageImageExtendedFormats", static_cast<bool>(_physicalDeviceInfo->features.shaderStorageImageExtendedFormats)},
        {"shaderStorageImageMultisample", static_cast<bool>(_physicalDeviceInfo->features.shaderStorageImageMultisample)},
        {"shaderStorageImageReadWithoutFormat", static_cast<bool>(_physicalDeviceInfo->features.shaderStorageImageReadWithoutFormat)},
        {"shaderStorageImageWriteWithoutFormat", static_cast<bool>(_physicalDeviceInfo->features.shaderStorageImageWriteWithoutFormat)},
        {"shaderUniformBufferArrayDynamicIndexing", static_cast<bool>(_physicalDeviceInfo->features.shaderUniformBufferArrayDynamicIndexing)},
        {"shaderSampledImageArrayDynamicIndexing", static_cast<bool>(_physicalDeviceInfo->features.shaderSampledImageArrayDynamicIndexing)},
        {"shaderStorageBufferArrayDynamicIndexing", static_cast<bool>(_physicalDeviceInfo->features.shaderStorageBufferArrayDynamicIndexing)},
        {"shaderStorageImageArrayDynamicIndexing", static_cast<bool>(_physicalDeviceInfo->features.shaderStorageImageArrayDynamicIndexing)},
        {"shaderClipDistance", static_cast<bool>(_physicalDeviceInfo->features.shaderClipDistance)},
        {"shaderCullDistance", static_cast<bool>(_physicalDeviceInfo->features.shaderCullDistance)},
        {"shaderFloat64", static_cast<bool>(_physicalDeviceInfo->features.shaderFloat64)},
        {"shaderInt64", static_cast<bool>(_physicalDeviceInfo->features.shaderInt64)},
        {"shaderInt16", static_cast<bool>(_physicalDeviceInfo->features.shaderInt16)},
        {"shaderResourceResidency", static_cast<bool>(_physicalDeviceInfo->features.shaderResourceResidency)},
        {"shaderResourceMinLod", static_cast<bool>(_physicalDeviceInfo->features.shaderResourceMinLod)},
        {"sparseBinding", static_cast<bool>(_physicalDeviceInfo->features.sparseBinding)},
        {"sparseResidencyBuffer", static_cast<bool>(_physicalDeviceInfo->features.sparseResidencyBuffer)},
        {"sparseResidencyImage2D", static_cast<bool>(_physicalDeviceInfo->features.sparseResidencyImage2D)},
        {"sparseResidencyImage3D", static_cast<bool>(_physicalDeviceInfo->features.sparseResidencyImage3D)},
        {"sparseResidency2Samples", static_cast<bool>(_physicalDeviceInfo->features.sparseResidency2Samples)},
        {"sparseResidency4Samples", static_cast<bool>(_physicalDeviceInfo->features.sparseResidency4Samples)},
        {"sparseResidency8Samples", static_cast<bool>(_physicalDeviceInfo->features.sparseResidency8Samples)},
        {"sparseResidency16Samples", static_cast<bool>(_physicalDeviceInfo->features.sparseResidency16Samples)},
        {"sparseResidencyAliased", static_cast<bool>(_physicalDeviceInfo->features.sparseResidencyAliased)},
        {"variableMultisampleRate", static_cast<bool>(_physicalDeviceInfo->features.variableMultisampleRate)},
        {"inheritedQueries", static_cast<bool>(_physicalDeviceInfo->features.inheritedQueries)},
    };

    json["memory"] = {
        {"memoryTypeCount", _physicalDeviceInfo->memoryProperties.memoryTypeCount},
        {"memoryHeapCount", _physicalDeviceInfo->memoryProperties.memoryHeapCount}
    };

    for (size_t i = 0; i < _physicalDeviceInfo->memoryProperties.memoryTypeCount; ++i) {
        json["memory"]["memoryTypes"].push_back(
        {
            {"propertyFlags", lug::Graphics::Vulkan::API::RTTI::VkMemoryPropertyFlagsToStr(_physicalDeviceInfo->memoryProperties.memoryTypes[i].propertyFlags)},
            {"heapIndex", _physicalDeviceInfo->memoryProperties.memoryTypes[i].heapIndex},
        }
        );
    }

    for (size_t i = 0; i < _physicalDeviceInfo->memoryProperties.memoryHeapCount; ++i) {
        json["memory"]["memoryHeaps"].push_back(
        {
            {"size", _physicalDeviceInfo->memoryProperties.memoryHeaps[i].size},
            {"flags", lug::Graphics::Vulkan::API::RTTI::VkMemoryHeapFlagsToStr(_physicalDeviceInfo->memoryProperties.memoryHeaps[i].flags)},
        }
        );
    }

    for (auto tmp : _physicalDeviceInfo->queueFamilies) {
        json["queues"].push_back({
            {"queueFlags", lug::Graphics::Vulkan::API::RTTI::VkQueueFlagsToStr(tmp.queueFlags)},
            {"queueCount", tmp.queueCount},
            {"timestampValidBits", tmp.timestampValidBits},
            {"minImageTransferGranularity", {
                {"width", tmp.minImageTransferGranularity.width},
                {"height", tmp.minImageTransferGranularity.height},
                {"depth", tmp.minImageTransferGranularity.depth}
            }}
        });
    }

    for (auto extension : _physicalDeviceInfo->extensions) {
        json["extension"].push_back(
        {
            {"extensionName", extension.extensionName},
            {"specVersion", {
                {"major", lug::Graphics::Vulkan::Version::fromInt(extension.specVersion).major},
                {"minor", lug::Graphics::Vulkan::Version::fromInt(extension.specVersion).minor},
                {"patch", lug::Graphics::Vulkan::Version::fromInt(extension.specVersion).patch}
            }},
        }
        );
    }

    for (auto format : _physicalDeviceInfo->formatProperties) {
        json["formats"].push_back(
        {
            lug::Graphics::Vulkan::API::RTTI::toStr(format.first),
            {
                {"linearTilingFeatures", lug::Graphics::Vulkan::API::RTTI::VkFormatFeatureFlagsToStr(format.second.linearTilingFeatures)},
                {"optimalTilingFeatures", lug::Graphics::Vulkan::API::RTTI::VkFormatFeatureFlagsToStr(format.second.optimalTilingFeatures)},
                {"bufferFeatures", lug::Graphics::Vulkan::API::RTTI::VkFormatFeatureFlagsToStr(format.second.bufferFeatures)},
            }
        }
        );
    }

    //    for (auto surface : _physicalDeviceInfo->swapchain)
    //  {
    json["zSwapChainInfo"].push_back(
    {
        {"minImageCount", _physicalDeviceInfo->swapchain.capabilities.minImageCount},
        {"maxImageCount", _physicalDeviceInfo->swapchain.capabilities.maxImageCount},
        {"currentExtent",
        {
            {"width", _physicalDeviceInfo->swapchain.capabilities.currentExtent.width},
            {"height", _physicalDeviceInfo->swapchain.capabilities.currentExtent.height}
        }
        },
        {"minImageExtent",
        {
            {"width", _physicalDeviceInfo->swapchain.capabilities.minImageExtent.width},
            {"height", _physicalDeviceInfo->swapchain.capabilities.minImageExtent.height}
        }
        },
        {"maxImageExtent",
        {
            {"width", _physicalDeviceInfo->swapchain.capabilities.maxImageExtent.width},
            {"height", _physicalDeviceInfo->swapchain.capabilities.maxImageExtent.height}
        }
        },

        {"maxImageArrayLayers", _physicalDeviceInfo->swapchain.capabilities.maxImageArrayLayers},

    }
    );
    //

    return json;
}
