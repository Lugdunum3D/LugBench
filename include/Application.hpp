#pragma once

#include <memory>
#include <stack>

#include <lug/Core/Application.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

#include "AState.hpp"

class AState;

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

    bool sendResult(uint32_t frames);

    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Camera::Camera> getCamera();

private:
    bool initDevice(lug::Graphics::Vulkan::PhysicalDeviceInfo* choosenDevice);
    std::stack<std::shared_ptr<AState>> _states;

    lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene> _scene;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Camera::Camera> _camera;
};

#include "Application.inl"
