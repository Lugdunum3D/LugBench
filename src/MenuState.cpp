#include "MenuState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

#include <lug/System/Time.hpp>

#include <lug/System/Logger/Logger.hpp>
#include "BenchmarkingState.hpp"
#include <json/json.hpp>
#include "LugNetwork.hpp"

MenuState::MenuState(Application &application) : AState(application) {
    _no_menu_bar = false;
    _no_titlebar = true;
    _no_border = true;
    _no_resize = true;
    _no_move = true;
    _no_scrollbar = false;
    _no_collapse = true;
    _no_menu = true;

//    ImGuiStyle& style = ImGui::GetStyle();
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
    if (_physicalDeviceInfo == NULL) {
        return false;
    }
    return true;
}

bool MenuState::onPop() {
    LUG_LOG.info("MenuState onPop");

    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();

    // auto& renderViews = _application.getGraphics().getRenderer()->getWindow()->getRenderViews();
    // LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");
    // _application.setCamera(renderViews[0]->detachCamera());

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

    // _scene->getSceneNode("duck")->setRotation(lug::Math::Geometry::radians(_rotation), {0, 1, 0});

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
    else {
        if (_display_sending_screen == true) {
            _sending_log_timer = 2.f;
            _sending_end_log_timer = 3.f;
            _display_sending_screen = false;
        }
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
//            ImGui::Text("%s Search", u8"\uf002");
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
            ImVec2 windowSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) / 3.f };
            ImVec2 windowPos = { 0, static_cast<float>(window->getHeight() - windowSize.y) };
            ImGui::SetWindowSize(windowSize);
            ImGui::SetWindowPos(windowPos);

            ImVec2 centerButtonPos;

            // Start tests button
            {
                // Centers the button and keeps it square at all times
                ImVec2 buttonSize;
                if (windowSize.x > windowSize.y) { buttonSize = { (windowSize.y / 2.f), (windowSize.y / 2.f) }; }
                else { buttonSize = { (windowSize.x / 2.f), (windowSize.x / 2.f) }; }
                centerButtonPos = { (windowSize.x / 2.f) - (buttonSize.x / 2.f), (windowSize.y / 2.f) - (buttonSize.y / 2.f) };
                ImGui::SetCursorPos(centerButtonPos);
                if (ImGui::Button("START\nTESTS", buttonSize)) {
                    if (_display_sending_screen == false) {
                        LUG_LOG.debug("Start button pressed");
                        std::shared_ptr<AState> benchmarkingState;
                        benchmarkingState = std::make_shared<BenchmarkingState>(_application);
                        _application.popState();
                        _application.pushState(benchmarkingState);
                    }
                    else {
                        LUG_LOG.debug("Wait for previous logs to be sent");
                    }
                }
            }

            // Config button
            {
                // Centers the button and keeps it square at all times
                ImVec2 buttonSize;
                if (windowSize.x > windowSize.y) { buttonSize = { (windowSize.y / 3.f), (windowSize.y / 3.f) }; }
                else { buttonSize = { (windowSize.x / 3.f), (windowSize.x / 3.f) }; }
                ImVec2 buttonPos{ centerButtonPos.x - (buttonSize.x * 1.5f), centerButtonPos.y + (buttonSize.y / 4.f) };
                ImGui::SetCursorPos(buttonPos);
                if (ImGui::Button("Config\nInfo", buttonSize)) {
                    LUG_LOG.debug("Config button pressed");
                    _display_info_screen = !_display_info_screen;
                }
            }

            // Result button
            {
                // Centers the button and keeps it square at all times
                ImVec2 buttonSize;
                if (windowSize.x > windowSize.y) { buttonSize = { (windowSize.y / 3.f), (windowSize.y / 3.f) }; }
                else { buttonSize = { (windowSize.x / 3.f), (windowSize.x / 3.f) }; }
                ImVec2 buttonPos{ centerButtonPos.x + (buttonSize.x * 2.f), centerButtonPos.y + (buttonSize.y / 4.f) };
                ImGui::SetCursorPos(buttonPos);
                if (ImGui::Button("Results", buttonSize)) {
                    LUG_LOG.debug("Results button pressed");
                    _display_result_screen = !_display_result_screen;
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
        if (_isStarted == false) {
            _isReceiving = true;
            _application.getNetwork().getScores();
            _isStarted = true;
            return true;
        }
        if (_isReceiving) {
            if (_application.getNetwork().getLastRequestStatusCode() > 0) {
                _isReceiving = false;
                LUG_LOG.info("statuscode: {}", _application.getNetwork().getLastRequestStatusCode());
                LUG_LOG.info("body: {}", _application.getNetwork().getLastRequestBody());
                nlohmann::json response = nlohmann::json::parse(_application.getNetwork().getLastRequestBody());
                _devices = response["data"];
            }
            return true;
        }
        
        GUI::displayResultScreen(&_isOpen, window_flags, _application.getGraphics().getRenderer()->getWindow(), _physicalDeviceInfo, &_devices);
        if (_isOpen == false) {
            _display_result_screen = !_display_result_screen;
        }
    }
    ImGui::ShowTestWindow();
    return true;
}

