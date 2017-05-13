#include <ctime>
#include <sstream>
#include "Application.hpp"
#include <VulkanInfoProvider.hpp>

#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include "restclient-cpp\restclient.h"
#include "Router.hpp"

Application::Application() : lug::Core::Application::Application{ {"hello", {0, 1, 0}} } {
    std::srand((uint32_t)std::time(0));
    getRenderWindowInfo().windowInitInfo.title = "LugBench";

    getRenderWindowInfo().renderViewsInitInfo.push_back({
        lug::Graphics::Render::Technique::Type::Forward,    // renderTechniqueType
        {                                                   // viewport
            {                                               // offset
                0.0f,                                       // x
                0.0f                                        // y
            },

            {                                               // extent
                1.0f,                                       // width
                1.0f                                        // height
            },

            0.0f,                                           // minDepth
            1.0f                                            // maxDepth
        },
        {                                                   // scissor
            {                                               // offset
                0.0f,                                       // x
                0.0f                                        // y
            },
            {                                               // extent
                1.0f,                                       // width
                1.0f                                        // height
            }
        },
        nullptr                                             // camera
    });
}

Application::~Application() {
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);

    for (auto& queue : vkRender->getQueues()) {
        queue.waitIdle();
    }
}

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::beginInit(argc, argv)) {
        return false;
    }

    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);

    for (auto& choosedDevice : vkRender->getPhysicalDeviceInfos()) {
        if (!initDevice(&choosedDevice)) {
            LUG_LOG.warn("Can't initialize the engine for the device {}", choosedDevice.properties.deviceName);
        }
    }

    return true;
}

bool Application::initDevice(lug::Graphics::Vulkan::PhysicalDeviceInfo* choosedDevice) {
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);

    vkRender->getPreferences().device = choosedDevice;

    if (!lug::Core::Application::finishInit()) {
        return false;
    }

    lug::Graphics::Vulkan::PhysicalDeviceInfo *physicalDeviceInfo = vkRender->getPhysicalDeviceInfo();

	VulkanInfoProvider vulkanInfoProvder(physicalDeviceInfo);
	nlohmann::json json  = vulkanInfoProvder.getJSONVulkanInfo();

    std::cout << json.dump(4) << std::endl;

    return (true);
}

void Application::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
        close();
    }
}

void Application::onFrame(const lug::System::Time&) {}
