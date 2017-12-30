#include "Application.hpp"

#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Texture.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

#include "GPUInfoProvider.hpp"
#include "ModelsState.hpp"
#include "BenchmarksState.hpp"

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

    _window_flags = 0;
    _window_flags |= ImGuiWindowFlags_NoTitleBar;
    _window_flags |= ImGuiWindowFlags_NoResize;
    _window_flags |= ImGuiWindowFlags_NoMove;
    _window_flags |= ImGuiWindowFlags_NoCollapse;
}

Application::~Application() {
    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
}

bool Application::init(int argc, char* argv[]) {
    if (!lug::Core::Application::init(argc, argv)) {
        return false;
    }

    lug::Graphics::Renderer* renderer = _graphics.getRenderer();
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

    if (!loadImages(renderer)) {
        return false;
    }

    std::shared_ptr<AState> menuState;

    menuState = std::make_shared<BenchmarksState>(*this);
    pushState(menuState);

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
        if (!(_epitechLogo = buildImage(renderer, "textures/epitech_logo.png"))) {
            LUG_LOG.error("Application: Can't create the epitech_logo texture");
            return false;
        }
        if (!(_epitechColorLogo = buildImage(renderer, "textures/epitech_color_logo.png"))) {
            LUG_LOG.error("Application: Can't create the epitech_logo texture");
            return false;
        }
        if (!(_gltfLogo = buildImage(renderer, "textures/gltf_logo.png"))) {
            LUG_LOG.error("Application: Can't create the gltf_logo texture");
            return false;
        }
        if (!(_vulkanLogo = buildImage(renderer, "textures/vulkan_logo.png"))) {
            LUG_LOG.error("Application: Can't create the vulkan_logo texture");
            return false;
        }
        if (!(_lugdunumLogo = buildImage(renderer, "textures/lugdunum_logo.png"))) {
            LUG_LOG.error("Application: Can't create the lugdunum_logo texture");
            return false;
        }
        if (!(_lugbenchLogo = buildImage(renderer, "textures/lugbench_logo.png"))) {
            LUG_LOG.error("Application: Can't create the lugbench_logo texture");
            return false;
        }
        if (!(_lugdunumShortLogo = buildImage(renderer, "textures/lugdunum-short.png"))) {
            LUG_LOG.error("Application: Can't create the lugdunum-short texture");
            return false;
        }
        if (!(_lugbenchShortLogo = buildImage(renderer, "textures/lugbench-short.png"))) {
            LUG_LOG.error("Application: Can't create the lugbench-short texture");
            return false;
        }
        if (!(_licenceLogo = buildImage(renderer, "textures/licence_logo.png"))) {
            LUG_LOG.error("Application: Can't create the licence_logo texture");
            return false;
        }
        if (!(_infoDeviceLogo = buildImage(renderer, "textures/info_device_logo.png"))) {
            LUG_LOG.error("Application: Can't create the vulkan_logo texture");
            return false;
        }
        if (!(_twitterLogo = buildImage(renderer, "textures/twitter_logo.png"))) {
            LUG_LOG.error("Application: Can't create the twitter_Logo texture");
            return false;
        }
        if (!(_githubLogo = buildImage(renderer, "textures/github_logo.png"))) {
            LUG_LOG.error("Application: Can't create the github_Logo texture");
            return false;
        }
        if (!(_messageIcon = buildImage(renderer, "textures/message_icon.png"))) {
            LUG_LOG.error("Application: Can't create the message_icon texture");
            return false;
        }
        if (!(_helmetThumbnail = buildImage(renderer, "textures/helmet_thumbnail.png")))
        {
            LUG_LOG.error("Application: Can't create the helmet_thumbnail texture");
            return false;
        }
    }

    return true;
}

lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> Application::buildImage(lug::Graphics::Renderer* renderer, std::string fileName)
{
    lug::Graphics::Builder::Texture textureBuilder(*renderer);

    textureBuilder.addLayer(fileName);
    textureBuilder.setMinFilter(lug::Graphics::Render::Texture::Filter::Linear);
    textureBuilder.setMagFilter(lug::Graphics::Render::Texture::Filter::Linear);

    return textureBuilder.build();
}

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
