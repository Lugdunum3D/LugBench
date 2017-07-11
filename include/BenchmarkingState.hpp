#pragma once

#include "AState.hpp"
#include "Application.hpp"

#include <lug/Graphics/Scene/Scene.hpp>

#include <lug/System/Time.hpp>
#include <lug/System/Clock.hpp>

class BenchmarkingState : public AState {
public:
    BenchmarkingState() = delete;
    BenchmarkingState(Application &application);
    ~BenchmarkingState();

    void onEvent(const lug::Window::Event& event) override;
    bool onFrame(const lug::System::Time& elapsedTime) override;
    bool onPop() override;
    bool onPush() override;

private:
    const lug::Graphics::Vulkan::PhysicalDeviceInfo *_physicalDeviceInfo;

    float _rotation{0.0f};

    // Variables for "Sample Window"
    bool isOpen{false};

    bool no_titlebar{true};
    bool no_border{true};
    bool no_resize{true};
    bool no_move{true};
    bool no_scrollbar{true};
    bool no_collapse{true};
    bool no_menu{true};

    float elapsed = 0;
    uint32_t frames = 0;
    float _fps = 1.0f;
};
