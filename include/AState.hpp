#pragma once

#include "Application.hpp"

class Application;

class AState {
public:
    AState() = delete;
    AState(Application& application) : _application(application) {}
    virtual ~AState() {};

    virtual void onEvent(const lug::Window::Event& event) = 0;
    virtual bool onFrame(const lug::System::Time& elapsedTime) = 0;

    virtual bool onPlay() { return true; };
    virtual bool onPause() { return true; };
    virtual bool onPop() { return true; };
    virtual bool onPush() { return true; };

protected:
   Application          &_application;
};
