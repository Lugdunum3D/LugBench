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
    };

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
