#include <ctime>
#include <sstream>
#include "Application.hpp"
#include <VulkanInfoProvider.hpp>

#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

Application::Application() : lug::Core::Application::Application{ {"hello", {0, 1, 0}} } {
    std::srand((uint32_t)std::time(0));
    getRenderWindowInfo().windowInitInfo.title = "LugBench";

    getRenderWindowInfo().renderViewsInitInfo.push_back({
        lug::Graphics::RenderTechnique::Type::Forward,  // renderTechniqueType
        {                                               // viewport
            {                                           // offset
                0.0f,                                   // x
                0.0f                                    // y
            },

            {                                           // extent
                0.5f,                                   // width
                1.0f                                    // height
            },

            0.0f,                                       // minDepth
            1.0f                                        // maxDepth
        },
        {                                               // scissor
            {                                           // offset
                0.0f,                                   // x
                0.0f                                    // y
            },
            {                                           // extent
                1.0f,                                   // width
                1.0f                                    // height
            }
        },
        nullptr                                         // camera
    });

    getRenderWindowInfo().renderViewsInitInfo.push_back({
        lug::Graphics::RenderTechnique::Type::Forward,  // renderTechniqueType
        {                                               // viewport
            {                                           // offset
                0.5f,                                   // x
                0.0f                                    // y
            },

            {                                           // extent
                0.5f,                                   // width
                1.0f                                    // height
            },

            0.0f,                                       // minDepth
            1.0f                                        // maxDepth
        },
        {                                               // scissor
            {                                           // offset
                0.0f,                                   // x
                0.0f                                    // y
            },
            {                                           // extent
                1.0f,                                   // width
                1.0f                                    // height
            }
        },
        nullptr                                         // camera
    });

    getGraphicsInfo().rendererInitInfo.useDiscreteGPU = false;
}

