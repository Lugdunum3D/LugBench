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
    void selectedButtonColorSet();
    void unselectedButtonColorSet();

    void displayAuthorsTab(const ImVec2& contactWindowSize);
    void displayLicenseTab();
    void displayContactTab(const ImVec2& contactWindowSize);

private:
    bool _authorsPageActive = true;
    bool _licencePageActive = false;
    bool _contactPageActive = false;

    std::vector<std::string> _autorNames;
    std::vector<std::string> _autorTitle;
};
