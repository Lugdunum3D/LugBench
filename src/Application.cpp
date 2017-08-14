#include "Application.hpp"

#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Builder/Light.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

#include "BenchmarkingState.hpp"
#include "GPUInfoProvider.hpp"
#include "MenuState.hpp"

namespace LugBench {

Application::Application() : lug::Core::Application::Application{{"lugbench", {0, 1, 0}}} {
    std::srand((uint32_t)std::time(0));
    getRenderWindowInfo().windowInitInfo.title = "LugBench";

    getRenderWindowInfo().renderViewsInitInfo.push_back({
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
    vkRender->getDevice().waitIdle();
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

    // Create camera
    {
        lug::Graphics::Builder::Camera cameraBuilder(*renderer);

        cameraBuilder.setFovY(45.0f);
        cameraBuilder.setZNear(0.1f);
        cameraBuilder.setZFar(1000.0f);

        _camera = cameraBuilder.build();
        if (!_camera) {
            LUG_LOG.error("Application::init Can't create camera");
            return false;
        }
    }

    std::shared_ptr<AState> menuState;

    menuState = std::make_shared<MenuState>(*this);
    pushState(menuState);

    return true;
}

bool Application::initDevice(lug::Graphics::Vulkan::PhysicalDeviceInfo* choosenDevice) {
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);

    vkRender->getPreferences().device = choosenDevice;

    if (!lug::Core::Application::finishInit()) {
        return false;
    }

    const lug::Graphics::Vulkan::PhysicalDeviceInfo *physicalDeviceInfo = vkRender->getPhysicalDeviceInfo();
    if (physicalDeviceInfo == NULL) {
        return false;
    }
    return true;
}

bool Application::sendDevice(uint32_t nbFrames) {
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);

    lug::Graphics::Vulkan::PhysicalDeviceInfo *physicalDeviceInfo = vkRender->getPhysicalDeviceInfo();
    if (!physicalDeviceInfo) {
        return false;
    }

    nlohmann::json device;

    device["name"] = physicalDeviceInfo->properties.deviceName;

#if defined(LUG_SYSTEM_ANDROID)
    device["os"] = "Android";
#elif defined(LUG_SYSTEM_WINDOWS)
    device["os"] = "Windows";
#else
    device["os"] = "Linux";
#endif

    device["deviceId"] = physicalDeviceInfo->properties.deviceID;
    device["vendorId"] = physicalDeviceInfo->properties.vendorID;
    device["driverVersion"] = physicalDeviceInfo->properties.driverVersion;
    device["vulkanInfo"] = GPUInfoProvider::get(*physicalDeviceInfo);

    _nbFrames = nbFrames;
    LugBench::LugNetwork::getInstance().putDevice(device.dump());
    _isSendingDevice = true;
    return true;
}

void Application::sendScore() {
    nlohmann::json score;
    nlohmann::json lastResquestBody;

    lastResquestBody = nlohmann::json::parse(LugBench::LugNetwork::getInstance().getLastRequestBody());

    score["device"] = lastResquestBody["id"].get<std::string>();
    score["scenario"] = "595ed69c734d1d25634280b0";
    score["nbFrames"] = _nbFrames;
    score["averageFps"] = _nbFrames / 10.0f;

    LugBench::LugNetwork::getInstance().putScore(score.dump());
}

void Application::onFrame(const lug::System::Time& elapsedTime) {
    if (_states.empty()) {
        return;
    }
    if ((_isSendingDevice || _isSendingScore) && LugBench::LugNetwork::getInstance().getMutex().try_lock()) {
        LUG_LOG.info("status code : {}", LugBench::LugNetwork::getInstance().getLastRequestStatusCode());
        LUG_LOG.info("body : {}", LugBench::LugNetwork::getInstance().getLastRequestBody());
        LugBench::LugNetwork::getInstance().getMutex().unlock();

        if (_isSendingDevice) {
            _isSendingDevice = false;
            _isSendingScore = true;
            sendScore();
        } else {
            _isSendingScore = false;
        }
    }
    std::shared_ptr<AState> tmpState = _states.top();
    tmpState->onFrame(elapsedTime);
}

void Application::onEvent(const lug::Window::Event& event) {
    if (_states.empty()) {
        return;
    }
    std::shared_ptr<AState> tmpState = _states.top();
    tmpState->onEvent(event);
}

bool Application::haveState() {
    return (!_states.empty());
}

bool Application::popState() {
    if (!_states.top()->onPop()) {
        return false;
    }
    _states.pop();
    if (!_states.empty()) {
        _states.top()->onPlay();
    }
    return true;
}

bool Application::pushState(std::shared_ptr<AState> &state) {
    if (!_states.empty()) {
        _states.top()->onPause();
    }
    _states.push(state);
    return (_states.top()->onPush());
}

bool Application::popAndPushState(std::shared_ptr<AState> &state) {
    _states.pop();
    _states.push(state);
    return true;
}

} // LugBench
