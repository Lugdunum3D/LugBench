#include "ContactState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

#include "BenchmarkingState.hpp"

#include "ModelsState.hpp"
#include "ResultsState.hpp"
#include "InfoState.hpp"
#include "BenchmarksState.hpp"

#include <IconsFontAwesome.h>

ContactState::ContactState(LugBench::Application &application) : AState(application) {
    GUI::setDefaultStyle();
}

ContactState::~ContactState() {
}

bool ContactState::onPush() {
    _application.setCurrentState(State::CONTACT);
    return true;
}

bool ContactState::onPop() {
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
    _scene = nullptr;
    return true;
}

void ContactState::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
        _application.close();
    }
}

bool ContactState::onFrame(const lug::System::Time& /*elapsedTime*/) {
    lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();
    uint16_t windowHeight = window->getHeight();
    uint16_t windowWidth = window->getWidth();
    float widowHeightOffset = GUI::displayMenu(_application);

    ImVec2 modelMenuSize{ static_cast<float>(windowWidth), windowHeight - (widowHeightOffset * 2) };

    ImGui::Begin("Contact Menu", 0, _application._window_flags | ImGuiWindowFlags_ShowBorders);
    {

        ImGui::SetWindowSize(modelMenuSize);
        ImGui::SetWindowPos(ImVec2{ 0.f, widowHeightOffset });

        ImGui::BeginChild("Model Select Menu", ImVec2{ 165.f, windowHeight - (widowHeightOffset * 2) });
        {
#if defined(LUG_SYSTEM_ANDROID)
            float modelMenuWidth = GUI::Utilities::getPercentage(windowWidth, 0.125f, 165.f * 2.75f);
#else
            float modelMenuWidth = GUI::Utilities::getPercentage(windowWidth, 0.125f, 165.f);
#endif
            modelMenuSize = ImVec2{ modelMenuWidth, windowHeight - (widowHeightOffset * 2) };

//            ImGui::SetWindowSize(modelMenuSize);0
//            ImGui::SetWindowPos(ImVec2{ 0.f, mainMenuHeight });
            ImGui::SetWindowFontScale(0.67f);

            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 1.f, 1.f, 1.00f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.31f, .67f, .98f, 1.00f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.31f, .67f, .98f, 1.00f));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
            {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f,0.f });
                {

                    float buttonWidth = ImGui::GetWindowWidth();
                    float buttonHeight;
#if defined(LUG_SYSTEM_ANDROID)
                    buttonHeight = 80.f * 2.75f;
#else
                    buttonHeight = 80.f;
#endif

                    ImGui::Button("Duck", ImVec2{ buttonWidth, buttonHeight });
                    ImGui::Button("Helmet", ImVec2{ buttonWidth, buttonHeight });
                    ImGui::Button("Monkey", ImVec2{ buttonWidth, buttonHeight });
                    ImGui::Button("Repunzel", ImVec2{ buttonWidth, buttonHeight });
                    ImGui::Button("Tower", ImVec2{ buttonWidth, buttonHeight });
                }
                ImGui::PopStyleVar();
            }
            ImGui::PopStyleColor(4);
        }
        ImGui::EndChild();
    }
    ImGui::End();

    GUI::displayFooter(_application);

    return true;
}
