#include <GPUInfoProvider.hpp>

#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Core/Version.hpp>

nlohmann::json GPUInfoProvider::get(const lug::Graphics::Vulkan::PhysicalDeviceInfo &physicalDeviceInfo) {
    nlohmann::json json;

    json["properties"] = {
        {"apiVersion", {
            {"major", static_cast<uint32_t>(lug::Core::Version::fromInt(physicalDeviceInfo.properties.apiVersion).major)},
            {"minor", static_cast<uint32_t>(lug::Core::Version::fromInt(physicalDeviceInfo.properties.apiVersion).minor)},
            {"patch", static_cast<uint32_t>(lug::Core::Version::fromInt(physicalDeviceInfo.properties.apiVersion).patch)}
        }},
        {"driverVersion", {
            {"major", static_cast<uint32_t>(lug::Core::Version::fromInt(physicalDeviceInfo.properties.driverVersion).major)},
            {"minor", static_cast<uint32_t>(lug::Core::Version::fromInt(physicalDeviceInfo.properties.driverVersion).minor)},
            {"patch", static_cast<uint32_t>(lug::Core::Version::fromInt(physicalDeviceInfo.properties.driverVersion).patch)}
        }},
        {"vendorID", physicalDeviceInfo.properties.vendorID},
        {"deviceID", physicalDeviceInfo.properties.deviceID},
        {"deviceType", lug::Graphics::Vulkan::API::RTTI::toStr(physicalDeviceInfo.properties.deviceType)},
        {"deviceName", physicalDeviceInfo.properties.deviceName},
        {"pipelineCacheUUID", std::vector<uint8_t>(std::begin(physicalDeviceInfo.properties.pipelineCacheUUID), std::end(physicalDeviceInfo.properties.pipelineCacheUUID))},
        {"limits", {
            {"maxImageDimension1D", physicalDeviceInfo.properties.limits.maxImageDimension1D},
            {"maxImageDimension2D", physicalDeviceInfo.properties.limits.maxImageDimension2D},
            {"maxImageDimension3D", physicalDeviceInfo.properties.limits.maxImageDimension3D},
            {"maxImageDimensionCube", physicalDeviceInfo.properties.limits.maxImageDimensionCube},
            {"maxImageArrayLayers", physicalDeviceInfo.properties.limits.maxImageArrayLayers},
            {"maxTexelBufferElements", physicalDeviceInfo.properties.limits.maxTexelBufferElements},
            {"maxUniformBufferRange", physicalDeviceInfo.properties.limits.maxUniformBufferRange},
            {"maxStorageBufferRange", physicalDeviceInfo.properties.limits.maxStorageBufferRange},
            {"maxPushConstantsSize", physicalDeviceInfo.properties.limits.maxPushConstantsSize},
            {"maxMemoryAllocationCount", physicalDeviceInfo.properties.limits.maxMemoryAllocationCount},
            {"maxSamplerAllocationCount", physicalDeviceInfo.properties.limits.maxSamplerAllocationCount},
            {"bufferImageGranularity", physicalDeviceInfo.properties.limits.bufferImageGranularity},
            {"sparseAddressSpaceSize", physicalDeviceInfo.properties.limits.sparseAddressSpaceSize},
            {"maxBoundDescriptorSets", physicalDeviceInfo.properties.limits.maxBoundDescriptorSets},
            {"maxPerStageDescriptorSamplers", physicalDeviceInfo.properties.limits.maxPerStageDescriptorSamplers},
            {"maxPerStageDescriptorUniformBuffers", physicalDeviceInfo.properties.limits.maxPerStageDescriptorUniformBuffers},
            {"maxPerStageDescriptorStorageBuffers", physicalDeviceInfo.properties.limits.maxPerStageDescriptorStorageBuffers},
            {"maxPerStageDescriptorSampledImages", physicalDeviceInfo.properties.limits.maxPerStageDescriptorSampledImages},
            {"maxPerStageDescriptorStorageImages", physicalDeviceInfo.properties.limits.maxPerStageDescriptorStorageImages},
            {"maxPerStageDescriptorInputAttachments", physicalDeviceInfo.properties.limits.maxPerStageDescriptorInputAttachments},
            {"maxPerStageResources", physicalDeviceInfo.properties.limits.maxPerStageResources},
            {"maxDescriptorSetSamplers", physicalDeviceInfo.properties.limits.maxDescriptorSetSamplers},
            {"maxDescriptorSetUniformBuffers", physicalDeviceInfo.properties.limits.maxDescriptorSetUniformBuffers},
            {"maxDescriptorSetUniformBuffersDynamic", physicalDeviceInfo.properties.limits.maxDescriptorSetUniformBuffersDynamic},
            {"maxDescriptorSetStorageBuffers", physicalDeviceInfo.properties.limits.maxDescriptorSetStorageBuffers},
            {"maxDescriptorSetStorageBuffersDynamic", physicalDeviceInfo.properties.limits.maxDescriptorSetStorageBuffersDynamic},
            {"maxDescriptorSetSampledImages", physicalDeviceInfo.properties.limits.maxDescriptorSetSampledImages},
            {"maxDescriptorSetStorageImages", physicalDeviceInfo.properties.limits.maxDescriptorSetStorageImages},
            {"maxDescriptorSetInputAttachments", physicalDeviceInfo.properties.limits.maxDescriptorSetInputAttachments},
            {"maxVertexInputAttributes", physicalDeviceInfo.properties.limits.maxVertexInputAttributes},
            {"maxVertexInputBindings", physicalDeviceInfo.properties.limits.maxVertexInputBindings},
            {"maxVertexInputAttributeOffset", physicalDeviceInfo.properties.limits.maxVertexInputAttributeOffset},
            {"maxVertexInputBindingStride", physicalDeviceInfo.properties.limits.maxVertexInputBindingStride},
            {"maxVertexOutputComponents", physicalDeviceInfo.properties.limits.maxVertexOutputComponents},
            {"maxTessellationGenerationLevel", physicalDeviceInfo.properties.limits.maxTessellationGenerationLevel},
            {"maxTessellationPatchSize", physicalDeviceInfo.properties.limits.maxTessellationPatchSize},
            {"maxTessellationControlPerVertexInputComponents", physicalDeviceInfo.properties.limits.maxTessellationControlPerVertexInputComponents},
            {"maxTessellationControlPerVertexOutputComponents", physicalDeviceInfo.properties.limits.maxTessellationControlPerVertexOutputComponents},
            {"maxTessellationControlPerPatchOutputComponents", physicalDeviceInfo.properties.limits.maxTessellationControlPerPatchOutputComponents},
            {"maxTessellationControlTotalOutputComponents", physicalDeviceInfo.properties.limits.maxTessellationControlTotalOutputComponents},
            {"maxTessellationEvaluationInputComponents", physicalDeviceInfo.properties.limits.maxTessellationEvaluationInputComponents},
            {"maxTessellationEvaluationOutputComponents", physicalDeviceInfo.properties.limits.maxTessellationEvaluationOutputComponents},
            {"maxGeometryShaderInvocations", physicalDeviceInfo.properties.limits.maxGeometryShaderInvocations},
            {"maxGeometryInputComponents", physicalDeviceInfo.properties.limits.maxGeometryInputComponents},
            {"maxGeometryOutputComponents", physicalDeviceInfo.properties.limits.maxGeometryOutputComponents},
            {"maxGeometryOutputVertices", physicalDeviceInfo.properties.limits.maxGeometryOutputVertices},
            {"maxGeometryTotalOutputComponents", physicalDeviceInfo.properties.limits.maxGeometryTotalOutputComponents},
            {"maxFragmentInputComponents", physicalDeviceInfo.properties.limits.maxFragmentInputComponents},
            {"maxFragmentOutputAttachments", physicalDeviceInfo.properties.limits.maxFragmentOutputAttachments},
            {"maxFragmentDualSrcAttachments", physicalDeviceInfo.properties.limits.maxFragmentDualSrcAttachments},
            {"maxFragmentCombinedOutputResources", physicalDeviceInfo.properties.limits.maxFragmentCombinedOutputResources},
            {"maxComputeSharedMemorySize", physicalDeviceInfo.properties.limits.maxComputeSharedMemorySize},
            {"maxComputeWorkGroupCount", std::vector<uint32_t>(std::begin(physicalDeviceInfo.properties.limits.maxComputeWorkGroupCount), std::end(physicalDeviceInfo.properties.limits.maxComputeWorkGroupCount))},
            {"maxComputeWorkGroupInvocations", physicalDeviceInfo.properties.limits.maxComputeWorkGroupInvocations},
            {"maxComputeWorkGroupSize", std::vector<uint32_t>(std::begin(physicalDeviceInfo.properties.limits.maxComputeWorkGroupSize), std::end(physicalDeviceInfo.properties.limits.maxComputeWorkGroupSize))},
            {"subPixelPrecisionBits", physicalDeviceInfo.properties.limits.subPixelPrecisionBits},
            {"subTexelPrecisionBits", physicalDeviceInfo.properties.limits.subTexelPrecisionBits},
            {"mipmapPrecisionBits", physicalDeviceInfo.properties.limits.mipmapPrecisionBits},
            {"maxDrawIndexedIndexValue", physicalDeviceInfo.properties.limits.maxDrawIndexedIndexValue},
            {"maxDrawIndirectCount", physicalDeviceInfo.properties.limits.maxDrawIndirectCount},
            {"maxSamplerLodBias", physicalDeviceInfo.properties.limits.maxSamplerLodBias},
            {"maxSamplerAnisotropy", physicalDeviceInfo.properties.limits.maxSamplerAnisotropy},
            {"maxViewports", physicalDeviceInfo.properties.limits.maxViewports},
            {"maxViewportDimensions", std::vector<uint32_t>(std::begin(physicalDeviceInfo.properties.limits.maxViewportDimensions), std::end(physicalDeviceInfo.properties.limits.maxViewportDimensions))},
            {"viewportBoundsRange", std::vector<float>(std::begin(physicalDeviceInfo.properties.limits.viewportBoundsRange), std::end(physicalDeviceInfo.properties.limits.viewportBoundsRange))},
            {"viewportSubPixelBits", physicalDeviceInfo.properties.limits.viewportSubPixelBits},
            {"minMemoryMapAlignment", physicalDeviceInfo.properties.limits.minMemoryMapAlignment},
            {"minTexelBufferOffsetAlignment", physicalDeviceInfo.properties.limits.minTexelBufferOffsetAlignment},
            {"minUniformBufferOffsetAlignment", physicalDeviceInfo.properties.limits.minUniformBufferOffsetAlignment},
            {"minStorageBufferOffsetAlignment", physicalDeviceInfo.properties.limits.minStorageBufferOffsetAlignment},
            {"minTexelOffset", physicalDeviceInfo.properties.limits.minTexelOffset},
            {"maxTexelOffset", physicalDeviceInfo.properties.limits.maxTexelOffset},
            {"minTexelGatherOffset", physicalDeviceInfo.properties.limits.minTexelGatherOffset},
            {"maxTexelGatherOffset", physicalDeviceInfo.properties.limits.maxTexelGatherOffset},
            {"minInterpolationOffset", physicalDeviceInfo.properties.limits.minInterpolationOffset},
            {"maxInterpolationOffset", physicalDeviceInfo.properties.limits.maxInterpolationOffset},
            {"subPixelInterpolationOffsetBits", physicalDeviceInfo.properties.limits.subPixelInterpolationOffsetBits},
            {"maxFramebufferWidth", physicalDeviceInfo.properties.limits.maxFramebufferWidth},
            {"maxFramebufferHeight", physicalDeviceInfo.properties.limits.maxFramebufferHeight},
            {"maxFramebufferLayers", physicalDeviceInfo.properties.limits.maxFramebufferLayers},
            {"framebufferColorSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStrVec(physicalDeviceInfo.properties.limits.framebufferColorSampleCounts)},
            {"framebufferDepthSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStrVec(physicalDeviceInfo.properties.limits.framebufferDepthSampleCounts)},
            {"framebufferStencilSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStrVec(physicalDeviceInfo.properties.limits.framebufferStencilSampleCounts)},
            {"framebufferNoAttachmentsSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStrVec(physicalDeviceInfo.properties.limits.framebufferNoAttachmentsSampleCounts)},
            {"maxColorAttachments", physicalDeviceInfo.properties.limits.maxColorAttachments},
            {"sampledImageColorSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStrVec(physicalDeviceInfo.properties.limits.sampledImageColorSampleCounts)},
            {"sampledImageIntegerSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStrVec(physicalDeviceInfo.properties.limits.sampledImageIntegerSampleCounts)},
            {"sampledImageDepthSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStrVec(physicalDeviceInfo.properties.limits.sampledImageDepthSampleCounts)},
            {"sampledImageStencilSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStrVec(physicalDeviceInfo.properties.limits.sampledImageStencilSampleCounts)},
            {"storageImageSampleCounts", lug::Graphics::Vulkan::API::RTTI::VkSampleCountFlagsToStrVec(physicalDeviceInfo.properties.limits.storageImageSampleCounts)},
            {"maxSampleMaskWords", physicalDeviceInfo.properties.limits.maxSampleMaskWords},
            {"timestampComputeAndGraphics", static_cast<bool>(physicalDeviceInfo.properties.limits.timestampComputeAndGraphics)},
            {"timestampPeriod", physicalDeviceInfo.properties.limits.timestampPeriod},
            {"maxClipDistances", physicalDeviceInfo.properties.limits.maxClipDistances},
            {"maxCullDistances", physicalDeviceInfo.properties.limits.maxCullDistances},
            {"maxCombinedClipAndCullDistances", physicalDeviceInfo.properties.limits.maxCombinedClipAndCullDistances},
            {"discreteQueuePriorities", physicalDeviceInfo.properties.limits.discreteQueuePriorities},
            {"pointSizeRange", std::vector<float>(std::begin(physicalDeviceInfo.properties.limits.pointSizeRange), std::end(physicalDeviceInfo.properties.limits.pointSizeRange))},
            {"lineWidthRange", std::vector<float>(std::begin(physicalDeviceInfo.properties.limits.lineWidthRange), std::end(physicalDeviceInfo.properties.limits.lineWidthRange))},
            {"pointSizeGranularity", physicalDeviceInfo.properties.limits.pointSizeGranularity},
            {"lineWidthGranularity", physicalDeviceInfo.properties.limits.lineWidthGranularity},
            {"strictLines", static_cast<bool>(physicalDeviceInfo.properties.limits.strictLines)},
            {"standardSampleLocations", static_cast<bool>(physicalDeviceInfo.properties.limits.standardSampleLocations)},
            {"optimalBufferCopyOffsetAlignment", physicalDeviceInfo.properties.limits.optimalBufferCopyOffsetAlignment},
            {"optimalBufferCopyRowPitchAlignment", physicalDeviceInfo.properties.limits.optimalBufferCopyRowPitchAlignment},
            {"nonCoherentAtomSize", physicalDeviceInfo.properties.limits.nonCoherentAtomSize}
        }},
        {"sparseProperties", {
            {"residencyStandard2DBlockShape", static_cast<bool>(physicalDeviceInfo.properties.sparseProperties.residencyStandard2DBlockShape)},
            {"residencyStandard2DMultisampleBlockShape", static_cast<bool>(physicalDeviceInfo.properties.sparseProperties.residencyStandard2DMultisampleBlockShape)},
            {"residencyStandard3DBlockShape", static_cast<bool>(physicalDeviceInfo.properties.sparseProperties.residencyStandard3DBlockShape)},
            {"residencyAlignedMipSize", static_cast<bool>(physicalDeviceInfo.properties.sparseProperties.residencyAlignedMipSize)},
            {"residencyNonResidentStrict", static_cast<bool>(physicalDeviceInfo.properties.sparseProperties.residencyNonResidentStrict)},
        }}
    };

    json["features"] = {
        {"robustBufferAccess", static_cast<bool>(physicalDeviceInfo.features.robustBufferAccess)},
        {"fullDrawIndexUint32", static_cast<bool>(physicalDeviceInfo.features.fullDrawIndexUint32)},
        {"imageCubeArray", static_cast<bool>(physicalDeviceInfo.features.imageCubeArray)},
        {"independentBlend", static_cast<bool>(physicalDeviceInfo.features.independentBlend)},
        {"geometryShader", static_cast<bool>(physicalDeviceInfo.features.geometryShader)},
        {"tessellationShader", static_cast<bool>(physicalDeviceInfo.features.tessellationShader)},
        {"sampleRateShading", static_cast<bool>(physicalDeviceInfo.features.sampleRateShading)},
        {"dualSrcBlend", static_cast<bool>(physicalDeviceInfo.features.dualSrcBlend)},
        {"logicOp", static_cast<bool>(physicalDeviceInfo.features.logicOp)},
        {"multiDrawIndirect", static_cast<bool>(physicalDeviceInfo.features.multiDrawIndirect)},
        {"drawIndirectFirstInstance", static_cast<bool>(physicalDeviceInfo.features.drawIndirectFirstInstance)},
        {"depthClamp", static_cast<bool>(physicalDeviceInfo.features.depthClamp)},
        {"depthBiasClamp", static_cast<bool>(physicalDeviceInfo.features.depthBiasClamp)},
        {"fillModeNonSolid", static_cast<bool>(physicalDeviceInfo.features.fillModeNonSolid)},
        {"depthBounds", static_cast<bool>(physicalDeviceInfo.features.depthBounds)},
        {"wideLines", static_cast<bool>(physicalDeviceInfo.features.wideLines)},
        {"largePoints", static_cast<bool>(physicalDeviceInfo.features.largePoints)},
        {"alphaToOne", static_cast<bool>(physicalDeviceInfo.features.alphaToOne)},
        {"multiViewport", static_cast<bool>(physicalDeviceInfo.features.multiViewport)},
        {"samplerAnisotropy", static_cast<bool>(physicalDeviceInfo.features.samplerAnisotropy)},
        {"textureCompressionETC2", static_cast<bool>(physicalDeviceInfo.features.textureCompressionETC2)},
        {"textureCompressionASTC_LDR", static_cast<bool>(physicalDeviceInfo.features.textureCompressionASTC_LDR)},
        {"textureCompressionBC", static_cast<bool>(physicalDeviceInfo.features.textureCompressionBC)},
        {"occlusionQueryPrecise", static_cast<bool>(physicalDeviceInfo.features.occlusionQueryPrecise)},
        {"pipelineStatisticsQuery", static_cast<bool>(physicalDeviceInfo.features.pipelineStatisticsQuery)},
        {"vertexPipelineStoresAndAtomics", static_cast<bool>(physicalDeviceInfo.features.vertexPipelineStoresAndAtomics)},
        {"fragmentStoresAndAtomics", static_cast<bool>(physicalDeviceInfo.features.fragmentStoresAndAtomics)},
        {"shaderTessellationAndGeometryPointSize", static_cast<bool>(physicalDeviceInfo.features.shaderTessellationAndGeometryPointSize)},
        {"shaderImageGatherExtended", static_cast<bool>(physicalDeviceInfo.features.shaderImageGatherExtended)},
        {"shaderStorageImageExtendedFormats", static_cast<bool>(physicalDeviceInfo.features.shaderStorageImageExtendedFormats)},
        {"shaderStorageImageMultisample", static_cast<bool>(physicalDeviceInfo.features.shaderStorageImageMultisample)},
        {"shaderStorageImageReadWithoutFormat", static_cast<bool>(physicalDeviceInfo.features.shaderStorageImageReadWithoutFormat)},
        {"shaderStorageImageWriteWithoutFormat", static_cast<bool>(physicalDeviceInfo.features.shaderStorageImageWriteWithoutFormat)},
        {"shaderUniformBufferArrayDynamicIndexing", static_cast<bool>(physicalDeviceInfo.features.shaderUniformBufferArrayDynamicIndexing)},
        {"shaderSampledImageArrayDynamicIndexing", static_cast<bool>(physicalDeviceInfo.features.shaderSampledImageArrayDynamicIndexing)},
        {"shaderStorageBufferArrayDynamicIndexing", static_cast<bool>(physicalDeviceInfo.features.shaderStorageBufferArrayDynamicIndexing)},
        {"shaderStorageImageArrayDynamicIndexing", static_cast<bool>(physicalDeviceInfo.features.shaderStorageImageArrayDynamicIndexing)},
        {"shaderClipDistance", static_cast<bool>(physicalDeviceInfo.features.shaderClipDistance)},
        {"shaderCullDistance", static_cast<bool>(physicalDeviceInfo.features.shaderCullDistance)},
        {"shaderFloat64", static_cast<bool>(physicalDeviceInfo.features.shaderFloat64)},
        {"shaderInt64", static_cast<bool>(physicalDeviceInfo.features.shaderInt64)},
        {"shaderInt16", static_cast<bool>(physicalDeviceInfo.features.shaderInt16)},
        {"shaderResourceResidency", static_cast<bool>(physicalDeviceInfo.features.shaderResourceResidency)},
        {"shaderResourceMinLod", static_cast<bool>(physicalDeviceInfo.features.shaderResourceMinLod)},
        {"sparseBinding", static_cast<bool>(physicalDeviceInfo.features.sparseBinding)},
        {"sparseResidencyBuffer", static_cast<bool>(physicalDeviceInfo.features.sparseResidencyBuffer)},
        {"sparseResidencyImage2D", static_cast<bool>(physicalDeviceInfo.features.sparseResidencyImage2D)},
        {"sparseResidencyImage3D", static_cast<bool>(physicalDeviceInfo.features.sparseResidencyImage3D)},
        {"sparseResidency2Samples", static_cast<bool>(physicalDeviceInfo.features.sparseResidency2Samples)},
        {"sparseResidency4Samples", static_cast<bool>(physicalDeviceInfo.features.sparseResidency4Samples)},
        {"sparseResidency8Samples", static_cast<bool>(physicalDeviceInfo.features.sparseResidency8Samples)},
        {"sparseResidency16Samples", static_cast<bool>(physicalDeviceInfo.features.sparseResidency16Samples)},
        {"sparseResidencyAliased", static_cast<bool>(physicalDeviceInfo.features.sparseResidencyAliased)},
        {"variableMultisampleRate", static_cast<bool>(physicalDeviceInfo.features.variableMultisampleRate)},
        {"inheritedQueries", static_cast<bool>(physicalDeviceInfo.features.inheritedQueries)},
    };

    json["memory"] = {
        {"memoryTypeCount", physicalDeviceInfo.memoryProperties.memoryTypeCount},
        {"memoryHeapCount", physicalDeviceInfo.memoryProperties.memoryHeapCount}
    };

    for (size_t i = 0; i < physicalDeviceInfo.memoryProperties.memoryTypeCount; ++i) {
        json["memory"]["memoryTypes"].push_back(
        {
            {"propertyFlags", lug::Graphics::Vulkan::API::RTTI::VkMemoryPropertyFlagsToStrVec(physicalDeviceInfo.memoryProperties.memoryTypes[i].propertyFlags)},
            {"heapIndex", physicalDeviceInfo.memoryProperties.memoryTypes[i].heapIndex},
        }
        );
    }

    for (size_t i = 0; i < physicalDeviceInfo.memoryProperties.memoryHeapCount; ++i) {
        json["memory"]["memoryHeaps"].push_back(
        {
            {"size", physicalDeviceInfo.memoryProperties.memoryHeaps[i].size},
            {"flags", lug::Graphics::Vulkan::API::RTTI::VkMemoryHeapFlagsToStrVec(physicalDeviceInfo.memoryProperties.memoryHeaps[i].flags)},
        }
        );
    }

    for (auto tmp : physicalDeviceInfo.queueFamilies) {
        json["queues"].push_back({
            {"queueFlags", lug::Graphics::Vulkan::API::RTTI::VkQueueFlagsToStrVec(tmp.queueFlags)},
            {"queueCount", tmp.queueCount},
            {"timestampValidBits", tmp.timestampValidBits},
            {"minImageTransferGranularity", {
                {"width", tmp.minImageTransferGranularity.width},
                {"height", tmp.minImageTransferGranularity.height},
                {"depth", tmp.minImageTransferGranularity.depth}
            }}
        });
    }

    for (auto extension : physicalDeviceInfo.extensions) {
        json["extensions"].push_back(
        {
            {"extensionName", extension.extensionName},
            {"specVersion", {
                {"major", static_cast<uint32_t>(lug::Core::Version::fromInt(extension.specVersion).major)},
                {"minor", static_cast<uint32_t>(lug::Core::Version::fromInt(extension.specVersion).minor)},
                {"patch", static_cast<uint32_t>(lug::Core::Version::fromInt(extension.specVersion).patch)}
            }},
        }
        );
    }

    for (auto format : physicalDeviceInfo.formatProperties) {
        json["formats"].push_back(
        {
            lug::Graphics::Vulkan::API::RTTI::toStr(format.first),
            {
                {"linearTilingFeatures", lug::Graphics::Vulkan::API::RTTI::VkFormatFeatureFlagsToStrVec(format.second.linearTilingFeatures)},
                {"optimalTilingFeatures", lug::Graphics::Vulkan::API::RTTI::VkFormatFeatureFlagsToStrVec(format.second.optimalTilingFeatures)},
                {"bufferFeatures", lug::Graphics::Vulkan::API::RTTI::VkFormatFeatureFlagsToStrVec(format.second.bufferFeatures)},
            }
        }
        );
    }

    json["Swapchain"].push_back(
    {
        {"minImageCount", physicalDeviceInfo.swapchain.capabilities.minImageCount},
        {"maxImageCount", physicalDeviceInfo.swapchain.capabilities.maxImageCount},
        {"currentExtent", {
            {"width", physicalDeviceInfo.swapchain.capabilities.currentExtent.width},
            {"height", physicalDeviceInfo.swapchain.capabilities.currentExtent.height}
        }},
        {"minImageExtent",{
            {"width", physicalDeviceInfo.swapchain.capabilities.minImageExtent.width},
            {"height", physicalDeviceInfo.swapchain.capabilities.minImageExtent.height}
        }},
        {"maxImageExtent", {
            {"width", physicalDeviceInfo.swapchain.capabilities.maxImageExtent.width},
            {"height", physicalDeviceInfo.swapchain.capabilities.maxImageExtent.height}
        }},
        {"maxImageArrayLayers", physicalDeviceInfo.swapchain.capabilities.maxImageArrayLayers},
    }
    );

    return json;
}
