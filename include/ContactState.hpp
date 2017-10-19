#pragma once

#include "AState.hpp"
#include "Application.hpp"

#include <lug/Graphics/Scene/Scene.hpp>
#include <json/json.hpp>
#include <imgui.h>

class ContactState : public AState {
public:
    ContactState() = delete;
    ContactState(LugBench::Application &application);
    ~ContactState();

    void onEvent(const lug::Window::Event& event) override;
    bool onFrame(const lug::System::Time& elapsedTime) override;
    bool onPop() override;
    bool onPush() override;

private:
	lug::Graphics::Vulkan::PhysicalDeviceInfo *_physicalDeviceInfo;

    // TEMP:(Stuart) Variables for "Window Editor" (temporary)

    bool _isOpen{ false };

};
