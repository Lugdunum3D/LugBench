#include <ctime>
#include <sstream>
#include "Application.hpp"
#include <VulkanInfoProvider.hpp>

#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

Application::Application() : lug::Core::Application::Application{{"hello", {0, 1, 0}}} {
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

    for (auto& queue: vkRender->getQueues()) {
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

    json["memory"] = {
        {"memoryTypeCount", physicalDeviceInfo->memoryProperties.memoryTypeCount},
        // {"memoryTypes", nlohmann::json::array()},
         // {"memoryTypes", std::vector<VkMemoryType>(std::begin(physicalDeviceInfo->memoryProperties.memoryTypes),
         //                                         std::end(physicalDeviceInfo->memoryProperties.memoryTypes))},

        {"memoryHeapCount", physicalDeviceInfo->memoryProperties.memoryHeapCount}
        //memoryHeaps
    };

    for (size_t i = 0; i < physicalDeviceInfo->memoryProperties.memoryTypeCount; i++)
    {
        // std::vector<const char *> plop = lug::Graphics::Vulkan::VkMemoryPropertyFlagsToStr(tmp.propertyFlags);
        // for (auto p : plop)
        //     std::cout << p << std::endl;
        // std::cout << "=======" << std::endl;
        json["memory"]["memoryTypes"].push_back(
            {
                {"heapIndex", physicalDeviceInfo->memoryProperties.memoryTypes[i].heapIndex},
                {"VkMemoryPropertyFlags", lug::Graphics::Vulkan::VkMemoryPropertyFlagsToStr(physicalDeviceInfo->memoryProperties.memoryTypes[i].propertyFlags)},
            }
        );
    }

    // std::vector<VkMemoryType> plop(1);
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
