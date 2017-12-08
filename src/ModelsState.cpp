#include "ModelsState.hpp"
#include "GUI.hpp"

#include <thread>

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

std::vector<ModelsState::ModelInfos> ModelsState::_models = {
    // { name, path, modelNodeName, skyboxName, rotation }
    { "Helmet", "models/DamagedHelmet/DamagedHelmet.gltf", "node_damagedHelmet_-8074", "default", {-90.0f, 0.0f} },
    { "Helmet2", "models/DamagedHelmet/DamagedHelmet.gltf", "node_damagedHelmet_-8074", "default", {-90.0f, 0.0f} }
};
std::unordered_map<std::string, ModelsState::SkyBoxInfo> ModelsState::_skyBoxes = {
    {
        "default", {
            "textures/Road_to_MonumentValley/Background.jpg",
            "textures/Road_to_MonumentValley/Environnement.hdr"
        }
    }
};

ModelsState::ModelsState(LugBench::Application &application) : AState(application) {
    GUI::setDefaultStyle();
}

ModelsState::~ModelsState() {}

bool ModelsState::onPush() {
    _application.setCurrentState(State::MODELS);
    handleResize();

    if (_models.size() && !loadModel(_models.front())) {
        return false;
    }

    lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();
    uint16_t windowWidth = window->getWidth();

    return _loadingAnimation.init(
        /* application */ _application,
        /* loadingDotImage */ "textures/loading_dot.png",
        /* size */ {16.0f, 16.0f},
        /* offset */ {ModelsState::getModelMenuWidth(windowWidth) / 2.0f, 0.0f}
    );
}

bool ModelsState::onPop() {
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
    return true;
}

