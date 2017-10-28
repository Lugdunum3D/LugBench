#pragma once

#include "AState.hpp"
#include "Application.hpp"

#include <lug/Graphics/Scene/Scene.hpp>
#include <lug/Core/FreeMovement.hpp>
#include <json/json.hpp>
#include <imgui.h>

class ModelsState : public AState {
public:
    ModelsState() = delete;
    ModelsState(LugBench::Application &application);
    ~ModelsState();

    void onEvent(const lug::Window::Event& event) override;
    bool onFrame(const lug::System::Time& elapsedTime) override;
    bool onPop() override;
    bool onPush() override;

private:

    lug::Core::FreeMovement _cameraMover;
};
