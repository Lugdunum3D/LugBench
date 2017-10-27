#include "Application.hpp"

#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Texture.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

#include "BenchmarkingState.hpp"
#include "GPUInfoProvider.hpp"
#include "BenchmarksState.hpp"
#include "InfoState.hpp"

//#include <json/json.hpp>
#include <IconsFontAwesome.h>

#if defined(LUG_SYSTEM_ANDROID)
    #include <android/asset_manager.h>

    #include <lug/Window/Android/WindowImplAndroid.hpp>
#endif

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

    if (loadFonts() == false) {
        return false;
    }

    // Call this function only AFTER setting up your fonts
    static_cast<lug::Graphics::Vulkan::Render::Window*>(renderer->getWindow())->initGui();

    loadImages(renderer);

    std::shared_ptr<AState> menuState;

    menuState = std::make_shared<BenchmarksState>(*this);
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

bool Application::loadFonts() {
    // Generating custom font texture
    {
        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig icons_config;
        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

        // FONT 0
#if defined(LUG_SYSTEM_ANDROID)
        {
            AAsset* asset = AAssetManager_open((lug::Window::priv::WindowImpl::activity)->assetManager, "fonts/Roboto-Bold.ttf", AASSET_MODE_STREAMING);

            if (!asset) {
                LUG_LOG.error("Builder::ShaderModule: Can't open Android asset \"{}\"", "fonts/Roboto-Bold.ttf");
                return false;
            }

            size_t size = AAsset_getLength(asset);

            if (size <= 0) {
                LUG_LOG.error("Builder::ShaderModule: Android asset \"{}\" is empty", "fonts/Roboto-Bold.ttf");
                return false;
            }
            char* buff(new char[size]);

            AAsset_read(asset, buff, size);
            AAsset_close(asset);
            io.Fonts->AddFontFromMemoryTTF(std::move(buff), size, 72, &icons_config);
        }
#else
        io.Fonts->AddFontFromFileTTF("./fonts/Roboto-Bold.ttf", 36, &icons_config);
#endif
        icons_config.MergeMode = true;
#if defined(LUG_SYSTEM_ANDROID)
        {
            AAsset* asset = AAssetManager_open((lug::Window::priv::WindowImpl::activity)->assetManager, "fonts/fontawesome-webfont.ttf", AASSET_MODE_STREAMING);

            if (!asset) {
                LUG_LOG.error("Builder::ShaderModule: Can't open Android asset \"{}\"", "fonts/fontawesome-webfont.ttf");
                return false;
            }

            size_t size = AAsset_getLength(asset);

            if (size <= 0) {
                LUG_LOG.error("Builder::ShaderModule: Android asset \"{}\" is empty", "fonts/fontawesome-webfont.ttf");
                return false;
            }
            char* buff(new char[size]);

            AAsset_read(asset, buff, size);
            AAsset_close(asset);
            io.Fonts->AddFontFromMemoryTTF(std::move(buff), size, 72, &icons_config, icons_ranges);
        }
#else
        io.Fonts->AddFontFromFileTTF("./fonts/fontawesome-webfont.ttf", 36, &icons_config, icons_ranges);
#endif

        // FONT 1
        icons_config.MergeMode = false;
#if defined(LUG_SYSTEM_ANDROID)
        {
            AAsset* asset = AAssetManager_open((lug::Window::priv::WindowImpl::activity)->assetManager, "fonts/Roboto-Light.ttf", AASSET_MODE_STREAMING);

            if (!asset) {
                LUG_LOG.error("Builder::ShaderModule: Can't open Android asset \"{}\"", "fonts/Roboto-Light.ttf");
                return false;
            }

            size_t size = AAsset_getLength(asset);

            if (size <= 0) {
                LUG_LOG.error("Builder::ShaderModule: Android asset \"{}\" is empty", "fonts/Roboto-Light.ttf");
                return false;
            }
            char* buff(new char[size]);

            AAsset_read(asset, buff, size);
            AAsset_close(asset);
            io.Fonts->AddFontFromMemoryTTF(std::move(buff), size, 36, &icons_config);
        }
#else
        io.Fonts->AddFontFromFileTTF("./fonts/Roboto-Light.ttf", 18, &icons_config);
#endif
        icons_config.MergeMode = true;
#if defined(LUG_SYSTEM_ANDROID)
        {
            AAsset* asset = AAssetManager_open((lug::Window::priv::WindowImpl::activity)->assetManager, "fonts/fontawesome-webfont.ttf", AASSET_MODE_STREAMING);

            if (!asset) {
                LUG_LOG.error("Builder::ShaderModule: Can't open Android asset \"{}\"", "fonts/fontawesome-webfont.ttf");
                return false;
            }

            size_t size = AAsset_getLength(asset);

            if (size <= 0) {
                LUG_LOG.error("Builder::ShaderModule: Android asset \"{}\" is empty", "fonts/fontawesome-webfont.ttf");
                return false;
            }
            char* buff(new char[size]);

            AAsset_read(asset, buff, size);
            AAsset_close(asset);
            io.Fonts->AddFontFromMemoryTTF(std::move(buff), size, 36, &icons_config, icons_ranges);
        }
#else
        io.Fonts->AddFontFromFileTTF("./fonts/fontawesome-webfont.ttf", 18, &icons_config, icons_ranges);
#endif

        // FONT 2
        icons_config.MergeMode = false;
#if defined(LUG_SYSTEM_ANDROID)
        {
            AAsset* asset = AAssetManager_open((lug::Window::priv::WindowImpl::activity)->assetManager, "fonts/Roboto-Bold.ttf", AASSET_MODE_STREAMING);

            if (!asset) {
                LUG_LOG.error("Builder::ShaderModule: Can't open Android asset \"{}\"", "fonts/Roboto-Bold.ttf");
                return false;
            }

            size_t size = AAsset_getLength(asset);

            if (size <= 0) {
                LUG_LOG.error("Builder::ShaderModule: Android asset \"{}\" is empty", "fonts/Roboto-Bold.ttf");
                return false;
            }
            char* buff(new char[size]);

            AAsset_read(asset, buff, size);
            AAsset_close(asset);
            io.Fonts->AddFontFromMemoryTTF(std::move(buff), size, 36, &icons_config);
        }
#else
        io.Fonts->AddFontFromFileTTF("./fonts/Roboto-Bold.ttf", 18, &icons_config);
#endif
        icons_config.MergeMode = true;
#if defined(LUG_SYSTEM_ANDROID)
        {
            AAsset* asset = AAssetManager_open((lug::Window::priv::WindowImpl::activity)->assetManager, "fonts/fontawesome-webfont.ttf", AASSET_MODE_STREAMING);

            if (!asset) {
                LUG_LOG.error("Builder::ShaderModule: Can't open Android asset \"{}\"", "fonts/fontawesome-webfont.ttf");
                return false;
            }

            size_t size = AAsset_getLength(asset);

            if (size <= 0) {
                LUG_LOG.error("Builder::ShaderModule: Android asset \"{}\" is empty", "fonts/fontawesome-webfont.ttf");
                return false;
            }
            char* buff(new char[size]);

            AAsset_read(asset, buff, size);
            AAsset_close(asset);
            io.Fonts->AddFontFromMemoryTTF(std::move(buff), size, 36, &icons_config, icons_ranges);
        }
#else
        io.Fonts->AddFontFromFileTTF("./fonts/fontawesome-webfont.ttf", 18, &icons_config, icons_ranges);
#endif

    }
    return true;
}

