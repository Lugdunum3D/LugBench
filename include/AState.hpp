#pragma once

#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Scene/Scene.hpp>
#include <lug/System/Time.hpp>
#include <lug/Window/Event.hpp>
#include "Application.hpp"

namespace LugBench {
    class Application;
} // LugBench

class AState {
public:
    AState() = delete;
    AState(LugBench::Application& application) : _application(application) {}
    virtual ~AState() {};

    virtual void onEvent(const lug::Window::Event& event) = 0;
    virtual bool onFrame(const lug::System::Time& elapsedTime) = 0;

    virtual bool onPlay() { return true; };
    virtual bool onPause() { return true; };
    virtual bool onPop() { return true; };
    virtual bool onPush() { return true; };

protected:
    LugBench::Application &_application;
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene> _scene;
};
