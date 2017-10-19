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
    LUG_LOG.info("ContactState destructor");
}

bool ContactState::onPush() {

    // Load scene
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Resource> sceneResource = renderer->getResourceManager()->loadFile("models/Duck/Duck.gltf");
    if (!sceneResource) {
        return false;
    }

    _scene = lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene>::cast(sceneResource);

    // Scale duck
    {
        _scene->getSceneNode("duck")->scale(lug::Math::Vec3f(0.01f));
    }

    // Attach directional light to the root node
    {
        lug::Graphics::Builder::Light lightBuilder(*renderer);

        lightBuilder.setType(lug::Graphics::Render::Light::Type::Directional);
        lightBuilder.setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
        lightBuilder.setDirection({ 0.0f, 4.0f, -5.0f });

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
        if (!light) {
            LUG_LOG.error("Application::init Can't create directional light");
            return false;
        }

        _scene->getRoot().attachLight(light);
    }

    // Attach ambient light to the root node
    {
        lug::Graphics::Builder::Light lightBuilder(*renderer);

        lightBuilder.setType(lug::Graphics::Render::Light::Type::Ambient);
        lightBuilder.setColor({ 0.05f, 0.05f, 0.05f, 0.05f });

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
        if (!light) {
            LUG_LOG.error("Application::init Can't create ambient light");
            return false;
        }

        _scene->getSceneNode("duck")->attachLight(light);
    }

    // Attach camera
    {
        lug::Graphics::Scene::Node* node = _scene->createSceneNode("camera");
        _scene->getRoot().attachChild(*node);

        node->attachCamera(_application.getCamera());

        // Set initial position of the camera
        node->setPosition({ 3.0f, 4.0f, 3.0f }, lug::Graphics::Node::TransformSpace::World);
        // Look at once
        node->getCamera()->lookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, lug::Graphics::Node::TransformSpace::World);

        // Attach camera to RenderView
        {
            auto& renderViews = renderer->getWindow()->getRenderViews();

            LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

            renderViews[0]->attachCamera(_application.getCamera());
        }
    }

    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);

    _physicalDeviceInfo = vkRender->getPhysicalDeviceInfo();

    if (!_physicalDeviceInfo) {
        return false;
    }
    return true;
}

bool ContactState::onPop() {
    LUG_LOG.info("ContactState onPop");
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
    _scene = nullptr;
    return true;
}

void ContactState::onEvent(const lug::Window::Event& event) {
    if (_application.isSending()) {
        return;
    }
    if (event.type == lug::Window::Event::Type::Close) {
        _application.close();
    }
}

bool ContactState::onFrame(const lug::System::Time& /*elapsedTime*/) {

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Main Menu", &_isOpen, window_flags);
    {
        lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

        ImVec2 mainMenuSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) };
        ImVec2 mainMenuPos = { 0, 0 };

        ImGui::SetWindowSize(mainMenuSize);
        ImGui::SetWindowPos(mainMenuPos);
        ImGui::SetCursorPos(ImVec2{ 0.f, 0.f });

        float headerHeight = static_cast<float>(window->getHeight()) / 18.f;
        headerHeight = (headerHeight < 60.f) ? 60.f : headerHeight;
        ImVec2 headerSize = { static_cast<float>(window->getWidth()), headerHeight };

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f,0.f });
        {
            ImGui::BeginChild("header", headerSize);
            {
#if defined(LUG_SYSTEM_ANDROID)
                ImGui::SetWindowFontScale(1.5f);
#else
                //ImGui::SetWindowFontScale(1.f);
#endif
                {
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.31f, .67f, .98f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.31f, .67f, .98f, 1.00f));
                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 170.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 170.f, headerSize.y };
#endif
                        ImGui::Button("LUGBENCH", buttonSize);
                    }
                    ImGui::PopStyleColor(2);
                }

                ImGui::SameLine();
                ImGui::BeginChild("clickable buttons", headerSize);
                {
#if defined(LUG_SYSTEM_ANDROID)
                    //ImGui::SetWindowFontScale(1.f);
#else
                    ImGui::SetWindowFontScale(0.67f);
#endif
                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 150.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 150.f, headerSize.y };
#endif
                        ImGui::SameLine();
                        if (ImGui::Button("BENCHMARKS", buttonSize)) {
                            LUG_LOG.debug("Start button pressed");
                            std::shared_ptr<AState> benchmarkingState;
                            benchmarkingState = std::make_shared<BenchmarksState>(_application);
                            _application.popState();
                            _application.pushState(benchmarkingState);
                        }
                    }

                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 100.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 100.f, headerSize.y };
#endif
                        ImGui::SameLine();
                        if (ImGui::Button("MODELS", buttonSize)) {
                            std::shared_ptr<AState> benchmarkingState;
                            benchmarkingState = std::make_shared<ModelsState>(_application);
                            _application.popState();
                            _application.pushState(benchmarkingState);
                        }
                    }

                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 60.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 60.f, headerSize.y };
#endif
                        ImGui::SameLine();
                        ImGui::Button("INFO", buttonSize);
                    }

                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 110.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 110.f, headerSize.y };
#endif
                        ImGui::SameLine();
                        if (ImGui::Button("RESULTS", buttonSize)) {
                            std::shared_ptr<AState> benchmarkingState;
                            benchmarkingState = std::make_shared<ResultsState>(_application);
                            _application.popState();
                            _application.pushState(benchmarkingState);
                        }
                    }

                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.31f, .67f, .98f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 110.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 110.f, headerSize.y };
#endif
                        ImGui::SameLine();
                        if (ImGui::Button("CONTACT", buttonSize)) {
                            std::shared_ptr<AState> benchmarkingState;
                            benchmarkingState = std::make_shared<ContactState>(_application);
                            _application.popState();
                            _application.pushState(benchmarkingState);
                        }
                    }
                    ImGui::PopStyleColor(2);
                }
                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleVar();
    }
    ImGui::End();
    return true;
}