bool Application::loadImages(lug::Graphics::Renderer* renderer) {
    // Load logos
    {
        // Epitech Logo
        {
            lug::Graphics::Builder::Texture textureBuilder(*renderer);

            textureBuilder.addLayer("textures/epitech_logo.png");
            textureBuilder.setMinFilter(lug::Graphics::Render::Texture::Filter::Linear);
            textureBuilder.setMagFilter(lug::Graphics::Render::Texture::Filter::Linear);

            _epitechLogo = textureBuilder.build();
            if (!_epitechLogo) {
                LUG_LOG.error("Application: Can't create the epitech_logo texture");
                return false;
            }
        }
        // GLTF Logo
        {
            lug::Graphics::Builder::Texture textureBuilder(*renderer);

            textureBuilder.addLayer("textures/gltf_logo.png");
            textureBuilder.setMinFilter(lug::Graphics::Render::Texture::Filter::Linear);
            textureBuilder.setMagFilter(lug::Graphics::Render::Texture::Filter::Linear);

            _gltfLogo = textureBuilder.build();
            if (!_gltfLogo) {
                LUG_LOG.error("Application: Can't create the gltf_logo texture");
                return false;
            }
        }
        // Vulkan Logo
        {
            lug::Graphics::Builder::Texture textureBuilder(*renderer);

            textureBuilder.addLayer("textures/vulkan_logo.png");
            textureBuilder.setMinFilter(lug::Graphics::Render::Texture::Filter::Linear);
            textureBuilder.setMagFilter(lug::Graphics::Render::Texture::Filter::Linear);

            _vulkanLogo = textureBuilder.build();
            if (!_vulkanLogo) {
                LUG_LOG.error("Application: Can't create the vulkan_logo texture");
                return false;
            }
        }
    }
    return true;
}

