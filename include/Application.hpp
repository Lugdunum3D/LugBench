#pragma once

#include <memory>
#include <stack>

#include <imgui.h>

#include <lug/Core/Application.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

#include "AState.hpp"

class AState;
enum class State : uint8_t;

namespace LugBench {

class Application : public ::lug::Core::Application {
public:
    Application();

    Application(const Application&) = delete;
    Application(Application&&) = delete;

    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    ~Application() override final;

    bool init(int argc, char* argv[]);

    void onEvent(const lug::Window::Event& event) override final;
    void onFrame(const lug::System::Time& elapsedTime) override final;

    bool haveState();
    bool popState();
    bool pushState(std::shared_ptr<AState> &state);
    bool popAndPushState(std::shared_ptr<AState> &state);

    State getCurrentState() const;
    void setCurrentState(State);

    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Camera::Camera> getCamera();

public:
    ImGuiWindowFlags _window_flags;

    lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene> _scene;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Camera::Camera> _camera;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _epitechLogo;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _epitechColorLogo;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _gltfLogo;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _vulkanLogo;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _lugbenchLogo;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _lugdunumLogo;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _lugbenchShortLogo;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _lugdunumShortLogo;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _licenceLogo;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _infoDeviceLogo;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _messageIcon;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _twitterLogo;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _githubLogo;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _helmetThumbnail;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _firehydrantThumbnail;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> _corsetThumbnail;

    int _helmetFps{-1};
    int _corsetFps{-1};
    int _fireHydrantFps{-1};

private:
    bool loadFonts();
    bool loadImages(lug::Graphics::Renderer* renderer);
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> buildImage(lug::Graphics::Renderer* renderer, std::string fileName);
    std::stack<std::shared_ptr<AState>> _states;

    State _currentState;
};

#include "Application.inl"

} // LugBench
