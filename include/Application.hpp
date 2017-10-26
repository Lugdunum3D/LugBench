#pragma once

#include <memory>
#include <stack>

#include <lug/Core/Application.hpp>
#include <lug/Graphics/Vulkan/Vulkan.hpp>

#include <AState.hpp>
#include <LugNetwork.hpp>

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

//  bool sendDevice(uint32_t frames, float elapsed);
//  void sendScore();
//  bool isSending() const;

    State getCurrentState() const;
    void setCurrentState(State);

    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Camera::Camera> getCamera();

private:
    bool initDevice(lug::Graphics::Vulkan::PhysicalDeviceInfo* choosenDevice);
    void loadFonts();
    std::stack<std::shared_ptr<AState>> _states;

    lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene> _scene;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Camera::Camera> _camera;

    State _currentState;

    // bool _isSendingDevice{false};
    // bool _isSendingScore{false};
};

#include "Application.inl"

} // LugBench
