#include "Application.hpp"

#include <ctime>
#include <sstream>

#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Builder/Light.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

#include <GPUInfoProvider.hpp>

#include "MenuState.hpp"
#include "BenchmarkingState.hpp"
#include <StdThread.hpp>
#include <Network.hpp>

#include <json/json.hpp>

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

    menuState = std::make_shared<BenchmarkingState>(*this);
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

bool Application::sendResult(uint32_t nbFrames) {
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);

    lug::Graphics::Vulkan::PhysicalDeviceInfo *physicalDeviceInfo = vkRender->getPhysicalDeviceInfo();
    if (physicalDeviceInfo == NULL) {
        return false;
    }

    std::string deviceId;

    // sending device
    {
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

        _network.putDevice(device);
        isSendingDevice = true;
        _nbFrames = nbFrames;
        return true;
    }

}

void Application::onEvent(const lug::Window::Event& event) {

    if (_states.empty()) {
        return;
    }

    std::shared_ptr<AState> tmpState = _states.top(); // Needed to prevent fault if popping a state

    tmpState->onEvent(event);
}

void Application::getResponse() {
    if (_network._mutex.try_lock()) {
        if (isSendingDevice) {
            LUG_LOG.info("isSendingDevice response");
            LUG_LOG.info("Code : {}", std::get<0>(_network._response));
            LUG_LOG.info("Body : {}", std::get<1>(_network._response)["id"].get<std::string>());
            _deviceID = std::get<1>(_network._response)["id"].get<std::string>();
            _network._mutex.unlock();
            isSendingDevice = false;
            isSendingScore = true;
            _network._response = {};

            // sending score
            {
                nlohmann::json score;

                score["device"] = _deviceID;
                score["scenario"] = "595ed69c734d1d25634280b0";
                score["nbFrames"] = _nbFrames;
                score["averageFps"] = _nbFrames / 10.0f;

                _network.putScore(score);
            }
        } else {
            LUG_LOG.info("isSendingScore response");
            LUG_LOG.info("Code : {}", std::get<0>(_network._response));
            LUG_LOG.info("Body : {}", std::get<1>(_network._response)["id"].get<std::string>());
            _network._mutex.unlock();
            isSendingScore = false;
            _network._response = {};

        }
    }
}

void Application::onFrame(const lug::System::Time& elapsedTime) {

    if (_states.empty()) {
        return;
    }
    if (isSendingDevice || isSendingScore) {
        getResponse();
    }

    std::shared_ptr<AState> tmpState = _states.top(); // Needed to prevent fault if popping a state

    tmpState->onFrame(elapsedTime);
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
    if (!_states.top()->onPush()) {
        return false;
    }
    return true;
}

bool Application::popAndPushState(std::shared_ptr<AState> &state) {
    _states.pop();
    _states.push(state);
    return true;
}
