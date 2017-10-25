#include "InfoState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

#include "BenchmarkingState.hpp"

#include "ModelsState.hpp"
#include "ContactState.hpp"
#include "ResultsState.hpp"
#include "BenchmarksState.hpp"


#include <IconsFontAwesome.h>

InfoState::InfoState(LugBench::Application &application) : AState(application) {
    GUI::setDefaultStyle();
}

InfoState::~InfoState() {
    LUG_LOG.info("InfoState destructor");
}

bool InfoState::onPush() {

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

        lightBuilder.setType(lug::Graphics::Render::Light::Type::Ambient);
        lightBuilder.setColor({ 0.05f, 0.05f, 0.05f, 0.05f });

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
        node->setPosition({ 3.0f, 4.0f, 3.0f }, lug::Graphics::Node::TransformSpace::World);
        // Look at once
        node->getCamera()->lookAt({ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, lug::Graphics::Node::TransformSpace::World);

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

bool InfoState::onPop() {
    LUG_LOG.info("InfoState onPop");
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
    _scene = nullptr;
    return true;
}

void InfoState::onEvent(const lug::Window::Event& event) {
    if (_application.isSending()) {
        return;
    }
    if (event.type == lug::Window::Event::Type::Close) {
        _application.close();
    }
}

bool InfoState::onFrame(const lug::System::Time& /*elapsedTime*/) {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();
    float mainMenuHeight;
#if defined(LUG_SYSTEM_ANDROID)
    mainMenuHeight = GUI::Utilities::getPercentage(window->getHeight(), 0.05f, 120.f);
#else
    mainMenuHeight = GUI::Utilities::getPercentage(window->getHeight(), 0.05f, 60.f);
#endif
    ImGui::Begin("Main Menu", &_isOpen, window_flags);
    {
        ImVec2 mainMenuSize{ static_cast<float>(window->getWidth()), mainMenuHeight };
        ImVec2 mainMenuPos = { 0, 0 };

        ImGui::SetWindowSize(mainMenuSize);
        ImGui::SetWindowPos(mainMenuPos);
        ImGui::SetCursorPos(ImVec2{ 0.f, 0.f });

        ImVec2 headerSize = { static_cast<float>(window->getWidth()), mainMenuHeight };

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f,0.f });
        {
            ImGui::BeginChild("header", headerSize);
            {
#if defined(LUG_SYSTEM_ANDROID)
                ImGui::SetWindowFontScale(1.5f);
#else
                //ImGui::SetWindowFontScale(1.f);
#endif
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
#if defined(LUG_SYSTEM_ANDROID)
                    //ImGui::SetWindowFontScale(1.f);
#else
                    ImGui::SetWindowFontScale(0.67f);
#endif

                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 150.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 150.f, headerSize.y };
#endif
                        ImGui::SameLine();
                        if (ImGui::Button("BENCHMARKS", buttonSize)) {
                            std::shared_ptr<AState> benchmarkingState;
                            benchmarkingState = std::make_shared<BenchmarksState>(_application);
                            _application.popState();
                            _application.pushState(benchmarkingState);
                        }                        
                    }

                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 100.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 100.f, headerSize.y };
#endif
                        ImGui::SameLine();
                        if (ImGui::Button("MODELS", buttonSize)) {
                            std::shared_ptr<AState> benchmarkingState;
                            benchmarkingState = std::make_shared<ModelsState>(_application);
                            _application.popState();
                            _application.pushState(benchmarkingState);
                        }
                    }

                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(.31f, .67f, .98f, 1.00f));
                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 60.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 60.f, headerSize.y };
