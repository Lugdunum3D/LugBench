#pragma once

#include <memory>
#include <stack>

#include <lug/Core/Application.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

#include <AState.hpp>
#include <LugNetwork.hpp>

class AState;

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

    bool sendDevice(uint32_t frames, float elapsed);
    void sendScore();

    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Camera::Camera> getCamera();
    bool isSending() const;

private:
    bool initDevice(lug::Graphics::Vulkan::PhysicalDeviceInfo* choosenDevice);
    std::stack<std::shared_ptr<AState>> _states;

    lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene> _scene;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Camera::Camera> _camera;

    uint32_t _nbFrames{0};
    float _elapsed{0.0f};
    bool _isSendingDevice{false};
    bool _isSendingScore{false};

};

#include "Application.inl"

} // LugBench
