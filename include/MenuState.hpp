#pragma once

#include "AState.hpp"
#include "Application.hpp"

#include <lug/Graphics/Scene/Scene.hpp>

#include <imgui.h>

class MenuState : public AState {
public:
    MenuState() = delete;
    MenuState(Application &application);
    ~MenuState();

    void onEvent(const lug::Window::Event& event) override;
    bool onFrame(const lug::System::Time& elapsedTime) override;

private:
    std::unique_ptr<lug::Graphics::Scene::Scene> _scene;

    // Temporary store mesh because we don't have resource manager yet
    std::unique_ptr<lug::Graphics::Render::Model> _model;

    float _rotation{0.0f};

    // Variables for "Sample Window"
    bool *isOpen{false};

    bool no_titlebar{false};
    bool no_border{false};
    bool no_resize{false};
    bool no_move{false};
    bool no_scrollbar{false};
    bool no_collapse{false};
    bool no_menu{false};
    
    // Variables for "Main Menu"
    ImVec2 buttonPos{0,0};
};
