#pragma once

#include "AState.hpp"
#include "Application.hpp"

#include <lug/Graphics/Scene/Scene.hpp>

#include <lug/System/Time.hpp>
#include <lug/System/Clock.hpp>

class BenchmarkingState : public AState {
public:
    BenchmarkingState() = delete;
    BenchmarkingState(LugBench::Application &application);
    ~BenchmarkingState();

    void onEvent(const lug::Window::Event& event) override;
    bool onFrame(const lug::System::Time& elapsedTime) override;
    bool onPop() override;
    bool onPush() override;

private:
    float _rotation{0.0f};

    bool no_titlebar{true};
    bool no_border{true};
    bool no_resize{true};
    bool no_move{true};
    bool no_scrollbar{true};
    bool no_collapse{true};
    bool no_menu{true};

    float _elapsed = 0;
    uint32_t _frames = 0;

    lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene> _scene{nullptr};
};