void ModelsState::onEvent(const lug::Window::Event& event) {
    if (!_loadingModel) {
        _cameraMover.onEvent(event);
    }

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
    float windowHeaderOffset = 0;
    float windowFooterOffset = 0;
    float modelMenuWidth = getModelMenuWidth(windowWidth);

    if (!_displayFullscreen) {
        windowHeaderOffset = GUI::displayMenu(_application);
        windowFooterOffset = GUI::displayFooter(_application);
    }

    if (_selectedModel->sceneResource && _loadingModel) {
        _loadingModel = false;
        _loadingAnimation.display(false);
        attachCameraToMover();
    }

    if (!_loadingModel) {
        _cameraMover.onFrame(elapsedTime);
    }

    ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
    {
        ImGui::Begin("Render settings", 0, _application._window_flags);
        {
#if defined(LUG_SYSTEM_ANDROID)
            float settingsMarginBottom = 30.0f;
            float buttonsSpacing = 60.0f;
            ImVec2 buttonSize{ 60.0f * 2.f, 60.0f  * 2.f };
#else
            float settingsMarginBottom = 10.0f;
            float buttonsSpacing = 10.0f;
            ImVec2 buttonSize{ 60.0f, 60.0f };
#endif
            ImVec2 modelSettingsWindowSize = ImVec2{ buttonSize.x * 3.0f + buttonsSpacing * 2.0f, buttonSize.y};
            ImVec2 buttonBottomAlign{
                modelMenuWidth + (windowWidth - modelMenuWidth) / 2.0f - modelSettingsWindowSize.x / 2.0f,
                static_cast<float>(windowHeight) - buttonSize.y - settingsMarginBottom
            };

            // Setup window
            ImGui::SetWindowSize(modelSettingsWindowSize);
            ImGui::SetWindowPos(buttonBottomAlign);


            // Display settings buttons
            ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ buttonsSpacing, 0.f });
                {
                    if (ImGui::Button(ICON_FA_ARROWS_ALT, buttonSize)) {
                        _displayFullscreen = !_displayFullscreen;
                        handleResize();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(ICON_FA_MAGIC, buttonSize)) {
                        const uint32_t currentDisplayMode = static_cast<uint32_t>(_application.getGraphics().getRenderer()->getDisplayMode());
                        _application.getGraphics().getRenderer()->setDisplayMode(static_cast<::lug::Graphics::Renderer::DisplayMode>(currentDisplayMode == 0 ? 7 : currentDisplayMode - 1));
                    }
                    ImGui::SameLine();
                    if (ImGui::Button(ICON_FA_MAP, buttonSize)) {
                        _displaySkyBox = !_displaySkyBox;
                        if (!loadModelSkyBox(*_selectedModel, _selectedModel->sceneResource, _application.getGraphics().getRenderer(), _displaySkyBox)) {
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

    if (!_displayFullscreen) {
        ImGui::Begin("Model Select Menu", 0, _application._window_flags);
        {
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

                    for (auto& model : _models) {
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
        _loadingAnimation.update(elapsedTime);
        ImGui::End();
    }

    return success;
}

void ModelsState::attachCameraToMover() {
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene> scene = lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene>::cast(_selectedModel->sceneResource);
    auto camera = _application.getCamera();
    lug::Graphics::Scene::Node* node = scene->getRoot().getNode("camera");

    node->attachCamera(camera);
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    _cameraMover.setTargetNode(*node);
    _cameraMover.setEventSource(*renderer->getWindow());
}

bool ModelsState::loadModel(ModelInfos& model) {
    if (_loadingModel) {
        return true;
    }

    _selectedModel = &model;

    // The model has already been loaded
    if (model.sceneResource) {
        attachCameraToMover();
        return true;
    }

    // Load scene
    _loadingModel = true;
    _loadingAnimation.display(true);

    // Remove camera before loading model
    {
        lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
        auto& renderViews = renderer->getWindow()->getRenderViews();

        LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

        renderViews[0]->attachCamera(nullptr);
    }

    std::thread loadModelThread([&]{
        lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
        bool displaySkybox = _displaySkyBox;
        auto camera = _application.getCamera();
        auto sceneResource = renderer->getResourceManager()->loadFile(model.path);
        if (!sceneResource) {
            return false;
        }

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene> scene = lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene>::cast(sceneResource);

        auto modelNode = scene->getSceneNode(model.modelNodeName);
        if (!modelNode) {
            LUG_LOG.error("ModelsState::loadModel Can't get model node {}", model.modelNodeName);
            return false;
        }

        // Attach directional light to the root node
        {
            lug::Graphics::Builder::Light lightBuilder(*renderer);

        lightBuilder.setType(lug::Graphics::Render::Light::Type::Directional);
        lightBuilder.setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
        lightBuilder.setDirection({ 0.0f, 0.0f, 3.0f });

            lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
            if (!light) {
                LUG_LOG.error("ModelsState::loadModel Can't create directional light");
                return false;
            }

            scene->getRoot().attachLight(light);
        }

        // Attach ambient light to the root node
        {
            lug::Graphics::Builder::Light lightBuilder(*renderer);

            lightBuilder.setType(lug::Graphics::Render::Light::Type::Directional);
            lightBuilder.setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
            lightBuilder.setDirection({ 0.0f, 0.0f, 3.0f });
            lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Light> light = lightBuilder.build();
            if (!light) {
                LUG_LOG.error("ModelsState::loadModel Can't create directional light");
                return false;
            }

            scene->getRoot().attachLight(light);
        }

        // Attach camera
        {
            lug::Graphics::Scene::Node* node = scene->createSceneNode("camera");
            scene->getRoot().attachChild(*node);

            // Attach skyBox
            if (!loadModelSkyBox(model, sceneResource, renderer, displaySkybox)) {
                return false;
            }

            node->attachCamera(camera);

            // Set initial position of the camera
            node->setPosition({ 0.0f, 0.0f, 3.0f }, lug::Graphics::Node::TransformSpace::World);
            // Look at once
            node->getCamera()->lookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, lug::Graphics::Node::TransformSpace::World);

            // Attach camera to RenderView
            {
                auto& renderViews = renderer->getWindow()->getRenderViews();

                LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

                renderViews[0]->attachCamera(camera);
            }

            model.sceneResource = sceneResource;
        }


        return true;
    });

    loadModelThread.detach();

    return true;
}

void applyIBL(const lug::Graphics::Scene::Node* node, lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> irradianceMap, lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> prefilteredMap) {
    const lug::Graphics::Scene::Node::MeshInstance* meshInstance = node->getMeshInstance();
    if (meshInstance) {
        for (auto& material: meshInstance->materials) {
            material->setIrradianceMap(irradianceMap);
            material->setPrefilteredMap(prefilteredMap);
        }
    }

    for (const auto& child : node->getChildren()) {
        applyIBL(static_cast<const lug::Graphics::Scene::Node*>(child), irradianceMap, prefilteredMap);
    }
}

bool ModelsState::loadModelSkyBox(
    const ModelInfos& model,
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Resource> sceneResource,
    lug::Graphics::Renderer* renderer,
    bool displaySkyBox
) {
    if (model.skyboxName.size()) {
        auto skyBox = _skyBoxes.find(model.skyboxName);
        if (skyBox == _skyBoxes.end()) {
            LUG_LOG.error("ModelsState::loadModel Can't load skybox {}", model.skyboxName);
            return false;
        }
        else if (!skyBox->second.resource) {
            lug::Graphics::Builder::SkyBox skyBoxBuilder(*renderer);

            skyBoxBuilder.setMagFilter(lug::Graphics::Render::Texture::Filter::Linear);
            skyBoxBuilder.setMinFilter(lug::Graphics::Render::Texture::Filter::Linear);
            skyBoxBuilder.setBackgroundFilename(skyBox->second.backgroundFile);
            skyBoxBuilder.setEnvironnementFilename(skyBox->second.environmentFile);

            skyBox->second.resource = skyBoxBuilder.build();
            if (!skyBox->second.resource) {
                LUG_LOG.error("ModelsState::loadModel Can't create skyBox {}", model.skyboxName);
                return false;
            }

            skyBox->second.irradianceMap = skyBox->second.resource->createIrradianceMap(*renderer);

            if (!skyBox->second.irradianceMap) {
                LUG_LOG.error("Application::init Can't create irradiance map");
                return false;
            }

            skyBox->second.prefilteredMap = skyBox->second.resource->createPrefilteredMap(*renderer);

            if (!skyBox->second.prefilteredMap) {
                LUG_LOG.error("Application::init Can't create prefiltered map");
                return false;
            }

        }

        lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene> scene = lug::Graphics::Resource::SharedPtr<lug::Graphics::Scene::Scene>::cast(sceneResource);
        applyIBL(&scene->getRoot(), skyBox->second.irradianceMap, skyBox->second.prefilteredMap);
        if (displaySkyBox) {
            scene->setSkyBox(skyBox->second.resource);
        }
        else {
            scene->setSkyBox(nullptr);
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
    if (_displayFullscreen)
    {
        return 0.f;
    }
    else
    {
#if defined(LUG_SYSTEM_ANDROID)
    return GUI::Utilities::getPercentage(windowWidth, 0.125f, 165.f * 2.75f);
#else
    return GUI::Utilities::getPercentage(windowWidth, 0.125f, 165.f);
#endif
    }
}

void ModelsState::handleResize() {
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    auto window = renderer->getWindow();
    auto& renderViews = window->getRenderViews();
    float mainMenuHeight = GUI::Utilities::getMainMenuHeight(window->getHeight());
    float modelMenuWidth = getModelMenuWidth(window->getWidth());

    LUG_ASSERT(renderViews.size() > 0, "There should be at least 1 render view");

    if (_displayFullscreen)
    {
        renderViews[0]->getInfo().viewport.extent.width = 1.0f;
        renderViews[0]->getInfo().viewport.extent.height = 1.0f;
        renderViews[0]->getInfo().viewport.offset.x = 0;
        renderViews[0]->getInfo().viewport.offset.y = 0;
        renderViews[0]->update();
    }
    else
    {
        renderViews[0]->getInfo().viewport.extent.width = 1.0f - modelMenuWidth / window->getWidth();
        renderViews[0]->getInfo().viewport.extent.height = 1.0f - mainMenuHeight / window->getHeight();
        renderViews[0]->getInfo().viewport.offset.x = modelMenuWidth / window->getWidth();
        renderViews[0]->getInfo().viewport.offset.y = mainMenuHeight / window->getHeight();
        renderViews[0]->update();
    }
}
