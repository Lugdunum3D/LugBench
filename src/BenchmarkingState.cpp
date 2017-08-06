#include "BenchmarkingState.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/Graphics/Graphics.hpp>
#include <lug/System/Time.hpp>

#include <imgui.h>

#include "MenuState.hpp"

BenchmarkingState::BenchmarkingState(Application &application) : AState(application) {
    LUG_LOG.info("BenchmarkingState constructor");
}

BenchmarkingState::~BenchmarkingState() {}

bool BenchmarkingState::onPush() {
    LUG_LOG.info("BenchmarkingState onPush");

    // Load scene
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Resource> sceneResource = renderer->getResourceManager()->loadFile("models/DamagedHelmet/DamagedHelmet.gltf");
    if (!sceneResource) {
        return false;
    }

    // Create the scene
    _scene = lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene>::cast(sceneResource);


    // Attach directional light to the root node
    {
        lug::Graphics::Builder::Light lightBuilder(*renderer);

        lightBuilder.setType(lug::Graphics::Render::Light::Type::Directional);
        lightBuilder.setColor({1.0f, 1.0f, 1.0f, 1.0f});
        lightBuilder.setDirection({0.0f, -4.0f, 5.0f});

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
        if (!light) {
            LUG_LOG.error("Application::init Can't create directional light");
            return false;
        }

        _scene->getRoot().attachLight(light);
    }

    // Add camera to scene
    {
        lug::Graphics::Scene::Node* node = _scene->createSceneNode("camera");
        _scene->getRoot().attachChild(*node);

        node->attachCamera(_application.getCamera());

        // Set initial position of the camera
        node->setPosition({2.0f, 0.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
        // Look at once
        node->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
    }

    // Attach cameras to RenderView
    {
        auto& renderViews = _application.getGraphics().getRenderer()->getWindow()->getRenderViews();

        LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

        if (!_application.getCamera()) {
            LUG_LOG.error("CAMERA IS NULL");
        }
        renderViews[0]->attachCamera(_application.getCamera());
    }
    return true;
}

bool BenchmarkingState::onPop() {
    _application.sendDevice(_frames);

    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();

    // auto& renderViews = _application.getGraphics().getRenderer()->getWindow()->getRenderViews();
    // LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

    _scene = nullptr;

   return true;
}

void BenchmarkingState::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
       _application.close();
    }
}

bool BenchmarkingState::onFrame(const lug::System::Time& elapsedTime) {

    _elapsed += elapsedTime.getSeconds<float>();
    _frames++;

    if (_elapsed >= 3.0f) {
        std::shared_ptr<AState> menuState;
        menuState = std::make_shared<MenuState>(_application);
        _application.popState();
        _application.pushState(menuState);
        return true;
    }

    _rotation += 0.01f * elapsedTime.getMilliseconds<float>();

    if (_rotation > 360.0f) {
        _rotation -= 360.0f;
    }

    auto& renderViews = _application.getGraphics().getRenderer()->getWindow()->getRenderViews();

    for (int i = 0; i < 1; ++i) {
        float x = 2.0f * cos(lug::Math::Geometry::radians((i % 2) ? _rotation : -_rotation));
        float y = 2.0f * sin(lug::Math::Geometry::radians((i % 2) ? _rotation : -_rotation));

        _scene->getSceneNode("camera")->setPosition({x, 2.0f, y}, lug::Graphics::Node::TransformSpace::World);
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

    return true;
}
