#include "MenuState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

#include "BenchmarkingState.hpp"

#include <IconsFontAwesome.h>
//
//MenuState::MenuState(Application &application) : AState(application) {
MenuState::MenuState(LugBench::Application &application) : AState(application) {

    ImGuiStyle& style = ImGui::GetStyle();
    style.ChildWindowRounding = 0.f;
    style.WindowRounding = 0.f;
    style.WindowPadding.x = 5.f;
    style.Colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f); //
    style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f); //
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_ComboBg] = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.64f, 0.87f, 0.25f, 1.00f); //
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.58f, 0.58f, 0.58f, 1.f); //
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style.Colors[ImGuiCol_CloseButton] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
    style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
    style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
    style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
}

MenuState::~MenuState() {
    LUG_LOG.info("MenuState destructor");
}

bool MenuState::onPush() {

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

bool MenuState::onPop() {
    LUG_LOG.info("MenuState onPop");
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
    _scene = nullptr;
    return true;
}

void MenuState::onEvent(const lug::Window::Event& event) {
    if (_application.isSending()) {
        return;
    }
    if (event.type == lug::Window::Event::Type::Close) {
        _application.close();
    }
}

bool MenuState::onFrame(const lug::System::Time& elapsedTime) {
    _rotation += 0.05f * elapsedTime.getMilliseconds<float>();

    if (_rotation > 360.0f) {
        _rotation -= 360.0f;
    }

    float x = 3.0f * cos(lug::Math::Geometry::radians(_rotation));
    float y = 3.0f * sin(lug::Math::Geometry::radians(_rotation));

    _scene->getSceneNode("camera")->setPosition({x, 4.0f, y}, lug::Graphics::Node::TransformSpace::World);
    _scene->getSceneNode("camera")->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);

    ImGuiWindowFlags window_flags = 0;
    if (_no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (!_no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
    if (_no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
    if (_no_move)      window_flags |= ImGuiWindowFlags_NoMove;
    if (_no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (_no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
    if (!_no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;

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
            lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();
        
            // Sets the window to be at the bottom of the screen (1/3rd of the height)
            ImVec2 windowSize{ static_cast<float>(window->getWidth()), 30.f };
            ImVec2 windowPos = { 0, 0 };
            ImGui::SetWindowSize(windowSize);
            ImGui::SetWindowPos(windowPos);
            // Centers the button and keeps it square at all times
            ImVec2 buttonSize{ windowSize.x - 10.0f , windowSize.y - 10.0f };
        
            ImGui::Button("Sending data in progress...", buttonSize);
        }
        ImGui::End();
        _sending_log_timer -= elapsedTime.getSeconds();
    }
    if (_sending_end_log_timer > 0.f) {
        ImGui::Begin("Send End Display", &_isOpen, window_flags);
        {
            lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

            // Sets the window to be at the bottom of the screen (1/3rd of the height)
            float height = 0;
            if (_sending_log_timer > 0.f) {
                height += 25.f;
            }
            ImVec2 windowSize{ static_cast<float>(window->getWidth()), 30.f };
            ImVec2 windowPos = { 0, height };
            ImGui::SetWindowSize(windowSize);
            ImGui::SetWindowPos(windowPos);
            // Centers the button and keeps it square at all times
            ImVec2 buttonSize{ windowSize.x - 10.0f , windowSize.y - 10.0f };
            ImGui::Button("Sending data completed!", buttonSize);
        }
        ImGui::End();
        _sending_end_log_timer -= elapsedTime.getSeconds();
    }


    if (_display_info_screen == false && _display_result_screen == false) {
        ImGui::Begin("Sample Window", &_isOpen, window_flags);
        {
            ImGui::Checkbox("No Menu Bar", &_no_menu_bar);
            ImGui::NewLine();
            ImGui::Checkbox("No Titlebar", &_no_titlebar);
            ImGui::Checkbox("No Border", &_no_border);
            ImGui::Checkbox("No Resize", &_no_resize);
            ImGui::Checkbox("No Move", &_no_move);
            ImGui::Checkbox("No Scrollbar", &_no_scrollbar);
            ImGui::Checkbox("No Collapse", &_no_collapse);
            ImGui::Checkbox("No Menu", &_no_menu);
            ImGui::NewLine();
            ImGui::Checkbox("Show info screen", &_display_info_screen);
            ImGui::Checkbox("Show result screen", &_display_result_screen);
        }
        ImGui::End();

        ImGui::Begin("Main Menu", &_isOpen, window_flags);
        {
            lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

            // Sets the window to be at the bottom of the screen (1/3rd of the height)
            ImVec2 windowSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) / 4.f };
            ImVec2 windowPos = { 0, static_cast<float>(window->getHeight() - windowSize.y) };
            ImGui::SetWindowSize(windowSize);
            ImGui::SetWindowPos(windowPos);

            ImVec2 centerButtonPos;

            // Start tests button
            {
                ImVec2 buttonSize = GUI::centerButton(windowSize, { .5f, 0.f });
                if (ImGui::Button(" RUN \nTESTS", buttonSize)) {
                    if (_display_sending_screen == false) {
                        LUG_LOG.debug("Start button pressed");
                        std::shared_ptr<AState> benchmarkingState;
                        benchmarkingState = std::make_shared<BenchmarkingState>(_application);
                        _application.popState();
                        _application.pushState(benchmarkingState);
                    } else {
                        LUG_LOG.debug("Wait for previous logs to be sent");
                    }
                }
            }

            // Config button
            {
                // Centers the button and keeps it square at all times
                ImVec2 buttonSize = GUI::centerButton(windowSize, { 1.5f, 0.f });
                if (ImGui::Button(u8" " ICON_FA_COGS "\nINFO", buttonSize)) {
                    LUG_LOG.debug("Config button pressed");
                    _display_info_screen = true;
                }
            }

            // Result button
            {
                // Centers the button and keeps it square at all times
                ImVec2 buttonSize = GUI::centerButton(windowSize, { -.5f, 0.f });
                if (ImGui::Button("RESULTS", buttonSize)) {
                    LUG_LOG.debug("Results button pressed");
                    _display_result_screen = true;
                }
            }
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
    ImGui::ShowTestWindow();
    return true;
}