#endif
                        ImGui::SameLine();
                        ImGui::Button("INFO", buttonSize);
                    }
                    ImGui::PopStyleColor(2);

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

    ImVec2 infoWindowSize{ GUI::Utilities::getPercentage(window->getWidth(), 0.35f, 300.f), static_cast<float>(window->getHeight()) - mainMenuHeight };

    ImGui::Begin("Info Window", &_isOpen, window_flags);
    {
        ImGui::SetWindowSize(infoWindowSize);
        ImGui::SetWindowPos(ImVec2{ 0.f, mainMenuHeight });

#if defined(LUG_SYSTEM_ANDROID)
        ImVec2 deviceWindowSize{ GUI::Utilities::getPercentage(ImGui::GetWindowWidth(), 0.9f), 250.f };
#else
        ImVec2 deviceWindowSize{ GUI::Utilities::getPercentage(ImGui::GetWindowWidth(), 0.9f), 150.f };
#endif
        float padding = (ImGui::GetWindowWidth() - deviceWindowSize.x) / 2.f;
        ImGui::SetCursorPos(ImVec2{ padding , 20.f });
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 5.f, 2.f });
            {
                ImGui::BeginChild("Device Window", deviceWindowSize);
                {
                    ImGui::SetCursorPos(ImVec2{ 20.f , 0.f });
                    ImGui::BeginChild("Device Info");
                    {
                        ImGui::Text("Device");
                        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
                        {
                            ImGui::Text("Name: Geforce gtx 960M");
                            ImGui::Text("Driver Version: 385.164.0");
                            ImGui::Text("API Version: 41953667");
                        }
                        ImGui::PopFont();
                    }
                    ImGui::EndChild();
                }
                ImGui::EndChild();

                ImVec2 childWindowHeaderSize;
#if defined(LUG_SYSTEM_ANDROID)
                const float childHeaderHeight = 70.f;
                const float childHeight = 400.f;
                ImGui::SetCursorPos(ImVec2{ padding , 310.f });
#else
                const float childHeaderHeight = 40.f;
                const float childHeight = 300.f;
                ImGui::SetCursorPos(ImVec2{ padding , 190.f });
#endif

                childWindowHeaderSize = ImVec2{ deviceWindowSize.x, childHeaderHeight };

                ImVec2 childWindowSize{ childWindowHeaderSize.x , childHeight };

                ImGui::BeginGroup();
                {
                    // First window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(.31f, .67f, .98f, 1.00f));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Title Main", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("DEVICE");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Info Main", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::Text("Plop");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();
                    }
                    ImGui::PopFont();
                }
                ImGui::EndGroup();
            }
            ImGui::PopStyleVar(2);
        }
        ImGui::PopStyleColor(2);
    }
    ImGui::End();

    ImGui::Begin("Info Extra Window", &_isOpen, window_flags);
    {
        ImVec2 infoExtraWindowSize{ window->getWidth() - infoWindowSize.x, infoWindowSize.y };

        ImGui::SetWindowSize(infoExtraWindowSize);
        ImGui::SetWindowPos(ImVec2{ infoWindowSize.x, mainMenuHeight });

        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 5.f, 2.f });
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });
            {
                ImVec2 uiSize = ImGui::GetWindowSize();
                ImVec2 childWindowHeaderSize;
#if defined(LUG_SYSTEM_ANDROID)
                const float childMinWidth = 1000.f;
                const float childHeaderHeight = 70.f;
                const float childHeight = 400.f;
                const float childSpacing = 30.f;
                //const float minSizeDoubleElements = (childMinWidth * 2.f) + (childSpacing * 3.f);
#else
                const float childMinWidth = 405.f;
                const float childHeaderHeight = 40.f;
                const float childHeight = 400.f;
                const float childSpacing = 20.f;
                const float minSizeDoubleElements = (childMinWidth * 2.f) + (childSpacing * 3.f);
#endif

#if defined(LUG_SYSTEM_ANDROID)
                {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, 20.f });
                }
#else

                if (uiSize.x >= minSizeDoubleElements) {
                    childWindowHeaderSize = ImVec2{ GUI::Utilities::getPercentage(uiSize.x - (childSpacing * 3.f), 0.5f, childMinWidth), childHeaderHeight };
                    ImGui::SetCursorPos(ImVec2{ childSpacing, 20.f });
                }
                else {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, 20.f });
                }
