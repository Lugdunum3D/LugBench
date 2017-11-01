#include "ContactState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Graphics/Vulkan/Render/Texture.hpp>
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
    float widowHeaderOffset = GUI::displayMenu(_application);
    float widowFooterOffset = GUI::displayFooter(_application);

#if defined(LUG_SYSTEM_ANDROID)
    float contactWindowSelectWidth = GUI::Utilities::getPercentage(windowWidth, 0.125f, 165.f * 2.75f);
#else
    float contactWindowSelectWidth = GUI::Utilities::getPercentage(windowWidth, 0.125f, 165.f);
#endif
    ImVec2 contactWindowSelectSize = ImVec2{ contactWindowSelectWidth, windowHeight - (widowHeaderOffset + widowFooterOffset) };

    ImGui::Begin("Contact Menu", 0, _application._window_flags);
    {
        ImGui::SetWindowSize(contactWindowSelectSize);
        ImGui::SetWindowPos(ImVec2{ 0.f, widowHeaderOffset });

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
                (_authorsPageActive == true) ? selectedButtonColorSet() : unselectedButtonColorSet();
                if (ImGui::Button("Authors", ImVec2{ buttonWidth, buttonHeight })) {
                    _authorsPageActive = true;
                    _licencePageActive = false;
                    _contactPageActive = false;
                }
                ImGui::SetCursorPos(ImVec2(xOffset, yOffset - (buttonHeight * .5f)));
                (_licencePageActive == true) ? selectedButtonColorSet() : unselectedButtonColorSet();
                if (ImGui::Button("Licence", ImVec2{ buttonWidth, buttonHeight })) {
                    _authorsPageActive = false;
                    _licencePageActive = true;
                    _contactPageActive = false;
                }
                ImGui::SetCursorPos(ImVec2(xOffset, yOffset + (buttonHeight * .5f)));
                (_contactPageActive == true) ? selectedButtonColorSet() : unselectedButtonColorSet();
                if (ImGui::Button("Contact", ImVec2{ buttonWidth, buttonHeight })) {
                    _authorsPageActive = false;
                    _licencePageActive = false;
                    _contactPageActive = true;
                }
                ImGui::PopStyleColor(12); // For the 4 PushStyleVar in each selectedButtonColorSet()/unselectedButtonColorSet()
            }
            ImGui::PopStyleVar();
        }
        ImGui::PopFont();
    }
    ImGui::End();

    ImVec2 contactWindowSize = ImVec2{ windowWidth - contactWindowSelectWidth, windowHeight - (widowHeaderOffset + widowFooterOffset) };

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
    {
        ImGui::PushStyleColor(ImGuiCol_Text, GUI::V4_DARKGRAY);
        {
            ImGui::Begin("Contact Window", 0, _application._window_flags);
            {
                ImGui::SetWindowSize(contactWindowSize);
                ImGui::SetWindowPos(ImVec2{ contactWindowSelectWidth, widowHeaderOffset });
        
                ImGui::SetCursorPosY(15.f);
                ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, GUI::V4_WHITE);
                {
                    ImGui::BeginChild("Left Section", ImVec2{ contactWindowSize.x / 3.f, contactWindowSize.y - 15.f });
                    {
                        if (_authorsPageActive) {

                            auto vkTexture = lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(_application._epitechColorLogo);
#if defined(LUG_SYSTEM_ANDROID)
                            ImGui::Image(vkTexture.get(), ImVec2(275.f * 2, 100 * 2), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
#else
                            ImGui::Image(vkTexture.get(), ImVec2(275.f, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
#endif
                            ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");

                            ImGui::NewLine(); ImGui::NewLine();

                            vkTexture = lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(_application._lugbenchLogo);
#if defined(LUG_SYSTEM_ANDROID)
                            ImGui::Image(vkTexture.get(), ImVec2(301.f * 2, 100 * 2), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
#else
                            ImGui::Image(vkTexture.get(), ImVec2(301.f, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
#endif
                            vkTexture = lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(_application._lugdunumLogo);
#if defined(LUG_SYSTEM_ANDROID)
                            ImGui::Image(vkTexture.get(), ImVec2(301 * 2, 100 * 2), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
#else
                            ImGui::Image(vkTexture.get(), ImVec2(301, 100), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
#endif
                        } else if (_licencePageActive) {
                            ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
                        } else if (_contactPageActive) {
                            ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
                        }
                    }
                    ImGui::EndChild();
                    ImGui::SameLine();
                    ImGui::BeginChild("Right Section", ImVec2{ contactWindowSize.x * 0.6667f, contactWindowSize.y - 15.f });
                    {
                    }
                    ImGui::EndChild();
                }
                ImGui::PopStyleColor();
            }
            ImGui::End();
        }
        ImGui::PopStyleColor();
    }
    ImGui::PopFont();

    return true;
}
