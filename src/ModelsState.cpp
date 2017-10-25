#include "ModelsState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

#include "BenchmarkingState.hpp"

#include "ContactState.hpp"
#include "ResultsState.hpp"
#include "InfoState.hpp"
#include "BenchmarksState.hpp"


#include <IconsFontAwesome.h>

ModelsState::ModelsState(LugBench::Application &application) : AState(application) {
    GUI::setDefaultStyle();
}

ModelsState::~ModelsState() {
}

bool ModelsState::onPush() {
    // Load scene
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Resource> sceneResource = renderer->getResourceManager()->loadFile("models/Duck/Duck.gltf");
    //    lug::Graphics::Resource::SharedPtr<lug::Graphics::Resource> sceneResource = renderer->getResourceManager()->loadFile("models/DamagedHelmet/DamagedHelmet.gltf");
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

        lightBuilder.setType(lug::Graphics::Render::Light::Type::Directional);
        lightBuilder.setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
        lightBuilder.setDirection({ 0.0f, -4.0f, 5.0f });

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
        if (!light) {
            LUG_LOG.error("Application::init Can't create directional light");
            return false;
        }

        //_scene->getSceneNode("duck")->attachLight(light);
        _scene->getRoot().attachLight(light);
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

        // Attach the camera node to the mover
        {
            _cameraMover.setTargetNode(*node);
            _cameraMover.setEventSource(*renderer->getWindow());
        }

        // Attach camera to RenderView
        {
            auto& renderViews = renderer->getWindow()->getRenderViews();

            LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

            renderViews[0]->attachCamera(_application.getCamera());
        }
    }

    return true;
}

bool ModelsState::onPop() {
    LUG_LOG.info("ModelsState onPop");
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
    _scene = nullptr;
    return true;
}

void ModelsState::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
        _application.close();
    }
}

bool ModelsState::onFrame(const lug::System::Time& elapsedTime) {
    _application.setCurrentState(State::MODELS);

    _cameraMover.onFrame(elapsedTime);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;


    lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

    float mainMenuHeight = static_cast<float>(window->getHeight()) / 18.f;

#if defined(LUG_SYSTEM_ANDROID)
    mainMenuHeight = (mainMenuHeight < 60.f * 2.f) ? 60.f * 2.f : mainMenuHeight;
#else
    mainMenuHeight = (mainMenuHeight < 60.f) ? 60.f : mainMenuHeight;
#endif

    GUI::displayMenu(_application, window_flags);

    window_flags |= ImGuiWindowFlags_ShowBorders;

    ImGui::Begin("Model Select Menu", &_isOpen, window_flags);
    {
        float modelMenuWidth = static_cast<float>(window->getWidth()) / 8.f;
#if defined(LUG_SYSTEM_ANDROID)
        modelMenuWidth = (modelMenuWidth < 165.f * 2.75f) ? 165.f * 2.75f : modelMenuWidth;
#else
        modelMenuWidth = (modelMenuWidth < 165.f) ? 165.f : modelMenuWidth;
#endif
        ImVec2 modelMenuSize{ modelMenuWidth, static_cast<float>(window->getHeight()) - mainMenuHeight };

        ImGui::SetWindowSize(modelMenuSize);
        ImGui::SetWindowPos(ImVec2{ 0.f, mainMenuHeight });
        ImGui::SetWindowFontScale(0.67f);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 1.f, 1.f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.31f, .67f, .98f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(.31f, .67f, .98f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f,0.f });
            {

                float buttonWidth = ImGui::GetWindowWidth();
                float buttonHeight;
#if defined(LUG_SYSTEM_ANDROID)
                buttonHeight = 80.f * 2.75f;
#else
                buttonHeight = 80.f;
#endif

                ImGui::Button("Duck", ImVec2{ buttonWidth, buttonHeight });
                ImGui::Button("Helmet", ImVec2{ buttonWidth, buttonHeight });
                ImGui::Button("Monkey", ImVec2{ buttonWidth, buttonHeight });
                ImGui::Button("Repunzel", ImVec2{ buttonWidth, buttonHeight });
                ImGui::Button("Tower", ImVec2{ buttonWidth, buttonHeight });
            }
            ImGui::PopStyleVar();
        }
        ImGui::PopStyleColor(4);
    }
    ImGui::End();
    return true;
}
