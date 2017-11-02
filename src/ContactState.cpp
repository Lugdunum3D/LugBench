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
    _autorNames.push_back("Yoann Long"); _autorTitle.push_back("Co-Founder & Project Lead");
    _autorNames.push_back("Quentin Buathier"); _autorTitle.push_back("Co-Founder & CTO");
    _autorNames.push_back("Nicolas Comte"); _autorTitle.push_back("Co-Founder & Communications Director");
    _autorNames.push_back("Antoine Bolvy"); _autorTitle.push_back("Co-Founder & Graphics Programmer");
    _autorNames.push_back("Nokitoo"); _autorTitle.push_back("Co-Founder");
    _autorNames.push_back("Nokitoo"); _autorTitle.push_back("Co-Founder");
    _autorNames.push_back("Nokitoo"); _autorTitle.push_back("Co-Founder");
    _autorNames.push_back("Nokitoo"); _autorTitle.push_back("Co-Founder");
    _autorNames.push_back("Nokitoo"); _autorTitle.push_back("Co-Founder");
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
    float windowHeaderOffset = GUI::displayMenu(_application);
    float windowFooterOffset = GUI::displayFooter(_application);

#if defined(LUG_SYSTEM_ANDROID)
    float contactWindowSelectWidth = GUI::Utilities::getPercentage(windowWidth, 0.125f, 165.f * 2.75f);
#else
    float contactWindowSelectWidth = GUI::Utilities::getPercentage(windowWidth, 0.125f, 165.f);
