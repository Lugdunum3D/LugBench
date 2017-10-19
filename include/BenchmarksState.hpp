#pragma once

#include "AState.hpp"
#include "Application.hpp"

#include <lug/Graphics/Scene/Scene.hpp>
#include <json/json.hpp>
#include <imgui.h>

class BenchmarksState : public AState {
public:
    BenchmarksState() = delete;
    BenchmarksState(LugBench::Application &application);
    ~BenchmarksState();

    void onEvent(const lug::Window::Event& event) override;
    bool onFrame(const lug::System::Time& elapsedTime) override;
    bool onPop() override;
    bool onPush() override;

private:
    // TEMP:(Stuart) Variables for "Window Editor" (temporary)

    bool _isOpen{ false };

    lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> baseColorTexture;
};
