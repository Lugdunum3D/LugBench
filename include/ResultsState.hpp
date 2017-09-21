#pragma once

#include "AState.hpp"
#include "Application.hpp"

#include <lug/Graphics/Scene/Scene.hpp>
#include <json/json.hpp>
#include <imgui.h>

class ResultsState : public AState {
public:
    ResultsState() = delete;
    ResultsState(LugBench::Application &application);
    ~ResultsState();

    void onEvent(const lug::Window::Event& event) override;
    bool onFrame(const lug::System::Time& elapsedTime) override;
    bool onPop() override;
    bool onPush() override;

private:
	lug::Graphics::Vulkan::PhysicalDeviceInfo *_physicalDeviceInfo;

    float _rotation{0.0f};

    bool _display_info_screen{false};
    bool _display_result_screen{false};
    bool _display_sending_screen{false};

    nlohmann::json _devices{};
    bool _isReceiving{false};

    float _sending_log_timer;
    float _sending_end_log_timer;

    // TEMP:(Stuart) Variables for "Window Editor" (temporary)

    bool _isOpen{ false };

};