#endif
    ImVec2 contactWindowSelectSize = ImVec2{ contactWindowSelectWidth, windowHeight - (windowHeaderOffset + windowFooterOffset) };

    ImGui::Begin("Contact Menu", 0, _application._window_flags);
    {
        ImGui::SetWindowSize(contactWindowSelectSize);
        ImGui::SetWindowPos(ImVec2{ 0.f, windowHeaderOffset });

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

    ImVec2 contactWindowSize = ImVec2{ windowWidth - contactWindowSelectWidth, windowHeight - (windowHeaderOffset + windowFooterOffset) };

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
    {
        ImGui::PushStyleColor(ImGuiCol_Text, GUI::V4_DARKGRAY);
        {
            ImGui::Begin("Contact Window", 0, _application._window_flags);
            {
                ImGui::SetWindowSize(contactWindowSize);
                ImGui::SetWindowPos(ImVec2{ contactWindowSelectWidth, windowHeaderOffset });
        
                ImGui::SetCursorPosY(15.f);
                ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, GUI::V4_WHITE);
                {
                    float leftSectionWidth = GUI::Utilities::getPercentage(contactWindowSize.x, 0.33f, 301.f);
                    ImGui::BeginChild("Left Section", ImVec2{ leftSectionWidth, contactWindowSize.y - 15.f});
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

                    ImGui::SetCursorPosY(0.f);
                    float rightSectionWidth = GUI::Utilities::getPercentage(contactWindowSize.x - 60.f, 0.667f, contactWindowSize.x - leftSectionWidth);
                    rightSectionWidth = contactWindowSize.x - (leftSectionWidth + 10.f);
                    ImGui::BeginChild("Right Section", ImVec2{ rightSectionWidth, contactWindowSize.y - 15.f});
                    {
                        if (_authorsPageActive) {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 5.f, 2.f });
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });
                            {
                                ImVec2 uiSize = ImVec2{ rightSectionWidth, contactWindowSize.y };
                                ImVec2 childWindowHeaderSize;
#if defined(LUG_SYSTEM_ANDROID)
                                const float childMinWidth = 1000.f / 2.f;
                                const float childHeight = 400.f;
                                const float childSpacing = 30.f;
#else
                                const float childMinWidth = 405.f / 2.33f;
                                const float childHeight = 400.f / 2.33f;
                                const float childSpacing = 20.f;
                                const float minSizeDoubleElements = (childMinWidth * 2.f) + (childSpacing * 3.f);
                                const float minSizeTripleElements = (childMinWidth * 3.f) + (childSpacing * 5.f);
#endif

                                ImVec2 childWindowSize{ childMinWidth, childHeight };
#if defined(LUG_SYSTEM_ANDROID)
                                //
#else

                                if (uiSize.x >= minSizeTripleElements) {
                                    childWindowSize.x = GUI::Utilities::getPercentage(uiSize.x - (childSpacing * 3.f), 0.33f, childMinWidth);
                                    ImGui::SetCursorPos(ImVec2{ childSpacing, 20.f });
                                }
                                else if (uiSize.x >= minSizeDoubleElements) {
                                    childWindowSize.x = GUI::Utilities::getPercentage(uiSize.x - (childSpacing * 3.f), 0.5f, childMinWidth);
                                    ImGui::SetCursorPos(ImVec2{ childSpacing, 20.f });
                                }
                                else {
                                    childWindowSize.x = uiSize.x - (childSpacing * 2.f);
                                    ImGui::SetCursorPos(ImVec2{ childSpacing, 20.f });
                                }
#endif
                                for (int i = 0; i < _autorNames.size(); ++i)
                                {
                                    ImGui::PushID(i);
                                    {
                                        int row;
                                        int collumn;
#if defined(LUG_SYSTEM_ANDROID)
                                        {
                                            childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                                            float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                                            ImGui::SetCursorPos(ImVec2{ cursorPos, ((childHeaderHeight + childHeight) * 3.f) + (childSpacing * 4.f) });
                                        }
#else
                                        if (uiSize.x >= minSizeTripleElements) {
                                            row = int(i / 3);
                                            collumn = (i % 3);
                                            ImGui::SetCursorPos(ImVec2{ (childWindowSize.x * collumn) + childSpacing + (childSpacing * collumn), (childHeight * row) + (childSpacing * (row + 1)) });
                                        }
                                        else if (uiSize.x >= minSizeDoubleElements) {
                                            row = int(i / 2);
                                            collumn = (i % 2);
                                            ImGui::SetCursorPos(ImVec2{ (childWindowSize.x * collumn) + childSpacing + (childSpacing * collumn), (childHeight * row) + (childSpacing * (row + 1)) });
                                        }
                                        else {
                                            row = i;
                                            collumn = 0;
                                            ImGui::SetCursorPos(ImVec2{ childSpacing, (childHeight * row) + (childSpacing * (row + 1)) });
                                        }
#endif

                                        ImGui::BeginGroup();
                                        {
                                            // First window
                                            ImGui::PushStyleColor(ImGuiCol_Text, GUI::V4_SKYBLUE);
                                            ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, GUI::V4_LIGHTBLUE);
                                            ImGui::BeginChild("Author 1", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                                            {
                                                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                                                {
                                                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                                                    {
                                                        float text1Centered = ImGui::CalcTextSize(_autorNames[i].c_str()).x;
                                                        ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2.f) - (text1Centered / 2.f));
                                                        ImGui::Text(_autorNames[i].c_str());
                                                    }
                                                    ImGui::PopFont();
                                                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                                                    {
                                                        float text1Centered = ImGui::CalcTextSize(_autorTitle[i].c_str()).x;
                                                        ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2.f) - (text1Centered / 2.f));
                                                        ImGui::Text(_autorTitle[i].c_str());
                                                    }
                                                    ImGui::PopFont();
                                                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);
                                                    {
                                                        ImGui::PushStyleColor(ImGuiCol_Button, GUI::V4_LIGHTBLUE);
                                                        {
                                                            ImGui::SetCursorPosX((ImGui::GetWindowWidth() / 2.f) - (((3.f + 25.f) * 3.f) / 2.f));
                                                            ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 55.f);
                                                            ImGui::Button(ICON_FA_INSTAGRAM, ImVec2{ 25, 25 }); ImGui::SameLine();
                                                            ImGui::Button(ICON_FA_TWITTER, ImVec2{ 25, 25 }); ImGui::SameLine();
                                                            ImGui::Button(ICON_FA_FACEBOOK, ImVec2{ 25, 25 });
                                                        }
                                                        ImGui::PopStyleColor();
                                                    }
                                                    ImGui::PopStyleVar();
                                                }
                                                ImGui::PopStyleVar();
                                            }
                                            ImGui::EndChild();
                                            ImGui::PopStyleColor(2);
                                        }
                                        ImGui::EndGroup();
                                    }
                                    ImGui::PopID();
                                }
                            }
                            ImGui::PopStyleVar(2);
                        }
                        ImGui::PopStyleColor(2);
                        }
                         else if (_licencePageActive) {
                             ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
                         }
                         else if (_contactPageActive) {
                             ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
                         }
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