#endif

                ImVec2 childWindowSize{ childWindowHeaderSize.x, childHeight };

                ImGui::BeginGroup();
                {
                    // First window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(.31f, .67f, .98f, 1.00f));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Title 1", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("DEVICE");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Info 1", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                static bool displayExtraDeviceInfo = false;

                                if (displayExtraDeviceInfo == false) {
                                    if (ImGui::Button(ICON_FA_PLUS_SQUARE)) {
                                        displayExtraDeviceInfo = true;
                                    }
                                }
                                else {
                                    if (ImGui::Button(ICON_FA_MINUS_SQUARE)) {
                                        displayExtraDeviceInfo = false;
                                    }
                                }
                                GUI::displayConfigInfoString("Name", _physicalDeviceInfo->properties.deviceName);
                                GUI::displayConfigInfoVersion("Driver Version", lug::Core::Version::fromInt(_physicalDeviceInfo->properties.driverVersion));
                                GUI::displayConfigInfoValue("API Version", lug::Core::Version::fromInt(_physicalDeviceInfo->properties.apiVersion));
                                if (displayExtraDeviceInfo == true) {
                                    GUI::displayConfigInfoValue("Device ID", _physicalDeviceInfo->properties.deviceID);
                                    GUI::displayConfigInfoValue("Vendor ID", _physicalDeviceInfo->properties.vendorID);
                                    GUI::displayConfigInfoString("Device Type", lug::Graphics::Vulkan::API::RTTI::toStr(_physicalDeviceInfo->properties.deviceType));
                                    GUI::displayConfigInfoArrayUint8("Pipeline Cache UUID", std::vector<uint8_t>(std::begin(_physicalDeviceInfo->properties.pipelineCacheUUID), std::end(_physicalDeviceInfo->properties.pipelineCacheUUID)));
                                    if (ImGui::CollapsingHeader("Limits")) {
                                        ImGui::Indent();
                                        {
                                            GUI::displayDeviceLimits(_physicalDeviceInfo);
                                        }
                                        ImGui::Unindent();
                                    }
                                    if (ImGui::CollapsingHeader("Sparse Properties")) {
                                        ImGui::Indent();
                                        {
                                            GUI::displayConfigInfoBool("Residency Standard 2D Block Shape", _physicalDeviceInfo->properties.sparseProperties.residencyStandard2DBlockShape);
                                            GUI::displayConfigInfoBool("Residency Standard 2D Multisample Block Shape", _physicalDeviceInfo->properties.sparseProperties.residencyStandard2DMultisampleBlockShape);
                                            GUI::displayConfigInfoBool("Residency Standard 3D Block Shape", _physicalDeviceInfo->properties.sparseProperties.residencyStandard3DBlockShape);
                                            GUI::displayConfigInfoBool("Residency Aligned Mip Size", _physicalDeviceInfo->properties.sparseProperties.residencyAlignedMipSize);
                                            GUI::displayConfigInfoBool("Residency Non Resident Strict", _physicalDeviceInfo->properties.sparseProperties.residencyNonResidentStrict);

                                        }
                                        ImGui::Unindent();
                                    }
                                }
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();
                    }
                    ImGui::PopFont();
                }
                ImGui::EndGroup();

#if defined(LUG_SYSTEM_ANDROID)
                {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, childHeaderHeight + childHeight + (childSpacing * 2.f) });
                }
#else
                if (uiSize.x >= minSizeDoubleElements) {
                    childWindowHeaderSize = ImVec2{ GUI::Utilities::getPercentage(uiSize.x - (childSpacing * 3.f), 0.5f, childMinWidth), childHeaderHeight };
                    ImGui::SetCursorPos(ImVec2{ childWindowHeaderSize.x + (childSpacing * 2.f), 20.f });
                }
                else {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, childHeaderHeight + childHeight + (childSpacing * 2.f) });
                }
#endif

                ImGui::BeginGroup();
                {
                    // First window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(.31f, .67f, .98f, 1.00f));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Title 2", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("DEVICE");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Info 2", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::Text("Plop");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();
                    }
                    ImGui::PopFont();
                }
                ImGui::EndGroup();

#if defined(LUG_SYSTEM_ANDROID)
                {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, ((childHeaderHeight + childHeight) * 2.f) + (childSpacing * 3.f) });
                }
#else
                if (uiSize.x >= minSizeDoubleElements) {
                    childWindowHeaderSize = ImVec2{ GUI::Utilities::getPercentage(uiSize.x - (childSpacing * 3.f), 0.5f, childMinWidth), childHeaderHeight };
                    ImGui::SetCursorPos(ImVec2{ childSpacing, childHeaderHeight + childHeight + (childSpacing * 2.f) });
                }
                else {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, ((childHeaderHeight + childHeight) * 2.f) + (childSpacing * 3.f) });
                }
#endif

                ImGui::BeginGroup();
                {
                    // First window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(.31f, .67f, .98f, 1.00f));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Title 3", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("DEVICE");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Info 3", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::Text("Plop");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();
                    }
                    ImGui::PopFont();
                }
                ImGui::EndGroup();

#if defined(LUG_SYSTEM_ANDROID)
                {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, ((childHeaderHeight + childHeight) * 3.f) + (childSpacing * 4.f) });
                }
#else
                if (uiSize.x >= minSizeDoubleElements) {
                    childWindowHeaderSize = ImVec2{ GUI::Utilities::getPercentage(uiSize.x - (childSpacing * 3.f), 0.5f, childMinWidth), childHeaderHeight };
                    ImGui::SetCursorPos(ImVec2{ childWindowHeaderSize.x + (childSpacing * 2.f), childHeaderHeight + childHeight + (childSpacing * 2.f) });
                }
                else {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, ((childHeaderHeight + childHeight) * 3.f) + (childSpacing * 4.f) });
                }
#endif

                ImGui::BeginGroup();
                {
                    // First window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(.31f, .67f, .98f, 1.00f));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Title 4", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("DEVICE");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Info 4", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::Text("Plop");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();
                    }
                    ImGui::PopFont();
                }
                ImGui::EndGroup();

            }
            ImGui::PopStyleVar(2);
        }
        ImGui::PopStyleColor(2);
    }
    ImGui::End();

    return true;
}
