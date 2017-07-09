#include "BenchmarkingState.hpp"

#include <lug/Graphics/Light/Directional.hpp>
#include <lug/Graphics/Scene/ModelInstance.hpp>

// TODO: Remove this when the ResourceManager is done
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

#include <lug/System/Time.hpp>

#include "MenuState.hpp"

BenchmarkingState::BenchmarkingState(Application &application) : AState(application) {
    LUG_LOG.info("BenchmarkingState constructor");
}

BenchmarkingState::~BenchmarkingState() {
}

bool BenchmarkingState::onPush() {
    LUG_LOG.info("BenchmarkingState onPush");

    // Load the model
    {
        // Low Poly by Olexandr Zymohliad
        // is licensed under CC Attribution
        // https://skfb.ly/IXwu
        _model = _application.getGraphics().createModel("model", "models/LowPoly/Low poly.obj");

        if (!_model) {
            throw;
        }
    }
        if (!_application.getCamera()) {
            LUG_LOG.error("CAMERA IS NULLLLL");
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

        if (!_application.getCamera()) {
            LUG_LOG.error("CAMERA IS NULL");
        }
        renderViews[0]->attachCamera(std::move(_application.getCamera()));
    }

    // Add model to scene
    {
        std::unique_ptr<lug::Graphics::Scene::ModelInstance> modelInstance = _scene->createModelInstance("model instance", _model.get());
        _scene->getRoot()->createSceneNode("model instance node", std::move(modelInstance));
    }




    // Add model to scene
    {
        std::unique_ptr<lug::Graphics::Scene::ModelInstance> modelInstance = _scene->createModelInstance("model instance", _model.get());
        _scene->getRoot()->createSceneNode("model instance node", std::move(modelInstance));
    }
	return true;
}

bool BenchmarkingState::onPop() {
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

//  _application.addResult(_fps);
  LUG_LOG.info("Result = {}", _fps);
  
   return true;
}

void BenchmarkingState::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
       _application.popState();
    }
}

bool BenchmarkingState::onFrame(const lug::System::Time& elapsedTime) {

    elapsed += elapsedTime.getSeconds<float>();
    frames++;

    if (elapsed >= 1.0f) {
        _fps = (_fps + (frames / elapsed)) / 2;
        frames = 0;
        elapsed = 0;
    }

    _rotation += (0.05f * elapsedTime.getMilliseconds<float>());

    if (_rotation > 360.0f) {
        _rotation -= 360.0f;
    }

    auto& renderViews = _application.getGraphics().getRenderer()->getWindow()->getRenderViews();

    for (int i = 0; i < 1; ++i) {
        float x = 30.0f * cos(lug::Math::Geometry::radians((i % 2) ? _rotation : -_rotation));
       float y = 30.0f * sin(lug::Math::Geometry::radians((i % 2) ? _rotation : -_rotation));

        renderViews[i]->getCamera()->setPosition({x, 5.0f, y}, lug::Graphics::Node::TransformSpace::World);
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

    ImGui::Begin("Main Menu", isOpen, window_flags);
    {
        lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

        // Sets the window to be at the bottom of the screen (1/3rd of the height)
        ImVec2 windowSize{static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) / 3.f};
        ImVec2 windowPos = {0, static_cast<float>(window->getHeight() - windowSize.y)};
        ImGui::SetWindowSize(windowSize);
        ImGui::SetWindowPos(windowPos);

        // Centers the button and keeps it square at all times
        ImVec2 buttonSize;
        if (windowSize.x > windowSize.y) { buttonSize = {(windowSize.y / 2.f), (windowSize.y / 2.f)}; }
        else { buttonSize = {(windowSize.x / 2.f), (windowSize.x / 2.f)}; }
        ImVec2 buttonPos{(windowSize.x / 2.f) - (buttonSize.x / 2.f), (windowSize.y / 2.f) - (buttonSize.y / 2.f)};
        ImGui::SetCursorPos(buttonPos);
        if (ImGui::Button("STOP", buttonSize)) {
            std::shared_ptr<AState> menuState;
			menuState = std::make_shared<MenuState>(_application);
            _application.popState();
            _application.pushState(menuState);
        }
    }
    ImGui::End();

    return true;
}
