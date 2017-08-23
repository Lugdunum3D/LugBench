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
#include <thread>
#include <LugNetwork.hpp>

#include <json/json.hpp>
#include <IconsFontAwesome.h>

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

    // Generating custom font texture
    {
        ImGuiIO& io = ImGui::GetIO();
        // merge in icons from Font Awesome
        io.Fonts->AddFontDefault();
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        io.Fonts->AddFontFromFileTTF("./fonts/fontawesome-webfont.ttf", 16.0f, &icons_config, icons_ranges);
    }

    static_cast<lug::Graphics::Vulkan::Render::Window*>(renderer->getWindow())->initGui();

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

        _nbFrames = nbFrames;
        _network.putDevice(device.dump());
        _isSendingDevice = true;
    }
    return true;
}

void Application::onEvent(const lug::Window::Event& event) {

    if (_states.empty()) {
        return;
    }

    std::shared_ptr<AState> tmpState = _states.top(); // Needed to prevent fault if popping a state

    tmpState->onEvent(event);
}

void Application::sendScore() {

       // sending score
       {
            nlohmann::json score;
            nlohmann::json lastResquestBody;

            lastResquestBody = nlohmann::json::parse(_network.getLastRequestBody());

            score["device"] = lastResquestBody["id"].get<std::string>();
            score["scenario"] = "595ed69c734d1d25634280b0";
            score["nbFrames"] = _nbFrames;
            score["averageFps"] = _nbFrames / 10.0f;

            _network.putScore(score.dump());
       }
}

void Application::onFrame(const lug::System::Time& elapsedTime) {

    if (_states.empty()) {
        return;
    }
    if (_isSendingDevice) {
        LUG_LOG.info("_isSendingDevice is true");
        LUG_LOG.info(" plop status code {}", _network.getLastRequestStatusCode());
    }
    if (_isSendingDevice && _network.getLastRequestStatusCode() > 0) { 
        LUG_LOG.info("sendDevice status code : {}", _network.getLastRequestStatusCode()); 
        LUG_LOG.info("sendDevice body : {}", _network.getLastRequestBody()); 
        _isSendingDevice = false; 
        _isSendingScore = true; 
        sendScore();
    }
    if (!_isSendingDevice && _isSendingScore && _network.getLastRequestStatusCode() > 0) {
        LUG_LOG.info("sendScore status code : {}", _network.getLastRequestStatusCode());
        LUG_LOG.info("sendScore body : {}", _network.getLastRequestBody());
        _isSendingScore = false;
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
