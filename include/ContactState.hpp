#pragma once

#include "AState.hpp"
#include "Application.hpp"

#include <lug/Graphics/Scene/Scene.hpp>
#include <json/json.hpp>
#include <imgui.h>

class ContactState : public AState {
private:
    struct LicenseInfo{
        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Texture> logo;
        std::string title;
        std::string text;
    };

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
    void displayLicenseTab(const ImVec2& contactWindowSize);
    void displayContactTab(const ImVec2& contactWindowSize);

private:
    bool _authorsPageActive = true;
    bool _licencePageActive = false;
    bool _contactPageActive = false;

    std::vector<std::string> _authorNames;
    std::vector<std::string> _authorTitle;
    std::vector<std::string> _authorEmails;
    std::vector<LicenseInfo> _licenses;
};
