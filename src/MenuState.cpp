#include "MenuState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Light/Directional.hpp>
#include <lug/Graphics/Scene/ModelInstance.hpp>

// TODO: Remove this when the ResourceManager is done
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>

#include <lug/System/Time.hpp>

#include "BenchmarkingState.hpp"

MenuState::MenuState(Application &application) : AState(application) {
}

MenuState::~MenuState() {
    LUG_LOG.info("MenuState destructor");
}

bool MenuState::onPush() {
    // Load the model
    {
        // Low Poly by Olexandr Zymohliad
        // is licensed under CC Attribution
        // https://skfb.ly/IXwu
        _model = _application.getGraphics().createModel("model", "models/LowPoly/cello.obj");

        if (!_model) {
            throw;
        }
    }

    // Create the scene
    _scene = _application.getGraphics().createScene();

    // Add directional light to scene
    {
        std::unique_ptr<lug::Graphics::Light::Light> light = _scene->createLight("light", lug::Graphics::Light::Light::Type::Directional);

        // Set the diffuse color to white and the position
        light->setDiffuse({1.0f, 1.0f, 1.0f});
        static_cast<lug::Graphics::Light::Directional*>(light.get())->setDirection({0.0f, -4.0f, 5.0f});

        _scene->getRoot()->createSceneNode("light node", std::move(light));
    }

    _application.getCamera()->setScene(_scene.get());

    // Add camera to scene
    {
        std::unique_ptr<lug::Graphics::Scene::MovableCamera> movableCamera = _scene->createMovableCamera("movable camera", _application.getCamera().get());

        std::unique_ptr<lug::Graphics::Scene::Node> movableCameraNode = _scene->createSceneNode("movable camera node");

        movableCameraNode->attachMovableObject(std::move(movableCamera));

        _scene->getRoot()->attachChild(std::move(movableCameraNode));
    }

    // Attach cameras to RenderView
    {
        auto& renderViews = _application.getGraphics().getRenderer()->getWindow()->getRenderViews();

        LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

        renderViews[0]->attachCamera(std::move(_application.getCamera()));
    }

    // Add model to scene
    {
        std::unique_ptr<lug::Graphics::Scene::ModelInstance> modelInstance = _scene->createModelInstance("model instance", _model.get());
        _scene->getRoot()->createSceneNode("model instance node", std::move(modelInstance));
    }

	lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
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

    auto& renderViews = _application.getGraphics().getRenderer()->getWindow()->getRenderViews();
    LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");
    _application.setCamera(renderViews[0]->detachCamera());

    if (!_application.getCamera()) {
        LUG_LOG.error("NOPE");
    } else {
        LUG_LOG.info("OK");
    }

   _scene = nullptr;

   return true;
}

void MenuState::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
       _application.popState();
    }
}

