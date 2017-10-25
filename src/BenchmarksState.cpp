#include "BenchmarksState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Builder/Texture.hpp>
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
    LUG_LOG.info("BenchmarksState destructor");
}

bool BenchmarksState::onPush() {
    return true;
}

bool BenchmarksState::onPop() {
    LUG_LOG.info("BenchmarksState onPop");
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
    _scene = nullptr;
    return true;
}

void BenchmarksState::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
        _application.close();
    }
}

bool BenchmarksState::onFrame(const lug::System::Time& /*elapsedTime*/) {
    _application.setCurrentState(State::BENCHMARKS);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    GUI::displayMenu(_application, window_flags);

    window_flags |= ImGuiWindowFlags_ShowBorders;

    lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

    float mainMenuHeight = static_cast<float>(window->getHeight()) / 18.f;

#if defined(LUG_SYSTEM_ANDROID)
    mainMenuHeight = (mainMenuHeight < 60.f * 2.f) ? 60.f * 2.f : mainMenuHeight;
#else
    mainMenuHeight = (mainMenuHeight < 60.f) ? 60.f : mainMenuHeight;
#endif

    ImGui::Begin("Model Select Menu", &_isOpen, window_flags);
    {
        ImVec2 modelMenuSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) - mainMenuHeight };

        ImGui::SetWindowSize(modelMenuSize);
        ImGui::SetWindowPos(ImVec2{ 0.f, mainMenuHeight });

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
                    ImVec2 buttonSize{ window->getWidth() / 10.f, window->getWidth() / 10.f };
                    ImVec2 fullSize{ window->getWidth() - buttonSize.x, buttonSize.y };
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
