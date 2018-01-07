#include "BenchmarksState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

#include "ModelsState.hpp"

BenchmarksState::BenchmarksState(LugBench::Application &application) : AState(application) {
    GUI::setDefaultStyle();

    _scenes.push_back({ "Helmet", _application._helmetThumbnail, _application._helmetFps });
    _scenes.push_back({ "FireHydrant", _application._firehydrantThumbnail, _application._fireHydrantFps });
    _scenes.push_back({ "Corset", _application._corsetThumbnail, _application._corsetFps });
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
            if (_sceneResult == nullptr) {
                float imageSide = GUI::Utilities::getPercentage(windowWidth, 0.12f, 200.f);
                ImVec2 imageSize{ imageSide, imageSide };
                ImVec2 buttonSize{ imageSide * 1.5f, imageSide };
                ImVec2 textSize{ imageSide * 1.5f, imageSide };
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });
                {
                    ImVec2 fullSize{ windowWidth - buttonSize.x, buttonSize.y };

                    int i = 0;
                    for (auto & scene : _scenes)
                    {
                        ++i;
                        ImGui::PushID(i);
                        {
                            auto vkTexture = lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(scene.thumbnail);
                            ImGui::Image(vkTexture.get(), imageSize, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
                            ImGui::SameLine();
                            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                            {
                                if (ImGui::Button("RUN BENCHMARK", buttonSize)) {
                                    ImGui::PopID();
                                    ImGui::PopFont();
                                    ImGui::PopStyleVar();
                                    ImGui::PopStyleColor(5);
                                    ImGui::End();
                                    std::shared_ptr<AState> modelState;
                                    modelState = std::make_shared<ModelsState>(_application, scene.sceneName);
                                    static_cast<ModelsState*>(modelState.get())->benchmarkMode(&scene.framePerSecond);
                                    _application.popState();
                                    _application.pushState(modelState);
                                    return true;
                                }
                            }
                            ImGui::PopFont();
                            ImGui::SameLine();
                            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[3]);
                            {
                                ImGui::BeginChild("Result", textSize);
                                {
                                    const ImVec2 fpsTextSize = ImGui::CalcTextSize("000 Average FPS");
                                    ImGui::SetCursorPosY((textSize.y - ImGui::CalcTextSize("N/A").y) / 2.f);
                                    ImGui::SetCursorPosX((textSize.x - fpsTextSize.x) / 2.f);
                                    if (scene.framePerSecond == -1.f) {
                                        ImGui::Text("N/A Average FPS");
                                    }
                                    else {
                                        ImGui::Text("%d Average FPS", scene.framePerSecond);
                                    }
                                }
                                ImGui::EndChild();
                            }
                            ImGui::PopFont();

                            if (scene.framePerSecond != -1.f) {
                                ImGui::SameLine();
                                ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                                {
                                    if (ImGui::Button("COMPARE", buttonSize)) {
                                        _sceneResult = &scene;
                                    }
                                }
                                ImGui::PopFont();
                            }
                        }
                        ImGui::PopID();
                    }
                }
                ImGui::PopStyleVar();
            } else {
                float resultWidth = GUI::Utilities::getPercentage(modelMenuSize.x, .85f);
                float resultEdge = GUI::Utilities::getPercentage(modelMenuSize.x, .15f);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (resultEdge / 2));
                ImGui::BeginChild("Result", ImVec2(resultWidth, modelMenuSize.y));
                {
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        if (ImGui::Button("Return", ImVec2(120, 60))) {
                            _sceneResult = nullptr;
                        } else {
                            GUI::displayScoreInCell(_sceneResult->sceneName.c_str(), _sceneResult->framePerSecond, static_cast<float>(_sceneResult->framePerSecond));
                        }
                    }
                    ImGui::PopFont();
                }
                ImGui::EndChild();
            }
        }
        ImGui::PopStyleColor(5);
    }
    ImGui::End();

    return true;
}
