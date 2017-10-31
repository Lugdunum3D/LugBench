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

void ContactState::selectedButtonColorSet()
{
    ImGui::PushStyleColor(ImGuiCol_Button, GUI::V4_SKYBLUE);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, GUI::V4_SKYBLUE);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, GUI::V4_SKYBLUE);
    ImGui::PushStyleColor(ImGuiCol_Text, GUI::V4_WHITE);
}

void ContactState::unselectedButtonColorSet()
{
    ImGui::PushStyleColor(ImGuiCol_Button, GUI::V4_WHITE);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, GUI::V4_WHITE);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, GUI::V4_WHITE);
    ImGui::PushStyleColor(ImGuiCol_Text, GUI::V4_SKYBLUE);

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

    //    ImVec2 modelMenuSize{ static_cast<float>(windowWidth), windowHeight - (widowHeightOffset * 2) };
#if defined(LUG_SYSTEM_ANDROID)
    float contactWindowSelectWidth = GUI::Utilities::getPercentage(windowWidth, 0.125f, 165.f * 2.75f);
#else
    float contactWindowSelectWidth = GUI::Utilities::getPercentage(windowWidth, 0.125f, 165.f);
#endif
    ImVec2 contactWindowSelectSize = ImVec2{ contactWindowSelectWidth, windowHeight - (widowHeightOffset * 2) };

    ImGui::Begin("Contact Menu", 0, _application._window_flags);
    {
        ImGui::SetWindowSize(contactWindowSelectSize);
        ImGui::SetWindowPos(ImVec2{ 0.f, widowHeightOffset });

        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
            {

                float buttonWidth = ImGui::GetWindowWidth() / 1.25f;
#if defined(LUG_SYSTEM_ANDROID)
                float buttonHeight = 60.f * 2.75f;
#else
                float buttonHeight = 60.f;
#endif
                float xOffset = ImGui::GetWindowWidth() - (buttonWidth * 1.125f);
                float yOffset = (ImGui::GetWindowHeight() / 2.f);

                ImGui::SetCursorPos(ImVec2(xOffset, yOffset - (buttonHeight * 1.5f)));
                (AuthorsPageActive == true) ? selectedButtonColorSet() : unselectedButtonColorSet();
                if (ImGui::Button("Authors", ImVec2{ buttonWidth, buttonHeight }))
                {
                    AuthorsPageActive = true;
                    LicencePageActive = false;
                    ContactPageActive = false;
                }
                ImGui::SetCursorPos(ImVec2(xOffset, yOffset - (buttonHeight * .5f)));
                (LicencePageActive == true) ? selectedButtonColorSet() : unselectedButtonColorSet();
                if (ImGui::Button("Licence", ImVec2{ buttonWidth, buttonHeight }))
                {
                    AuthorsPageActive = false;
                    LicencePageActive = true;
                    ContactPageActive = false;
                }
                ImGui::SetCursorPos(ImVec2(xOffset, yOffset + (buttonHeight * .5f)));
                (ContactPageActive == true) ? selectedButtonColorSet() : unselectedButtonColorSet();
                if (ImGui::Button("Contact", ImVec2{ buttonWidth, buttonHeight }))
                {
                    AuthorsPageActive = false;
                    LicencePageActive = false;
                    ContactPageActive = true;
                }
                ImGui::PopStyleColor(12); // For the 4 PushStyleVar in each selectedButtonColorSet()/unselectedButtonColorSet()
            }
            ImGui::PopStyleVar();
        }
        ImGui::PopFont();
    }
    ImGui::End();

    ImVec2 contactWindowSize = ImVec2{ windowWidth - contactWindowSelectWidth, windowHeight - (widowHeightOffset * 2) };

    ImGui::PushStyleColor(ImGuiCol_WindowBg, GUI::V4_PINK);
    ImGui::Begin("Contact Window", 0, _application._window_flags);
    {
        ImGui::SetWindowSize(contactWindowSize);
        ImGui::SetWindowPos(ImVec2{ contactWindowSelectWidth, widowHeightOffset });
    }
    ImGui::End();
    ImGui::PopStyleColor();

    GUI::displayFooter(_application);

    return true;
}
