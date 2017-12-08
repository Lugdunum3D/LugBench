#include "BenchmarksState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

#include "BenchmarkingState.hpp"

#include "ModelsState.hpp"
#include "ContactState.hpp"
#include "ResultsState.hpp"
#include "InfoState.hpp"


#include <IconsFontAwesome.h>

BenchmarksState::BenchmarksState(LugBench::Application &application) : AState(application) {
    GUI::setDefaultStyle();
}

BenchmarksState::~BenchmarksState() {
}

bool BenchmarksState::onPush() {
    _application.setCurrentState(State::BENCHMARKS);
    return true;
}

bool BenchmarksState::onPop() {
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
    return true;
}

void BenchmarksState::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
        _application.close();
    }
}

bool BenchmarksState::onFrame(const lug::System::Time& /*elapsedTime*/) {
    lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();
    uint16_t windowHeight = window->getHeight();
    uint16_t windowWidth = window->getWidth();
    float windowHeaderOffset = GUI::displayMenu(_application);
    float windowFooterOffset = GUI::displayFooter(_application);

    ImGui::Begin("Model Select Menu", 0, _application._window_flags);
    {
        ImVec2 modelMenuSize{ static_cast<float>(windowWidth), windowHeight - (windowHeaderOffset + windowFooterOffset) };

        ImGui::SetWindowSize(modelMenuSize);
        ImGui::SetWindowPos(ImVec2{ 0.f, windowHeaderOffset });

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 1.f, 1.f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.31f, .67f, .98f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.31f, .67f, .98f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(1.f, 1.f, 1.f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f,0.f });
            {
                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                {
                    ImVec2 buttonSize{ windowWidth / 10.f, windowWidth / 10.f };
                    ImVec2 fullSize{ windowWidth - buttonSize.x, buttonSize.y };
                    ImVec2 textSize{ 500.f, buttonSize.y };

                    {
                        ImGui::BeginChild("Image 1", buttonSize);
                        {
                            ImGui::Button(ICON_FA_FLOPPY_O, buttonSize);
                        }
                        ImGui::EndChild();
                        ImGui::SameLine();
                        ImGui::BeginChild("Description 1", fullSize);
                        {
                            ImGui::BeginChild("Description Limiter 1", fullSize);
                            {
                                ImGui::SameLine(); ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
                            }
                            ImGui::EndChild();
                        }
                        ImGui::EndChild();
                    }

                    {
                        ImGui::BeginChild("Image 2", buttonSize);
                        {
                            ImGui::Button(ICON_FA_FLOPPY_O, buttonSize);
                        }
                        ImGui::EndChild();
                        ImGui::SameLine();
                        ImGui::BeginChild("Description 2", fullSize);
                        {
                            ImGui::BeginChild("Description Limiter 2", fullSize);
                            {
                                ImGui::SameLine(); ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
                            }
                            ImGui::EndChild();
                        }
                        ImGui::EndChild();
                    }

                    {
                        ImGui::BeginChild("Image 3", buttonSize);
                        {
                            ImGui::Button(ICON_FA_FLOPPY_O, buttonSize);
                        }
                        ImGui::EndChild();
                        ImGui::SameLine();
                        ImGui::BeginChild("Description 3", fullSize);
                        {
                            ImGui::BeginChild("Description Limiter 3", fullSize);
                            {
                                ImGui::SameLine(); ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
                            }
                            ImGui::EndChild();
                        }
                        ImGui::EndChild();
                    }
                }
                ImGui::PopFont();
            }
            ImGui::PopStyleVar();
        }
        ImGui::PopStyleColor(5);
    }
    ImGui::End();

    return true;
}
