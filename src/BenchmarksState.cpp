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
    LUG_LOG.info("BenchmarksState destructor");
}

bool BenchmarksState::onPush() {

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
        lightBuilder.setColor({1.0f, 1.0f, 1.0f, 1.0f});
        lightBuilder.setDirection({0.0f, 4.0f, -5.0f});

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
        lightBuilder.setColor({0.05f, 0.05f, 0.05f, 0.05f});

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
        node->setPosition({3.0f, 4.0f, 3.0f}, lug::Graphics::Node::TransformSpace::World);
        // Look at once
        node->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);

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

bool BenchmarksState::onPop() {
    LUG_LOG.info("BenchmarksState onPop");
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
    _scene = nullptr;
    return true;
}

void BenchmarksState::onEvent(const lug::Window::Event& event) {
    if (_application.isSending()) {
        return;
    }
    if (event.type == lug::Window::Event::Type::Close) {
        _application.close();
    }
}

bool BenchmarksState::onFrame(const lug::System::Time& elapsedTime) {
    _rotation += 0.05f * elapsedTime.getMilliseconds<float>();

    if (_rotation > 360.0f) {
        _rotation -= 360.0f;
    }

    float x = 3.0f * cos(lug::Math::Geometry::radians(_rotation));
    float y = 3.0f * sin(lug::Math::Geometry::radians(_rotation));

    _scene->getSceneNode("camera")->setPosition({x, 4.0f, y}, lug::Graphics::Node::TransformSpace::World);
    _scene->getSceneNode("camera")->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    if (_application.isSending()) {
        _sending_log_timer = 1.f;
        _sending_end_log_timer = 0.f;
        _display_sending_screen = true;
    }
    else if (_display_sending_screen == true) {
        _sending_log_timer = 2.f;
        _sending_end_log_timer = 3.f;
        _display_sending_screen = false;
    }

    if (_sending_log_timer > 0.f) {
        ImGui::Begin("Send Display", &_isOpen, window_flags);
        {
//            lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();
        
//            // Sets the window to be at the bottom of the screen (1/3rd of the height)
//            ImVec2 windowSize{ static_cast<float>(window->getWidth()), 30.f };
//            ImVec2 windowPos = { 0, 0 };
//            ImGui::SetWindowSize(windowSize);
//            ImGui::SetWindowPos(windowPos);
//            // Centers the button and keeps it square at all times
//            ImVec2 buttonSize{ windowSize.x - 10.0f , windowSize.y - 10.0f };        
//            ImGui::Button("Sending data in progress...", buttonSize);
        }
        ImGui::End();
        _sending_log_timer -= elapsedTime.getSeconds();
    }
    if (_sending_end_log_timer > 0.f) {
        ImGui::Begin("Send End Display", &_isOpen, window_flags);
        {
//            lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

            // Sets the window to be at the bottom of the screen (1/3rd of the height)
            float height = 0;
            if (_sending_log_timer > 0.f) {
                height += 25.f;
            }
//            ImVec2 windowSize{ static_cast<float>(window->getWidth()), 30.f };
//            ImVec2 windowPos = { 0, height };
//            ImGui::SetWindowSize(windowSize);
//            ImGui::SetWindowPos(windowPos);
//            // Centers the button and keeps it square at all times
//            ImVec2 buttonSize{ windowSize.x - 10.0f , windowSize.y - 10.0f };
//            ImGui::Button("Sending data completed!", buttonSize);
        }
        ImGui::End();
        _sending_end_log_timer -= elapsedTime.getSeconds();
    }


    if (_display_info_screen == false && _display_result_screen == false) {
        ImGui::Begin("Main Menu", &_isOpen, window_flags);
        {
            lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

            ImVec2 mainMenuSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) };
            ImVec2 mainMenuPos = { 0, 0 };

            ImGui::SetWindowSize(mainMenuSize);
            ImGui::SetWindowPos(mainMenuPos);
            ImGui::SetCursorPos(ImVec2{ 0.f, 0.f });

            float headerHeight = static_cast<float>(window->getHeight()) / 8.f;

#if defined(LUG_SYSTEM_ANDROID)
            headerHeight = (headerHeight < 60.f * 4.f) ? 60.f * 4.f : headerHeight;
#else
            headerHeight = (headerHeight < 60.f) ? 60.f : headerHeight;
#endif

            ImVec2 headerSize = { static_cast<float>(window->getWidth()), headerHeight };

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f,0.f });
            {
                ImGui::BeginChild("header", headerSize);
                {
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
                        ImGui::SetWindowFontScale(0.67f);

                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.31f, .67f, .98f, 1.00f));
                        {
#if defined(LUG_SYSTEM_ANDROID)
                            ImVec2 buttonSize{ 150.f * 2.75f, headerSize.y };
#else
                            ImVec2 buttonSize{ 150.f, headerSize.y };
#endif
                            ImGui::SameLine();
                            ImGui::Button("BENCHMARKS", buttonSize);
                        }
                        ImGui::PopStyleColor(2);

                        {
#if defined(LUG_SYSTEM_ANDROID)
                            ImVec2 buttonSize{ 100.f * 2.75f, headerSize.y };
#else
                            ImVec2 buttonSize{ 100.f, headerSize.y };
#endif
                            ImGui::SameLine();
                            if (ImGui::Button("MODELS", buttonSize)) {
                                if (_display_sending_screen == false) {
                                    std::shared_ptr<AState> benchmarkingState;
                                    benchmarkingState = std::make_shared<ModelsState>(_application);
                                    _application.popState();
                                    _application.pushState(benchmarkingState);
                                }
                                else {
                                    LUG_LOG.debug("Wait for previous logs to be sent");
                                }
                            }
                        }

                        {
#if defined(LUG_SYSTEM_ANDROID)
                            ImVec2 buttonSize{ 60.f * 2.75f, headerSize.y };
#else
                            ImVec2 buttonSize{ 60.f, headerSize.y };
#endif
                            ImGui::SameLine();
                            if (ImGui::Button("INFO", buttonSize)) {
                                std::shared_ptr<AState> benchmarkingState;
                                benchmarkingState = std::make_shared<InfoState>(_application);
                                _application.popState();
                                _application.pushState(benchmarkingState);
                            }
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
                    }
                    ImGui::EndChild();
                }
                ImGui::EndChild();
            }
            ImGui::PopStyleVar();
        }
        ImGui::End();
    } else if (_display_info_screen == true) {
        GUI::displayInfoScreen(&_isOpen, window_flags, _application.getGraphics().getRenderer()->getWindow(), _physicalDeviceInfo);
        if (_isOpen == false) {
            _display_info_screen = !_display_info_screen;
        }
    } else if (_display_result_screen == true) {
        if (_devices.size() == 0 && !_isReceiving) {
            _isReceiving = true;
            LugBench::LugNetwork::getInstance().makeRequest(LugBench::Method::GET,
                                                            LugBench::LugNetwork::urlToString(LugBench::Route::getScores));
            return true;
        }
        if (_isReceiving && LugBench::LugNetwork::getInstance().getMutex().try_lock()) {
            _isReceiving = false;
            nlohmann::json response = nlohmann::json::parse(
                    LugBench::LugNetwork::getInstance().getLastRequestBody());
            _devices = response["data"];
            LugBench::LugNetwork::getInstance().getMutex().unlock();
            return true;
        }

        GUI::displayResultScreen(&_isOpen, window_flags,
                                 _application.getGraphics().getRenderer()->getWindow(),
                                 _physicalDeviceInfo, &_devices);
        if (_isOpen == false) {
            _display_result_screen = !_display_result_screen;
        }
    }

    return true;
}
