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

    // Variables for "Sample Window" (temporary)
    bool *isOpen{false};

    bool no_menu_bar{false};
    bool no_titlebar{true};
    bool no_border{true};
    bool no_resize{true};
    bool no_move{true};
    bool no_scrollbar{true};
    bool no_collapse{true};
    bool no_menu{true};

    bool display_info_screen{false};
    bool display_result_screen{false};

};