bool MenuState::onFrame(const lug::System::Time& elapsedTime) {
    _rotation += (0.05f * elapsedTime.getMilliseconds<float>());

    if (_rotation > 360.0f) {
        _rotation -= 360.0f;
    }

    auto& renderViews = _application.getGraphics().getRenderer()->getWindow()->getRenderViews();

    for (int i = 0; i < 1; ++i) {
        float x = 30.0f * cos(lug::Math::Geometry::radians((i % 2) ? _rotation : -_rotation));

        renderViews[i]->getCamera()->setPosition({x, 5.0f, 10.0f}, lug::Graphics::Node::TransformSpace::World);
        renderViews[i]->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
    }

    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
    if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
    if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
    if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;

    if (display_info_screen == false && display_result_screen == false) {
        if (!no_menu_bar) {
            ImGui::BeginMainMenuBar();
            {
                if (ImGui::BeginMenu("Menu1")) {
                    if (ImGui::MenuItem("Item 1-1")) {
                        // BLA
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Menu2")) {
                    if (ImGui::MenuItem("Item 2-1")) {
                        // BLA
                    }
                    ImGui::EndMenu();
                }
            }
            ImGui::EndMainMenuBar();
        }
    
        ImGui::Begin("Sample Window", isOpen, window_flags);
        {
            ImGui::Checkbox("No Menu Bar", &no_menu_bar);
            ImGui::NewLine();
            ImGui::Checkbox("No Titlebar", &no_titlebar);
            ImGui::Checkbox("No Border", &no_border);
            ImGui::Checkbox("No Resize", &no_resize);
            ImGui::Checkbox("No Move", &no_move);
            ImGui::Checkbox("No Scrollbar", &no_scrollbar);
            ImGui::Checkbox("No Collapse", &no_collapse);
            ImGui::Checkbox("No Menu", &no_menu);
            ImGui::NewLine();
            ImGui::Checkbox("Show info screen", &display_info_screen);
            ImGui::Checkbox("Show result screen", &display_result_screen);
        }
        ImGui::End();

        ImGui::Begin("Main Menu", isOpen, window_flags);
        {
            lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

            // Sets the window to be at the bottom of the screen (1/3rd of the height)
            ImVec2 windowSize{static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) / 3.f};
            ImVec2 windowPos = {0, static_cast<float>(window->getHeight() - windowSize.y)};
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
                    LUG_LOG.debug("Start button pressed");
                    std::shared_ptr<AState> benchmarkingState;
                    benchmarkingState = std::make_shared<BenchmarkingState>(_application);
                    _application.popState();
                    _application.pushState(benchmarkingState);
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
                    display_info_screen = !display_info_screen;
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
                    display_result_screen = !display_result_screen;
                }
            }
        }
        ImGui::End();
    }
    else if (display_info_screen == true) {
        ImGui::Begin("Info Display", isOpen, window_flags);
        {
            lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

            // Sets the window to be at the bottom of the screen (1/3rd of the height)
            ImVec2 windowSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) };
            ImVec2 windowPos = {0, 0};
            ImGui::SetWindowSize(windowSize);
            ImGui::SetWindowPos(windowPos);

			if (ImGui::CollapsingHeader("Device", ImGuiTreeNodeFlags_NoAutoOpenOnLog)) {
				GUI::displayConfigInfoUI("Name", _physicalDeviceInfo->properties.deviceName);
				GUI::displayConfigInfoUI("Device ID", (char *)std::to_string(_physicalDeviceInfo->properties.deviceID).c_str());
				std::string driverVersion = std::to_string(lug::Core::Version::fromInt(_physicalDeviceInfo->properties.driverVersion).major) + "." +
					std::to_string(lug::Core::Version::fromInt(_physicalDeviceInfo->properties.driverVersion).minor) + "."
					+ std::to_string(lug::Core::Version::fromInt(_physicalDeviceInfo->properties.driverVersion).patch);
				GUI::displayConfigInfoUI("Driver Version", (char *)driverVersion.c_str());
				GUI::displayConfigInfoUI("Vendor ID", (char *)std::to_string(_physicalDeviceInfo->properties.vendorID).c_str());
				GUI::displayConfigInfoUI("API Version", (char *)std::to_string(_physicalDeviceInfo->properties.apiVersion).c_str());
				GUI::displayConfigInfoUI("Device Type", (char *)lug::Graphics::Vulkan::API::RTTI::toStr(_physicalDeviceInfo->properties.deviceType));
				GUI::displayConfigInfoUI("API Version", (char *)std::to_string(_physicalDeviceInfo->properties.apiVersion).c_str());
			}

          

			
            ImGui::Separator();
            ImGui::NewLine();
            if (ImGui::Button("< RETURN")) {
                display_info_screen = !display_info_screen;
            }
        }
        ImGui::End();
    }
    else if (display_result_screen == true) {
        ImGui::Begin("Result Display", isOpen, window_flags);
        {
            lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

            // Sets the window to be at the bottom of the screen (1/3rd of the height)
            ImVec2 windowSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) };
            ImVec2 windowPos = { 0, 0 };
            ImGui::SetWindowSize(windowSize);
            ImGui::SetWindowPos(windowPos);

            ImGui::SetCursorPos({50, 10});
            ImGui::Button("Test", {50, 20});
            ImGui::SetCursorPos({250, 10});
            ImGui::Button("Score", {50, 20});
            ImGui::NewLine();

            for (int i = 0; i < 3; ++i) {
                ImGui::SetCursorPos({ 60, 20 * static_cast<float>(i + 2)});
                ImGui::Text("Test");
                ImGui::SetCursorPos({ 260, 20 * static_cast<float>(i + 2)});
                ImGui::Text("1337 f/s");
            }

            ImGui::Separator();
            ImGui::NewLine();
            if (ImGui::Button("< RETURN")) {
                display_result_screen = !display_result_screen;
            }
        }
        ImGui::End();
    }
	return true;
}

