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
    _models = {
        // { name, path, modelNodeName }
        { "Helmet", "models/DamagedHelmet/DamagedHelmet.gltf", "node_damagedHelmet_-8074" },
        { "Helmet2", "models/DamagedHelmet/DamagedHelmet.gltf", "node_damagedHelmet_-8074" }
    };
}

ModelsState::~ModelsState() {
}

bool ModelsState::onPush() {
    _application.setCurrentState(State::MODELS);

    if (!_models.size()) {
        return true;
    }

    return loadModel(_models.front());
}

bool ModelsState::onPop() {
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
    _scene = nullptr;
    return true;
}

void ModelsState::onEvent(const lug::Window::Event& event) {
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    auto window = renderer->getWindow();
    auto& renderViews = window->getRenderViews();
    float mainMenuHeight = GUI::Utilities::getMainMenuHeight(window->getHeight());
    float modelMenuWidth = getModelMenuWidth(window->getWidth());
    float footerHeight = GUI::Utilities::getFooterHeight(window->getHeight());

    LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

    renderViews[0]->getInfo().viewport.extent.width = 1.0f - modelMenuWidth / window->getWidth();
    renderViews[0]->getInfo().viewport.extent.height = 1.0f - (mainMenuHeight + footerHeight) / window->getHeight();
    renderViews[0]->getInfo().viewport.offset.x = modelMenuWidth / window->getWidth();
    renderViews[0]->getInfo().viewport.offset.y = mainMenuHeight / window->getHeight();
    renderViews[0]->update();
    _cameraMover.onEvent(event);
    if (event.type == lug::Window::Event::Type::Close) {
        _application.close();
    }
}

bool ModelsState::onFrame(const lug::System::Time& elapsedTime) {
    bool success = true;

    lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();
    uint16_t windowHeight = window->getHeight();
    uint16_t windowWidth = window->getWidth();
    float windowHeaderOffset = GUI::displayMenu(_application);
    float windowFooterOffset = GUI::displayFooter(_application);

    _cameraMover.onFrame(elapsedTime);

    ImGui::Begin("Model Select Menu", 0, _application._window_flags);
    {
        float modelMenuWidth = getModelMenuWidth(windowWidth);

        ImVec2 modelMenuSize{ modelMenuWidth, windowHeight - (windowHeaderOffset + windowFooterOffset) };

        ImGui::SetWindowSize(modelMenuSize);
        ImGui::SetWindowPos(ImVec2{ 0.f, windowHeaderOffset });
        ImGui::SetWindowFontScale(0.67f);

        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f,0.f });
            {

                float buttonWidth = ImGui::GetWindowWidth();
#if defined(LUG_SYSTEM_ANDROID)
                float buttonHeight = 80.f * 2.75f;
#else
                float buttonHeight = 80.f;
#endif

                for (auto& model: _models) {
                    // Selected style
                    if (_selectedModel == &model) {
                        pushButtonsStyle(
                            ImVec4(.31f, .67f, .98f, 1.00f),
                            ImVec4(.31f, .67f, .98f, 1.00f),
                            ImVec4(.31f, .67f, .98f, 1.00f),
                            ImVec4(0.33f, 0.33f, 0.33f, 1.00f)
                        );
                    }
                    // Unselected style
                    else {
                        pushButtonsStyle(
                            ImVec4(1.f, 1.f, 1.f, 1.00f),
                            ImVec4(.31f, .67f, .98f, 1.00f),
                            ImVec4(.31f, .67f, .98f, 1.00f),
                            ImVec4(0.33f, 0.33f, 0.33f, 1.00f)
                        );
                    }

                    if (ImGui::Button(model.name.c_str(), ImVec2{ buttonWidth, buttonHeight })) {
                        if (!loadModel(model)) {
                            // Don't return false directly or it will make ImGui crash
                            // (Because we need to pop styles and call ImGui::End)
                            success = false;
                        }
                    }

                    ImGui::PopStyleColor(4);
                }
            }
            ImGui::PopStyleVar();
        }
    }
    ImGui::End();

    return success;
}

bool ModelsState::loadModel(const ModelInfos& model) {
    // Load scene
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Resource> sceneResource = renderer->getResourceManager()->loadFile(model.path);
    if (!sceneResource) {
        return false;
    }

    _scene = lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene>::cast(sceneResource);

    auto modelNode = _scene->getSceneNode(model.modelNodeName);
    if (!modelNode) {
        LUG_LOG.error("ModelsState::loadModel Can't get model node {}", model.modelNodeName);
        return false;
    }

    // Attach directional light to the root node
    {
        lug::Graphics::Builder::Light lightBuilder(*renderer);

        lightBuilder.setType(lug::Graphics::Render::Light::Type::Directional);
        lightBuilder.setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
        lightBuilder.setDirection({ 0.0f, 4.0f, -5.0f });

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
        if (!light) {
            LUG_LOG.error("ModelsState::loadModel Can't create directional light");
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
            LUG_LOG.error("ModelsState::loadModel Can't create directional light");
            return false;
        }

        _scene->getRoot().attachLight(light);
    }

    // Attach camera
    {
        lug::Graphics::Scene::Node* node = _scene->createSceneNode("camera");
        _scene->getRoot().attachChild(*node);

        node->attachCamera(_application.getCamera());

        // Set initial position of the camera
        node->setPosition({ 0.0f, 4.0f, 3.0f }, lug::Graphics::Node::TransformSpace::World);
        // Look at once
        node->getCamera()->lookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, lug::Graphics::Node::TransformSpace::World);

        // Attach the camera node to the mover
        {
            _cameraMover.setCameraNode(*node);
            _cameraMover.setModelNode(*modelNode);
            _cameraMover.setEventSource(*renderer->getWindow());
        }

        // Attach camera to RenderView
        {
            auto& renderViews = renderer->getWindow()->getRenderViews();

            LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

            renderViews[0]->attachCamera(_application.getCamera());
        }
    }

    _selectedModel = &model;
    return true;
}

void ModelsState::pushButtonsStyle(
    const ImVec4& color,
    const ImVec4& hoveredColor,
    const ImVec4& activeColor,
    const ImVec4& textColor
) const {
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hoveredColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, activeColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
}

float ModelsState::getModelMenuWidth(float windowWidth) {
#if defined(LUG_SYSTEM_ANDROID)
        return GUI::Utilities::getPercentage(windowWidth, 0.125f, 165.f * 2.75f);
#else
        return GUI::Utilities::getPercentage(windowWidth, 0.125f, 165.f);
#endif
}
