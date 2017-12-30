#include "BenchmarksState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

#include "BenchmarkingState.hpp"

#include "ModelsState.hpp"


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
        ImGui::SetWindowFontScale(1.f);

        ImGui::PushStyleColor(ImGuiCol_Button, GUI::V4_WHITE);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, GUI::V4_SKYBLUE);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, GUI::V4_SKYBLUE);
        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, GUI::V4_WHITE);
        ImGui::PushStyleColor(ImGuiCol_Text, GUI::V4_DARKGRAY);
        {
            float buttonSide = GUI::Utilities::getPercentage(windowWidth, 0.12f, 200.f);
            ImVec2 buttonSize{ buttonSide, buttonSide };
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });
            {
                ImVec2 fullSize{ windowWidth - buttonSize.x, buttonSize.y };
                ImVec2 textSize{ 500.f, buttonSize.y };

                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                {
                    for (int i = 0; i < 3; ++i)
                    {
                        ImGui::PushID(i);
                        {
                            ImGui::BeginChild("Image", buttonSize);
                            {
                                auto vkTexture = lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(_application._helmetThumbnail);
                                ImGui::Image(vkTexture.get(), buttonSize, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
                            }
                            ImGui::EndChild();
                            ImGui::SameLine();
                            if (ImGui::Button("RUN BENCHMARK", buttonSize)) {
                                ImGui::PopID();
                                ImGui::PopFont();
                                ImGui::PopStyleVar();
                                ImGui::PopStyleColor(5);
                                ImGui::End();
                                std::shared_ptr<AState> modelState;
                                modelState = std::make_shared<ModelsState>(_application, "FireHydrant");
                                _application.popState();
                                _application.pushState(modelState);
                                static_cast<ModelsState*>(modelState.get())->benchmarkMode();
                                return true;
                            }
                            ImGui::SameLine();
                            ImGui::BeginChild("Result 1", buttonSize);
                            {
                                ImGui::Text("YOUR RESULT:");
                            }
                            ImGui::EndChild();
                        }
                        ImGui::PopID();
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