Application::~Application() {
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);

    for (auto& queue : vkRender->getQueues()) {
        queue.waitIdle();
    }
}

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::init(argc, argv)) {
        return false;
    }
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    lug::Graphics::Vulkan::InstanceInfo instanceInfo = vkRender->getInstanceInfo();
    lug::Graphics::Vulkan::PhysicalDeviceInfo *physicalDeviceInfo = vkRender->getPhysicalDeviceInfo();
    
   

    nlohmann::json json;
    json["properties"] = {
        {"apiVersion", physicalDeviceInfo->properties.apiVersion},
        {"driverVersion", physicalDeviceInfo->properties.driverVersion},
        {"vendorID", physicalDeviceInfo->properties.vendorID},
        {"deviceID", physicalDeviceInfo->properties.deviceID},
        {"deviceType", physicalDeviceInfo->properties.deviceType},
        {"deviceName", physicalDeviceInfo->properties.deviceName},
        //        {"pipelineCacheUUID", std::vector<uint8_t>(physicalDeviceInfo->properties.pipelineCacheUUID, physicalDeviceInfo->properties.pipelineCacheUUID + sizeof(physicalDeviceInfo->properties.pipelineCacheUUID) / sizeof(physicalDeviceInfo->properties.pipelineCacheUUID[0]))},
        {"pipelineCacheUUID", std::vector<uint8_t>(std::begin(physicalDeviceInfo->properties.pipelineCacheUUID), std::end(physicalDeviceInfo->properties.pipelineCacheUUID))},
            {"limits", {
                {"maxImageDimension1D",physicalDeviceInfo->properties.limits.maxImageDimension1D},
                {"maxImageDimension2D" , physicalDeviceInfo->properties.limits.maxImageDimension2D },
                {"maxImageDimension3D" , physicalDeviceInfo->properties.limits.maxImageDimension3D },
                {"maxImageDimensionCube" , physicalDeviceInfo->properties.limits.maxImageDimensionCube},
                {"maxImageArrayLayers" ,  physicalDeviceInfo->properties.limits.maxImageArrayLayers },
                {"maxTexelBufferElements" , physicalDeviceInfo->properties.limits.maxTexelBufferElements },
                {"maxUniformBufferRange" , physicalDeviceInfo->properties.limits.maxUniformBufferRange },
                { "maxStorageBufferRange" , physicalDeviceInfo->properties.limits.maxStorageBufferRange },
                {"maxPushConstantsSize" , physicalDeviceInfo->properties.limits.maxPushConstantsSize },
                {"maxMemoryAllocationCount" ,physicalDeviceInfo->properties.limits.maxMemoryAllocationCount },
                {"maxSamplerAllocationCount" , physicalDeviceInfo->properties.limits.maxSamplerAllocationCount },
                {"bufferImageGranularity" , physicalDeviceInfo->properties.limits.bufferImageGranularity },
                {"sparseAddressSpaceSize" , physicalDeviceInfo->properties.limits.sparseAddressSpaceSize },
                {"maxBoundDescriptorSets" , physicalDeviceInfo->properties.limits.maxBoundDescriptorSets },
                {"maxPerStageDescriptorSamplers" , physicalDeviceInfo->properties.limits.maxPerStageDescriptorSamplers },
                {"maxPerStageDescriptorUniformBuffers" , physicalDeviceInfo->properties.limits.maxPerStageDescriptorUniformBuffers },
                {"maxPerStageDescriptorStorageBuffers" ,physicalDeviceInfo->properties.limits.maxPerStageDescriptorStorageBuffers },
                {"maxPerStageDescriptorSampledImages" , physicalDeviceInfo->properties.limits.maxPerStageDescriptorSampledImages },
                {"maxPerStageDescriptorStorageImages" , physicalDeviceInfo->properties.limits.maxPerStageDescriptorStorageImages },
                {"maxPerStageDescriptorInputAttachments" , physicalDeviceInfo->properties.limits.maxPerStageDescriptorInputAttachments },
                {"maxPerStageResources" , physicalDeviceInfo->properties.limits.maxPerStageResources },
                {"maxDescriptorSetSamplers" , physicalDeviceInfo->properties.limits.maxDescriptorSetSamplers },
                {"maxDescriptorSetUniformBuffers" ,physicalDeviceInfo->properties.limits.maxDescriptorSetUniformBuffers },
                {"maxDescriptorSetUniformBuffersDynamic" ,physicalDeviceInfo->properties.limits.maxDescriptorSetUniformBuffersDynamic },
                {"maxDescriptorSetStorageBuffers" ,physicalDeviceInfo->properties.limits.maxDescriptorSetStorageBuffers },
                {"maxDescriptorSetStorageBuffersDynamic" , physicalDeviceInfo->properties.limits.maxDescriptorSetStorageBuffersDynamic },
                {"maxDescriptorSetSampledImages" ,physicalDeviceInfo->properties.limits.maxDescriptorSetSampledImages },
                {"maxDescriptorSetStorageImages" , physicalDeviceInfo->properties.limits.maxDescriptorSetStorageImages },
                {"maxDescriptorSetInputAttachments" ,physicalDeviceInfo->properties.limits.maxDescriptorSetInputAttachments },
                {"maxVertexInputAttributes" , physicalDeviceInfo->properties.limits.maxVertexInputAttributes },
                {"maxVertexInputBindings" , physicalDeviceInfo->properties.limits.maxVertexInputBindings },
                {"maxVertexInputAttributeOffset" , physicalDeviceInfo->properties.limits.maxVertexInputAttributeOffset },
                {"maxVertexInputBindingStride" , physicalDeviceInfo->properties.limits.maxVertexInputBindingStride },
                {"maxVertexOutputComponents" , physicalDeviceInfo->properties.limits.maxVertexOutputComponents },
                {"maxTessellationGenerationLevel" , physicalDeviceInfo->properties.limits.maxTessellationGenerationLevel },
                {"maxTessellationPatchSize" , physicalDeviceInfo->properties.limits.maxTessellationPatchSize },
                {"maxTessellationControlPerVertexInputComponents" , physicalDeviceInfo->properties.limits.maxTessellationControlPerVertexInputComponents },
                {"maxTessellationControlPerVertexOutputComponents" ,physicalDeviceInfo->properties.limits.maxTessellationControlPerVertexOutputComponents },
                {"maxTessellationControlPerPatchOutputComponents" , physicalDeviceInfo->properties.limits.maxTessellationControlPerPatchOutputComponents },
                {"maxTessellationControlTotalOutputComponents" , physicalDeviceInfo->properties.limits.maxTessellationControlTotalOutputComponents },
                {"maxTessellationEvaluationInputComponents" , physicalDeviceInfo->properties.limits.maxTessellationEvaluationInputComponents },
                {"maxTessellationEvaluationOutputComponents" , physicalDeviceInfo->properties.limits.maxTessellationEvaluationOutputComponents },
                {"maxGeometryShaderInvocations" , physicalDeviceInfo->properties.limits.maxGeometryShaderInvocations },
                {"maxGeometryInputComponents" , physicalDeviceInfo->properties.limits.maxGeometryInputComponents },
                {"maxGeometryOutputComponents" , physicalDeviceInfo->properties.limits.maxGeometryOutputComponents },
                {"maxGeometryOutputVertices" , physicalDeviceInfo->properties.limits.maxGeometryOutputVertices },
                {"maxGeometryTotalOutputComponents" , physicalDeviceInfo->properties.limits.maxGeometryTotalOutputComponents },
                {"maxFragmentInputComponents" , physicalDeviceInfo->properties.limits.maxFragmentInputComponents },
                {"maxFragmentOutputAttachments" , physicalDeviceInfo->properties.limits.maxFragmentOutputAttachments },
                {"maxFragmentDualSrcAttachments" , physicalDeviceInfo->properties.limits.maxFragmentDualSrcAttachments },
                {"maxFragmentCombinedOutputResources" , physicalDeviceInfo->properties.limits.maxFragmentCombinedOutputResources },
                {"maxComputeSharedMemorySize" , physicalDeviceInfo->properties.limits.maxComputeSharedMemorySize },
                {"maxComputeWorkGroupCount" , physicalDeviceInfo->properties.limits.maxComputeWorkGroupCount },
                {"maxComputeWorkGroupInvocations" , physicalDeviceInfo->properties.limits.maxComputeWorkGroupInvocations },
                {"maxComputeWorkGroupSize" ,physicalDeviceInfo->properties.limits.maxComputeWorkGroupSize },
                {"subPixelPrecisionBits" , physicalDeviceInfo->properties.limits.subPixelPrecisionBits },
                {"subTexelPrecisionBits" ,physicalDeviceInfo->properties.limits.subTexelPrecisionBits },
                {"mipmapPrecisionBits" , physicalDeviceInfo->properties.limits.mipmapPrecisionBits },
                {"maxDrawIndexedIndexValue" ,physicalDeviceInfo->properties.limits.maxDrawIndexedIndexValue },
                {"maxDrawIndirectCount" , physicalDeviceInfo->properties.limits.maxDrawIndirectCount },
                {"maxSamplerLodBias" , physicalDeviceInfo->properties.limits.maxSamplerLodBias },
                {"maxSamplerAnisotropy" , physicalDeviceInfo->properties.limits.maxSamplerAnisotropy },
                {"maxViewports" , physicalDeviceInfo->properties.limits.maxViewports },
                {"maxViewportDimensions" ,physicalDeviceInfo->properties.limits.maxViewportDimensions },
                {"viewportBoundsRange" , physicalDeviceInfo->properties.limits.viewportBoundsRange },
                {"viewportSubPixelBits" , physicalDeviceInfo->properties.limits.viewportSubPixelBits },
                {"minMemoryMapAlignment" ,physicalDeviceInfo->properties.limits.minMemoryMapAlignment },
                {"minTexelBufferOffsetAlignment" ,physicalDeviceInfo->properties.limits.minTexelBufferOffsetAlignment },
                {"minUniformBufferOffsetAlignment" ,physicalDeviceInfo->properties.limits.minUniformBufferOffsetAlignment },
                {"minStorageBufferOffsetAlignment" ,physicalDeviceInfo->properties.limits.minStorageBufferOffsetAlignment },
                {"minTexelOffset" , physicalDeviceInfo->properties.limits.minTexelOffset },
                {"maxTexelOffset" , physicalDeviceInfo->properties.limits.maxTexelOffset },
                {"minTexelGatherOffset" ,physicalDeviceInfo->properties.limits.minTexelGatherOffset },
                {"maxTexelGatherOffset" ,physicalDeviceInfo->properties.limits.maxTexelGatherOffset },
                {"minInterpolationOffset" ,physicalDeviceInfo->properties.limits.minInterpolationOffset },
                {"maxInterpolationOffset" , physicalDeviceInfo->properties.limits.maxInterpolationOffset },
                {"subPixelInterpolationOffsetBits" , physicalDeviceInfo->properties.limits.subPixelInterpolationOffsetBits },
                {"maxFramebufferWidth" ,physicalDeviceInfo->properties.limits.maxFramebufferWidth },
                {"maxFramebufferHeight" , physicalDeviceInfo->properties.limits.maxFramebufferHeight },
                {"maxFramebufferLayers" , physicalDeviceInfo->properties.limits.maxFramebufferLayers },
                {"framebufferColorSampleCounts" , physicalDeviceInfo->properties.limits.framebufferColorSampleCounts },
                {"framebufferDepthSampleCounts" , physicalDeviceInfo->properties.limits.framebufferDepthSampleCounts },
                {"framebufferStencilSampleCounts" , physicalDeviceInfo->properties.limits.framebufferStencilSampleCounts },
                {"framebufferNoAttachmentsSampleCounts" , physicalDeviceInfo->properties.limits.framebufferNoAttachmentsSampleCounts },
                {"maxColorAttachments" , physicalDeviceInfo->properties.limits.maxColorAttachments },
                {"sampledImageColorSampleCounts" , physicalDeviceInfo->properties.limits.sampledImageColorSampleCounts },
                {"sampledImageIntegerSampleCounts" ,physicalDeviceInfo->properties.limits.sampledImageIntegerSampleCounts },
                {"sampledImageDepthSampleCounts" , physicalDeviceInfo->properties.limits.sampledImageDepthSampleCounts },
                {"sampledImageStencilSampleCounts" ,physicalDeviceInfo->properties.limits.sampledImageStencilSampleCounts },
                {"storageImageSampleCounts" , physicalDeviceInfo->properties.limits.storageImageSampleCounts },
                {"maxSampleMaskWords" , physicalDeviceInfo->properties.limits.maxSampleMaskWords },
                {"timestampComputeAndGraphics" ,physicalDeviceInfo->properties.limits.timestampComputeAndGraphics },
                {"timestampPeriod" , physicalDeviceInfo->properties.limits.timestampPeriod },
                {"maxClipDistances" , physicalDeviceInfo->properties.limits.maxClipDistances },
                {"maxCullDistances" , physicalDeviceInfo->properties.limits.maxCullDistances },
                {"maxCombinedClipAndCullDistances" , physicalDeviceInfo->properties.limits.maxCombinedClipAndCullDistances },
                {"discreteQueuePriorities" , physicalDeviceInfo->properties.limits.discreteQueuePriorities },
                {"pointSizeRange" ,physicalDeviceInfo->properties.limits.pointSizeRange },
                {"lineWidthRange" ,physicalDeviceInfo->properties.limits.lineWidthRange },
                {"pointSizeGranularity" ,physicalDeviceInfo->properties.limits.pointSizeGranularity },
                {"lineWidthGranularity" ,physicalDeviceInfo->properties.limits.lineWidthGranularity },
                {"strictLines" , physicalDeviceInfo->properties.limits.strictLines },
                {"standardSampleLocations" , physicalDeviceInfo->properties.limits.standardSampleLocations },
                {"optimalBufferCopyOffsetAlignment" , physicalDeviceInfo->properties.limits.optimalBufferCopyOffsetAlignment },
                {"optimalBufferCopyRowPitchAlignment" , physicalDeviceInfo->properties.limits.optimalBufferCopyRowPitchAlignment },
                {"nonCoherentAtomSize" , physicalDeviceInfo->properties.limits.nonCoherentAtomSize}
            }
        },
        {"sparseProperties",
            {
                {"residencyStandard2DBlockShape", physicalDeviceInfo->properties.sparseProperties.residencyStandard2DBlockShape},
                {"residencyStandard2DMultisampleBlockShape", physicalDeviceInfo->properties.sparseProperties.residencyStandard2DMultisampleBlockShape},
                {"residencyStandard3DBlockShape", physicalDeviceInfo->properties.sparseProperties.residencyStandard3DBlockShape},
                {"residencyAlignedMipSize", physicalDeviceInfo->properties.sparseProperties.residencyAlignedMipSize},
                {"residencyNonResidentStrict", physicalDeviceInfo->properties.sparseProperties.residencyNonResidentStrict},
            }
        },
    };

    json["features"] = {
        {"robustBufferAccess", physicalDeviceInfo->features.robustBufferAccess},
        {"fullDrawIndexUint32", physicalDeviceInfo->features.fullDrawIndexUint32},
        {"imageCubeArray", physicalDeviceInfo->features.imageCubeArray},
        {"independentBlend", physicalDeviceInfo->features.independentBlend},
        {"geometryShader", physicalDeviceInfo->features.geometryShader},
        {"tessellationShader", physicalDeviceInfo->features.tessellationShader},
        {"sampleRateShading", physicalDeviceInfo->features.sampleRateShading},
        {"dualSrcBlend", physicalDeviceInfo->features.dualSrcBlend},
        {"logicOp", physicalDeviceInfo->features.logicOp},
        {"multiDrawIndirect", physicalDeviceInfo->features.multiDrawIndirect},

        {"drawIndirectFirstInstance", physicalDeviceInfo->features.drawIndirectFirstInstance},
        {"depthClamp", physicalDeviceInfo->features.depthClamp},
        {"depthBiasClamp", physicalDeviceInfo->features.depthBiasClamp},
        {"fillModeNonSolid", physicalDeviceInfo->features.fillModeNonSolid},
        {"depthBounds", physicalDeviceInfo->features.depthBounds},
        {"wideLines", physicalDeviceInfo->features.wideLines},
        {"largePoints", physicalDeviceInfo->features.largePoints},
        {"alphaToOne", physicalDeviceInfo->features.alphaToOne},
        {"multiViewport", physicalDeviceInfo->features.multiViewport},
        {"samplerAnisotropy", physicalDeviceInfo->features.samplerAnisotropy},
        {"textureCompressionETC2", physicalDeviceInfo->features.textureCompressionETC2},
        {"textureCompressionASTC_LDR", physicalDeviceInfo->features.textureCompressionASTC_LDR},
        {"textureCompressionBC", physicalDeviceInfo->features.textureCompressionBC},
        {"occlusionQueryPrecise", physicalDeviceInfo->features.occlusionQueryPrecise},
        {"pipelineStatisticsQuery", physicalDeviceInfo->features.pipelineStatisticsQuery},

        {"vertexPipelineStoresAndAtomics", physicalDeviceInfo->features.vertexPipelineStoresAndAtomics},
        {"fragmentStoresAndAtomics", physicalDeviceInfo->features.fragmentStoresAndAtomics},
        {"shaderTessellationAndGeometryPointSize", physicalDeviceInfo->features.shaderTessellationAndGeometryPointSize},
        {"shaderImageGatherExtended", physicalDeviceInfo->features.shaderImageGatherExtended},
        {"shaderStorageImageExtendedFormats", physicalDeviceInfo->features.shaderStorageImageExtendedFormats},
        {"shaderStorageImageMultisample", physicalDeviceInfo->features.shaderStorageImageMultisample},
        {"shaderStorageImageReadWithoutFormat", physicalDeviceInfo->features.shaderStorageImageReadWithoutFormat},
        {"shaderStorageImageWriteWithoutFormat", physicalDeviceInfo->features.shaderStorageImageWriteWithoutFormat},


        {"shaderUniformBufferArrayDynamicIndexing", physicalDeviceInfo->features.shaderUniformBufferArrayDynamicIndexing},
        {"shaderSampledImageArrayDynamicIndexing", physicalDeviceInfo->features.shaderSampledImageArrayDynamicIndexing},
        {"shaderStorageBufferArrayDynamicIndexing", physicalDeviceInfo->features.shaderStorageBufferArrayDynamicIndexing},
        {"shaderStorageImageArrayDynamicIndexing", physicalDeviceInfo->features.shaderStorageImageArrayDynamicIndexing},
        {"shaderClipDistance", physicalDeviceInfo->features.shaderClipDistance},
        {"shaderCullDistance", physicalDeviceInfo->features.shaderCullDistance},
        {"shaderFloat64", physicalDeviceInfo->features.shaderFloat64},
        {"shaderInt64", physicalDeviceInfo->features.shaderInt64},
        {"shaderInt16", physicalDeviceInfo->features.shaderInt16},
        {"shaderResourceResidency", physicalDeviceInfo->features.shaderResourceResidency},
        {"shaderResourceMinLod", physicalDeviceInfo->features.shaderResourceMinLod},
        {"sparseBinding", physicalDeviceInfo->features.sparseBinding},

        {"sparseResidencyBuffer", physicalDeviceInfo->features.sparseResidencyBuffer},
        {"sparseResidencyImage2D", physicalDeviceInfo->features.sparseResidencyImage2D},
        {"sparseResidencyImage3D", physicalDeviceInfo->features.sparseResidencyImage3D},
        {"sparseResidency2Samples", physicalDeviceInfo->features.sparseResidency2Samples},
        {"sparseResidency4Samples", physicalDeviceInfo->features.sparseResidency4Samples},
        {"sparseResidency8Samples", physicalDeviceInfo->features.sparseResidency8Samples},
        {"sparseResidency16Samples", physicalDeviceInfo->features.sparseResidency16Samples},
        {"sparseResidencyAliased", physicalDeviceInfo->features.sparseResidencyAliased},
        {"variableMultisampleRate", physicalDeviceInfo->features.variableMultisampleRate},
        {"inheritedQueries", physicalDeviceInfo->features.inheritedQueries},



    };


    for (auto extension : physicalDeviceInfo->extensions)
        json["extension"].push_back(
            {
                { "extensionName", extension.extensionName },
                { "specVersion", extension.specVersion },
            }
        );
    
    for (auto layer : instanceInfo.layers)
        json["layers"].push_back(
    {
        { "layerName", layer.layerName },
        { "specVersion", layer.specVersion },
        {"implementationVersion", layer.implementationVersion},
        {"description", layer.description}
    }
    );

    size_t i = 0;

    for (auto format : physicalDeviceInfo->formatProperties)
    {   
        json["formats"].push_back(
        {
            { format.first,{
                    { "linearTilingFeatures", format.second.linearTilingFeatures},
                    { "optimalTilingFeatures", format.second.optimalTilingFeatures },
                    { "bufferFeatures", format.second.bufferFeatures},
                }
            } 
        }
        );
    }
  
        

    std::cout << json.dump(4) << std::endl;


    return (true);
}

void Application::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::EventType::CLOSE) {
        close();
    }
}

void Application::onFrame(const lug::System::Time& elapsedTime) {
    (void)elapsedTime;
    //_cubeNode->rotate(0.5f * elapsedTime, {0.0f, 1.0f, 0.0f});
}
