#pragma once

#include "AState.hpp"
#include "Application.hpp"

#include <lug/Graphics/Scene/Scene.hpp>
#include <json/json.hpp>
#include <imgui.h>

class BenchmarksState : public AState {
private:
    struct SceneInfos {
        std::string sceneName;  
        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture>& thumbnail;
        int& framePerSecond;
    };

private:
    std::vector<SceneInfos> _scenes;

public:
    BenchmarksState() = delete;
    BenchmarksState(LugBench::Application &application);
    ~BenchmarksState();

    void onEvent(const lug::Window::Event& event) override;
    bool onFrame(const lug::System::Time& elapsedTime) override;
    bool onPop() override;
    bool onPush() override;
};
