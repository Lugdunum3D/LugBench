#include "ModelsState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Builder/SkyBox.hpp>
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
    _skyBoxes["default"] = {
        "textures/skybox/right.jpg",
        "textures/skybox/left.jpg",
        "textures/skybox/top.jpg",
        "textures/skybox/bottom.jpg",
        "textures/skybox/back.jpg",
        "textures/skybox/front.jpg"
    };
    _models = {
        // { name, path, modelNodeName, skyboxName, rotation }
        { "Helmet", "models/DamagedHelmet/DamagedHelmet.gltf", "node_damagedHelmet_-8074", "default", {0.0f, -160.0f, 0.0f} },
        { "Helmet2", "models/DamagedHelmet/DamagedHelmet.gltf", "node_damagedHelmet_-8074", "default", {0.0f, -160.0f, 0.0f} }
    };
}

ModelsState::~ModelsState() {
}

bool ModelsState::onPush() {
    _application.setCurrentState(State::MODELS);
    handleResize();

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
    _cameraMover.onEvent(event);
    if (event.type == lug::Window::Event::Type::Close) {
        _application.close();
    }
    else if (event.type == lug::Window::Event::Type::Resize) {
        handleResize();
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

    ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0f, 0.0f, 0.0f, 0.0f});
    {
        ImGui::Begin("Render settings", 0, _application._window_flags);
        {
            ImVec2 modelSettingsWindowSize = ImVec2{ static_cast<float>(windowWidth), windowHeight - (windowHeaderOffset + windowFooterOffset) };
            float settingsMarginRight = 10.0f;
            float buttonsSpacing = 10.0f;
            ImVec2 buttonSize{ 30.0f, 30.0f };
            ImVec2 windowRightAlign {
                windowWidth - buttonSize.x - settingsMarginRight,
                (windowHeight / 2) - ((buttonSize.y * 2 + buttonsSpacing) / 2)
            };

            // Setup window
            ImGui::SetWindowSize(modelSettingsWindowSize);
            ImGui::SetWindowPos(windowRightAlign);

            // Display settings buttons
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, buttonsSpacing });
                {
                    if (ImGui::Button(ICON_FA_DESKTOP, buttonSize)) {
                        const uint32_t currentDisplayMode = static_cast<uint32_t>(_application.getGraphics().getRenderer()->getDisplayMode());
                        _application.getGraphics().getRenderer()->setDisplayMode(static_cast<::lug::Graphics::Renderer::DisplayMode>(currentDisplayMode == 0 ? 7 : currentDisplayMode - 1));
                    }
                    if (ImGui::Button(ICON_FA_TWITTER, buttonSize)) {
                        _displaySkyBox = !_displaySkyBox;
                        if (!loadModelSkyBox(*_selectedModel)) {
                            success = false;
                        }
                    }
                }
                ImGui::PopStyleVar(2);
            }
            ImGui::PopFont();
        }
        ImGui::End();
    }
    ImGui::PopStyleColor();

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
                            GUI::V4_SKYBLUE,
                            GUI::V4_SKYBLUE,
                            GUI::V4_SKYBLUE,
                            GUI::V4_DARKGRAY
                        );
                    }
                    // Unselected style
                    else {
                        pushButtonsStyle(
                            GUI::V4_WHITE,
                            GUI::V4_SKYBLUE,
                            GUI::V4_SKYBLUE,
                            GUI::V4_DARKGRAY
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

    if (model.rotation.x()) {
        modelNode->rotate(model.rotation.x(), {1.0f, 0.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
    }
    if (model.rotation.y()) {
        modelNode->rotate(model.rotation.y(), {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
    }
    if (model.rotation.z()) {
        modelNode->rotate(model.rotation.z(), {0.0f, 0.0f, 1.0f}, lug::Graphics::Node::TransformSpace::World);
    }

    // Attach directional light to the root node
    {
        lug::Graphics::Builder::Light lightBuilder(*renderer);

        lightBuilder.setType(lug::Graphics::Render::Light::Type::Directional);
        lightBuilder.setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
        lightBuilder.setDirection({ 0.0f, 0.0f, -3.0f });

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
        lightBuilder.setDirection({ 0.0f, 0.0f, -3.0f });

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
        if (!light) {
            LUG_LOG.error("ModelsState::loadModel Can't create directional light");
            return false;
        }

        _scene->getRoot().attachLight(light);
    }

    // Attach skyBox
    if (!loadModelSkyBox(model)) {
        return false;
    }

    // Attach camera
    {
        lug::Graphics::Scene::Node* node = _scene->createSceneNode("camera");
        _scene->getRoot().attachChild(*node);

        node->attachCamera(_application.getCamera());

        // Set initial position of the camera
        node->setPosition({ 0.0f, 0.0f, 3.0f }, lug::Graphics::Node::TransformSpace::World);
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

bool ModelsState::loadModelSkyBox(const ModelInfos& model) {
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();

    if (model.skyboxName.size()) {
        auto skyBox = _skyBoxes.find(model.skyboxName);
        if (skyBox == _skyBoxes.end()) {
            LUG_LOG.error("ModelsState::loadModel Can't load skybox {}", model.skyboxName);
            return false;
        }
        else if (!skyBox->second.resource) {
            lug::Graphics::Builder::SkyBox skyBoxBuilder(*renderer);

            skyBoxBuilder.setFaceFilename(lug::Graphics::Builder::SkyBox::Face::PositiveX, skyBox->second.positiveXFile);
            skyBoxBuilder.setFaceFilename(lug::Graphics::Builder::SkyBox::Face::NegativeX, skyBox->second.negativeXFile);
            skyBoxBuilder.setFaceFilename(lug::Graphics::Builder::SkyBox::Face::PositiveY, skyBox->second.positiveYFile);
            skyBoxBuilder.setFaceFilename(lug::Graphics::Builder::SkyBox::Face::NegativeY, skyBox->second.negativeYFile);
            skyBoxBuilder.setFaceFilename(lug::Graphics::Builder::SkyBox::Face::PositiveZ, skyBox->second.positiveZFile);
            skyBoxBuilder.setFaceFilename(lug::Graphics::Builder::SkyBox::Face::NegativeZ, skyBox->second.negativeZFile);

            skyBox->second.resource = skyBoxBuilder.build();
            if (!skyBox->second.resource) {
                LUG_LOG.error("ModelsState::loadModel Can't create skyBox {}", model.skyboxName);
                return false;
            }
        }

        if (_displaySkyBox) {
            _scene->setSkyBox(skyBox->second.resource);
        }
        else {
            _scene->setSkyBox(nullptr);
        }
    }

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

void ModelsState::handleResize() {
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    auto window = renderer->getWindow();
    auto& renderViews = window->getRenderViews();
    float mainMenuHeight = GUI::Utilities::getMainMenuHeight(window->getHeight());
    float modelMenuWidth = getModelMenuWidth(window->getWidth());

    LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

    renderViews[0]->getInfo().viewport.extent.width = 1.0f - modelMenuWidth / window->getWidth();
    renderViews[0]->getInfo().viewport.extent.height = 1.0f - mainMenuHeight / window->getHeight();
    renderViews[0]->getInfo().viewport.offset.x = modelMenuWidth / window->getWidth();
    renderViews[0]->getInfo().viewport.offset.y = mainMenuHeight / window->getHeight();
    renderViews[0]->update();
}