// bool Application::sendDevice(uint32_t nbFrames, float elapsed) {
//     lug::Graphics::Renderer* renderer = _graphics.getRenderer();
//     lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);

//     lug::Graphics::Vulkan::PhysicalDeviceInfo *physicalDeviceInfo = vkRender->getPhysicalDeviceInfo();
//     if (!physicalDeviceInfo) {
//         return false;
//     }

//     nlohmann::json device;

//     device["name"] = physicalDeviceInfo->properties.deviceName;

// #if defined(LUG_SYSTEM_ANDROID)
//     device["os"] = "Android";
// #elif defined(LUG_SYSTEM_WINDOWS)
//     device["os"] = "Windows";
// #else
//     device["os"] = "Linux";
// #endif

//     device["deviceId"] = physicalDeviceInfo->properties.deviceID;
//     device["vendorId"] = physicalDeviceInfo->properties.vendorID;
//     device["driverVersion"] = physicalDeviceInfo->properties.driverVersion;
//     device["vulkanInfo"] = GPUInfoProvider::get(*physicalDeviceInfo);

//     _nbFrames = nbFrames;
//     _elapsed = elapsed;
//     LugBench::LugNetwork::getInstance().makeRequest(LugBench::Method::POST,
//                                                     LugBench::LugNetwork::urlToString(LugBench::Route::sendDevice),
//                                                     device.dump());
//     _isSendingDevice = true;
//     return true;
// }

// void Application::sendScore() {
//     nlohmann::json score;
//     nlohmann::json lastResquestBody;

//     lastResquestBody = nlohmann::json::parse(LugBench::LugNetwork::getInstance().getLastRequestBody());

//     score["device"] = lastResquestBody["id"].get<std::string>();
//     score["scenario"] = "595ed69c734d1d25634280b0";
//     score["nbFrames"] = _nbFrames;
//     score["averageFps"] = _nbFrames / _elapsed;

//     LugBench::LugNetwork::getInstance().makeRequest(LugBench::Method::POST,
//                                                     LugBench::LugNetwork::urlToString(LugBench::Route::sendScore),
//                                                     score.dump());
// }

void Application::onFrame(const lug::System::Time& elapsedTime) {
    if (_states.empty()) {
        return;
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
