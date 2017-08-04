#include "MenuState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

#include <lug/System/Time.hpp>

#include "BenchmarkingState.hpp"

MenuState::MenuState(Application &application) : AState(application) {}

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
    if (_application.isSendingDevice || _application.isSendingScore) {
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

        ImGui::Begin("Sample Window", &isOpen, window_flags);
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

        ImGui::Begin("Main Menu", &isOpen, window_flags);
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

            // Sendind result bar
            if (_application.isSendingDevice || _application.isSendingScore) {
                // Centers the button and keeps it square at all times
                ImVec2 buttonSize = {windowSize.x, windowSize.y};
                // if (windowSize.x > windowSize.y) { buttonSize = { (windowSize.y / 3.f), (windowSize.y / 3.f) }; }
                // else { buttonSize = { (windowSize.x / 3.f), (windowSize.x / 3.f) }; }
                // ImVec2 buttonPos{ centerButtonPos.x + (buttonSize.x * 2.f), centerButtonPos.y + (buttonSize.y / 4.f) };
                ImVec2 buttonPos{ windowSize.x, windowSize.y};
                ImGui::SetCursorPos(buttonPos);
                if (_application.isSendingDevice) {
                    if (ImGui::Button("Sending device in progress...", buttonSize)) {}                    
                } else {
                    if (ImGui::Button("Sending score in progress...", buttonSize)) {}                                        
                }
            }

        }
        ImGui::End();
    } else if (display_info_screen == true) {
        ImGui::Begin("Info Display", &isOpen, window_flags);
        {
            lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

            // Sets the window to be at the bottom of the screen (1/3rd of the height)
            ImVec2 windowSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) };
            ImVec2 windowPos = { 0, 0 };
            ImGui::SetWindowSize(windowSize);
            ImGui::SetWindowPos(windowPos);

            if (ImGui::CollapsingHeader("Device", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Indent();
                {
                    GUI::displayConfigInfoString("Name", _physicalDeviceInfo->properties.deviceName);
                    GUI::displayConfigInfoVersion("Driver Version", lug::Core::Version::fromInt(_physicalDeviceInfo->properties.driverVersion));
                    GUI::displayConfigInfoValue("API Version", lug::Core::Version::fromInt(_physicalDeviceInfo->properties.apiVersion));
                    if (ImGui::CollapsingHeader("Extra Info")) {
                        ImGui::Indent();
                        {
                            GUI::displayConfigInfoValue("Device ID", _physicalDeviceInfo->properties.deviceID);
                            GUI::displayConfigInfoValue("Vendor ID", _physicalDeviceInfo->properties.vendorID);
                            GUI::displayConfigInfoString("Device Type", lug::Graphics::Vulkan::API::RTTI::toStr(_physicalDeviceInfo->properties.deviceType));
                            GUI::displayConfigInfoArrayUint8("Pipeline Cache UUID", std::vector<uint8_t>(std::begin(_physicalDeviceInfo->properties.pipelineCacheUUID), std::end(_physicalDeviceInfo->properties.pipelineCacheUUID)));
                            if (ImGui::CollapsingHeader("Limits")) {
                                ImGui::Indent();
                                {
                                    GUI::displayConfigInfoValue("Max Image Dimension 1D", _physicalDeviceInfo->properties.limits.maxImageDimension1D);
                                    GUI::displayConfigInfoValue("Max Image Dimension 2D", _physicalDeviceInfo->properties.limits.maxImageDimension2D);
                                    GUI::displayConfigInfoValue("Max Image Dimension 3D", _physicalDeviceInfo->properties.limits.maxImageDimension3D);
                                    GUI::displayConfigInfoValue("Max Image Dimension Cube", _physicalDeviceInfo->properties.limits.maxImageDimensionCube);
                                    GUI::displayConfigInfoValue("Max Image Array Layers", _physicalDeviceInfo->properties.limits.maxImageArrayLayers);
                                    GUI::displayConfigInfoValue("Max Texel Buffer Elements", _physicalDeviceInfo->properties.limits.maxTexelBufferElements);
                                    GUI::displayConfigInfoValue("Max Uniform Buffer Range", _physicalDeviceInfo->properties.limits.maxUniformBufferRange);
                                    GUI::displayConfigInfoValue("Max Storage Buffer Range", _physicalDeviceInfo->properties.limits.maxStorageBufferRange);
                                    GUI::displayConfigInfoValue("Max Push Constants Size", _physicalDeviceInfo->properties.limits.maxPushConstantsSize);
                                    GUI::displayConfigInfoValue("Max Memory Allocation Count", _physicalDeviceInfo->properties.limits.maxMemoryAllocationCount);
                                    GUI::displayConfigInfoValue("Max Sampler Allocation Count", _physicalDeviceInfo->properties.limits.maxSamplerAllocationCount);
                                    GUI::displayConfigInfoUnsignedLongValue("Buffer Image Granularity", _physicalDeviceInfo->properties.limits.bufferImageGranularity);
                                    GUI::displayConfigInfoUnsignedLongValue("Sparse Address Space Size", _physicalDeviceInfo->properties.limits.sparseAddressSpaceSize);
                                    GUI::displayConfigInfoValue("Max Bound Descriptor Sets", _physicalDeviceInfo->properties.limits.maxBoundDescriptorSets);
                                    GUI::displayConfigInfoValue("Max Per Stage Descriptor Samplers", _physicalDeviceInfo->properties.limits.maxPerStageDescriptorSamplers);
                                    GUI::displayConfigInfoValue("Max Per Stage Descriptor Uniform Buffers", _physicalDeviceInfo->properties.limits.maxPerStageDescriptorUniformBuffers);
                                    GUI::displayConfigInfoValue("Max Per Stage Descriptor Storage Buffers", _physicalDeviceInfo->properties.limits.maxPerStageDescriptorStorageBuffers);
                                    GUI::displayConfigInfoValue("Max Per Stage Descriptor Sampled Images", _physicalDeviceInfo->properties.limits.maxPerStageDescriptorSampledImages);
                                    GUI::displayConfigInfoValue("Max Per Stage Descriptor Storage Images", _physicalDeviceInfo->properties.limits.maxPerStageDescriptorStorageImages);
                                    GUI::displayConfigInfoValue("Max Per Stage Descriptor Input Attachments", _physicalDeviceInfo->properties.limits.maxPerStageDescriptorInputAttachments);
                                    GUI::displayConfigInfoValue("Max Per Stage Resources", _physicalDeviceInfo->properties.limits.maxPerStageResources);
                                    GUI::displayConfigInfoValue("Max Descriptor Set Samplers", _physicalDeviceInfo->properties.limits.maxDescriptorSetSamplers);
                                    GUI::displayConfigInfoValue("Max Descriptor Set Uniform Buffers", _physicalDeviceInfo->properties.limits.maxDescriptorSetUniformBuffers);
                                    GUI::displayConfigInfoValue("Max Descriptor Set Uniform Buffers Dynamic", _physicalDeviceInfo->properties.limits.maxDescriptorSetUniformBuffersDynamic);
                                    GUI::displayConfigInfoValue("Max Descriptor Set Storage Buffers", _physicalDeviceInfo->properties.limits.maxDescriptorSetStorageBuffers);
                                    GUI::displayConfigInfoValue("Max Descriptor Set Storage Buffers Dynamic", _physicalDeviceInfo->properties.limits.maxDescriptorSetStorageBuffersDynamic);
                                    GUI::displayConfigInfoValue("Max Descriptor Set Sampled Images", _physicalDeviceInfo->properties.limits.maxDescriptorSetSampledImages);
                                    GUI::displayConfigInfoValue("Max Descriptor Set Storage Images", _physicalDeviceInfo->properties.limits.maxDescriptorSetStorageImages);
                                    GUI::displayConfigInfoValue("Max Descriptor Set Input Attachments", _physicalDeviceInfo->properties.limits.maxDescriptorSetInputAttachments);
                                    GUI::displayConfigInfoValue("Max Vertex Input Attributes", _physicalDeviceInfo->properties.limits.maxVertexInputAttributes);
                                    GUI::displayConfigInfoValue("Max Vertex Input Bindings", _physicalDeviceInfo->properties.limits.maxVertexInputBindings);
                                    GUI::displayConfigInfoValue("Max Vertex Input AttributeOffset", _physicalDeviceInfo->properties.limits.maxVertexInputAttributeOffset);
                                    GUI::displayConfigInfoValue("Max Vertex Input BindingStride", _physicalDeviceInfo->properties.limits.maxVertexInputBindingStride);
                                    GUI::displayConfigInfoValue("Max Vertex Output Components", _physicalDeviceInfo->properties.limits.maxVertexOutputComponents);
                                    GUI::displayConfigInfoValue("Max Tessellation GenerationLevel", _physicalDeviceInfo->properties.limits.maxTessellationGenerationLevel);
                                    GUI::displayConfigInfoValue("Max Tessellation PatchSize", _physicalDeviceInfo->properties.limits.maxTessellationPatchSize);
                                    GUI::displayConfigInfoValue("Max Tessellation Control Per Vertex Input Components", _physicalDeviceInfo->properties.limits.maxTessellationControlPerVertexInputComponents);
                                    GUI::displayConfigInfoValue("Max Tessellation Control Per Vertex Output Components", _physicalDeviceInfo->properties.limits.maxTessellationControlPerVertexOutputComponents);
                                    GUI::displayConfigInfoValue("Max Tessellation Control Per Patch Output Components", _physicalDeviceInfo->properties.limits.maxTessellationControlPerPatchOutputComponents);
                                    GUI::displayConfigInfoValue("Max Tessellation Control Total Output Components", _physicalDeviceInfo->properties.limits.maxTessellationControlTotalOutputComponents);
                                    GUI::displayConfigInfoValue("Max Tessellation Evaluation Input Components", _physicalDeviceInfo->properties.limits.maxTessellationEvaluationInputComponents);
                                    GUI::displayConfigInfoValue("Max Tessellation Evaluation Output Components", _physicalDeviceInfo->properties.limits.maxTessellationEvaluationOutputComponents);
                                    GUI::displayConfigInfoValue("Max Geometry Shader Invocations", _physicalDeviceInfo->properties.limits.maxGeometryShaderInvocations);
                                    GUI::displayConfigInfoValue("Max Geometry Input Components", _physicalDeviceInfo->properties.limits.maxGeometryInputComponents);
                                    GUI::displayConfigInfoValue("Max Geometry Output Components", _physicalDeviceInfo->properties.limits.maxGeometryOutputComponents);
                                    GUI::displayConfigInfoValue("Max Geometry Output Vertices", _physicalDeviceInfo->properties.limits.maxGeometryOutputVertices);
                                    GUI::displayConfigInfoValue("Max Geometry Total Output Components", _physicalDeviceInfo->properties.limits.maxGeometryTotalOutputComponents);
                                    GUI::displayConfigInfoValue("Max Fragment Input Components", _physicalDeviceInfo->properties.limits.maxFragmentInputComponents);
                                    GUI::displayConfigInfoValue("Max Fragment Output Attachments", _physicalDeviceInfo->properties.limits.maxFragmentOutputAttachments);
                                    GUI::displayConfigInfoValue("Max Fragment Dual SrcAttachments", _physicalDeviceInfo->properties.limits.maxFragmentDualSrcAttachments);
                                    GUI::displayConfigInfoValue("Max Fragment Combined Output Resources", _physicalDeviceInfo->properties.limits.maxFragmentCombinedOutputResources);
                                    GUI::displayConfigInfoValue("Max Compute Shared Memory Size", _physicalDeviceInfo->properties.limits.maxComputeSharedMemorySize);
                                    GUI::displayConfigInfoArrayUint32("Max Compute Work Group Count", std::vector<uint32_t>(std::begin(_physicalDeviceInfo->properties.limits.maxComputeWorkGroupCount), std::end(_physicalDeviceInfo->properties.limits.maxComputeWorkGroupCount)));
                                    GUI::displayConfigInfoValue("Max Compute Work Group Invocations", _physicalDeviceInfo->properties.limits.maxComputeWorkGroupInvocations);
                                    GUI::displayConfigInfoArrayUint32("Max Compute Work Group Size", std::vector<uint32_t>(std::begin(_physicalDeviceInfo->properties.limits.maxComputeWorkGroupSize), std::end(_physicalDeviceInfo->properties.limits.maxComputeWorkGroupSize)));
                                    GUI::displayConfigInfoValue("Sub Pixel Precision Bits", _physicalDeviceInfo->properties.limits.subPixelPrecisionBits);
                                    GUI::displayConfigInfoValue("Sub Texel Precision Bits", _physicalDeviceInfo->properties.limits.subTexelPrecisionBits);
                                    GUI::displayConfigInfoValue("Mipmap Precision Bits", _physicalDeviceInfo->properties.limits.mipmapPrecisionBits);
                                    GUI::displayConfigInfoValue("Max Draw Indexed Index Value", _physicalDeviceInfo->properties.limits.maxDrawIndexedIndexValue);
                                    GUI::displayConfigInfoValue("Max Draw Indirect Count", _physicalDeviceInfo->properties.limits.maxDrawIndirectCount);
                                    GUI::displayConfigInfoFloatValue("Max Sampler Lod Bias", _physicalDeviceInfo->properties.limits.maxSamplerLodBias);
                                    GUI::displayConfigInfoFloatValue("Max Sampler Anisotropy", _physicalDeviceInfo->properties.limits.maxSamplerAnisotropy);
                                    GUI::displayConfigInfoValue("Max Viewports", _physicalDeviceInfo->properties.limits.maxViewports);
                                    GUI::displayConfigInfoArrayUint32("Max Viewport Dimensions", std::vector<uint32_t>(std::begin(_physicalDeviceInfo->properties.limits.maxViewportDimensions), std::end(_physicalDeviceInfo->properties.limits.maxViewportDimensions)));
                                    GUI::displayConfigInfoArrayFloat("Viewport Bounds Range", std::vector<float>(std::begin(_physicalDeviceInfo->properties.limits.viewportBoundsRange), std::end(_physicalDeviceInfo->properties.limits.viewportBoundsRange)));
                                    GUI::displayConfigInfoValue("Viewport Sub Pixel Bits", _physicalDeviceInfo->properties.limits.viewportSubPixelBits);
                                    GUI::displayConfigInfoUnsignedLongValue("Min Memory Map Alignment", _physicalDeviceInfo->properties.limits.minMemoryMapAlignment);
                                    GUI::displayConfigInfoUnsignedLongValue("Min Texel Buffer Offset Alignment", _physicalDeviceInfo->properties.limits.minTexelBufferOffsetAlignment);
                                    GUI::displayConfigInfoUnsignedLongValue("Min Uniform Buffer Offset Alignment", _physicalDeviceInfo->properties.limits.minUniformBufferOffsetAlignment);
                                    GUI::displayConfigInfoUnsignedLongValue("Min Storage Buffer Offset Alignment", _physicalDeviceInfo->properties.limits.minStorageBufferOffsetAlignment);
                                    GUI::displayConfigInfoValue("Min Texel Offset", _physicalDeviceInfo->properties.limits.minTexelOffset);
                                    GUI::displayConfigInfoValue("Max Texel Offset", _physicalDeviceInfo->properties.limits.maxTexelOffset);
                                    GUI::displayConfigInfoValue("Min Texel Gather Offset", _physicalDeviceInfo->properties.limits.minTexelGatherOffset);
                                    GUI::displayConfigInfoValue("Max Texel Gather Offset", _physicalDeviceInfo->properties.limits.maxTexelGatherOffset);
                                    GUI::displayConfigInfoFloatValue("Min Interpolation Offset", _physicalDeviceInfo->properties.limits.minInterpolationOffset);
                                    GUI::displayConfigInfoFloatValue("Max InterpolationOffset", _physicalDeviceInfo->properties.limits.maxInterpolationOffset);
                                    GUI::displayConfigInfoValue("Sub Pixel Interpolation Offset Bits", _physicalDeviceInfo->properties.limits.subPixelInterpolationOffsetBits);
                                    GUI::displayConfigInfoValue("Max FramebufferWidth", _physicalDeviceInfo->properties.limits.maxFramebufferWidth);
                                    GUI::displayConfigInfoValue("Max FramebufferHeight", _physicalDeviceInfo->properties.limits.maxFramebufferHeight);
                                    GUI::displayConfigInfoValue("Max FramebufferLayers", _physicalDeviceInfo->properties.limits.maxFramebufferLayers);
                                    GUI::displayConfigInfoValue("Framebuffer Color Sample Counts", _physicalDeviceInfo->properties.limits.framebufferColorSampleCounts);
                                    GUI::displayConfigInfoValue("Framebuffer Depth Sample Counts", _physicalDeviceInfo->properties.limits.framebufferDepthSampleCounts);
                                    GUI::displayConfigInfoValue("Framebuffer Stencil Sample Counts", _physicalDeviceInfo->properties.limits.framebufferStencilSampleCounts);
                                    GUI::displayConfigInfoValue("Framebuffer No Attachments Sample Counts", _physicalDeviceInfo->properties.limits.framebufferNoAttachmentsSampleCounts);
                                    GUI::displayConfigInfoValue("Max Color Attachments", _physicalDeviceInfo->properties.limits.maxColorAttachments);
                                    GUI::displayConfigInfoValue("Sampled Image Color Sample Counts", _physicalDeviceInfo->properties.limits.sampledImageColorSampleCounts);
                                    GUI::displayConfigInfoValue("Sampled Image Integer Sample Counts", _physicalDeviceInfo->properties.limits.sampledImageIntegerSampleCounts);
                                    GUI::displayConfigInfoValue("Sampled Image Depth Sample Counts", _physicalDeviceInfo->properties.limits.sampledImageDepthSampleCounts);
                                    GUI::displayConfigInfoValue("Sampled Image Stencil Sample Counts", _physicalDeviceInfo->properties.limits.sampledImageStencilSampleCounts);
                                    GUI::displayConfigInfoValue("Storage Image Sample Counts", _physicalDeviceInfo->properties.limits.storageImageSampleCounts);
                                    GUI::displayConfigInfoValue("Max SampleMaskWords", _physicalDeviceInfo->properties.limits.maxSampleMaskWords);
                                    GUI::displayConfigInfoValue("Timestamp Compute And Graphics", _physicalDeviceInfo->properties.limits.timestampComputeAndGraphics);
                                    GUI::displayConfigInfoFloatValue("Timestamp Period", _physicalDeviceInfo->properties.limits.timestampPeriod);
                                    GUI::displayConfigInfoValue("Max Clip Distances", _physicalDeviceInfo->properties.limits.maxClipDistances);
                                    GUI::displayConfigInfoValue("Max Cull Distances", _physicalDeviceInfo->properties.limits.maxCullDistances);
                                    GUI::displayConfigInfoValue("Max Combined Clip And Cull Distances", _physicalDeviceInfo->properties.limits.maxCombinedClipAndCullDistances);
                                    GUI::displayConfigInfoValue("Discrete Queue Priorities", _physicalDeviceInfo->properties.limits.discreteQueuePriorities);
                                    GUI::displayConfigInfoArrayFloat("Point Size Range", std::vector<float>(std::begin(_physicalDeviceInfo->properties.limits.pointSizeRange), std::end(_physicalDeviceInfo->properties.limits.pointSizeRange)));
                                    GUI::displayConfigInfoArrayFloat("Line Width Range", std::vector<float>(std::begin(_physicalDeviceInfo->properties.limits.lineWidthRange), std::end(_physicalDeviceInfo->properties.limits.lineWidthRange)));
                                    GUI::displayConfigInfoFloatValue("Point Size Granularity", _physicalDeviceInfo->properties.limits.pointSizeGranularity);
                                    GUI::displayConfigInfoFloatValue("Line Width Granularity", _physicalDeviceInfo->properties.limits.lineWidthGranularity);
                                    GUI::displayConfigInfoValue("Strict Lines", _physicalDeviceInfo->properties.limits.strictLines);
                                    GUI::displayConfigInfoValue("Standard Sample Locations", _physicalDeviceInfo->properties.limits.standardSampleLocations);
                                    GUI::displayConfigInfoUnsignedLongValue("Optimal Buffer Copy Offset Alignment", _physicalDeviceInfo->properties.limits.optimalBufferCopyOffsetAlignment);
                                    GUI::displayConfigInfoUnsignedLongValue("Optimal Buffer Copy Row Pitch Alignment", _physicalDeviceInfo->properties.limits.optimalBufferCopyRowPitchAlignment);
                                    GUI::displayConfigInfoUnsignedLongValue("Non Coherent AtomSize", _physicalDeviceInfo->properties.limits.nonCoherentAtomSize);
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
                            ImGui::Unindent();
                        }
                        ImGui::Unindent();
                    }
                    if (ImGui::CollapsingHeader("Features")) {
                        ImGui::Indent();
                        {
                            GUI::displayConfigInfoBool("Robust Buffer Access", _physicalDeviceInfo->features.robustBufferAccess);
                            GUI::displayConfigInfoBool("Full Draw Index Uint32", _physicalDeviceInfo->features.fullDrawIndexUint32);
                            GUI::displayConfigInfoBool("Image Cube Array", _physicalDeviceInfo->features.imageCubeArray);
                            GUI::displayConfigInfoBool("Independent Blend", _physicalDeviceInfo->features.independentBlend);
                            GUI::displayConfigInfoBool("Geometry Shader", _physicalDeviceInfo->features.geometryShader);
                            GUI::displayConfigInfoBool("Tessellation Shader", _physicalDeviceInfo->features.tessellationShader);
                            GUI::displayConfigInfoBool("Sample Rate Shading", _physicalDeviceInfo->features.sampleRateShading);
                            GUI::displayConfigInfoBool("Dual Src Blend", _physicalDeviceInfo->features.dualSrcBlend);
                            GUI::displayConfigInfoBool("LogicOp", _physicalDeviceInfo->features.logicOp);
                            GUI::displayConfigInfoBool("Multi Draw Indirect", _physicalDeviceInfo->features.multiDrawIndirect);
                            GUI::displayConfigInfoBool("Draw Indirect First Instance", _physicalDeviceInfo->features.drawIndirectFirstInstance);
                            GUI::displayConfigInfoBool("Depth Clamp", _physicalDeviceInfo->features.depthClamp);
                            GUI::displayConfigInfoBool("Depth Bias Clamp", _physicalDeviceInfo->features.depthBiasClamp);
                            GUI::displayConfigInfoBool("Fill Mode Non Solid", _physicalDeviceInfo->features.fillModeNonSolid);
                            GUI::displayConfigInfoBool("Depth Bounds", _physicalDeviceInfo->features.depthBounds);
                            GUI::displayConfigInfoBool("Wide Lines", _physicalDeviceInfo->features.wideLines);
                            GUI::displayConfigInfoBool("Large Points", _physicalDeviceInfo->features.largePoints);
                            GUI::displayConfigInfoBool("Alpha To One", _physicalDeviceInfo->features.alphaToOne);
                            GUI::displayConfigInfoBool("Multi Viewport", _physicalDeviceInfo->features.multiViewport);
                            GUI::displayConfigInfoBool("Sampler Anisotropy", _physicalDeviceInfo->features.samplerAnisotropy);
                            GUI::displayConfigInfoBool("Texture Compression ETC2", _physicalDeviceInfo->features.textureCompressionETC2);
                            GUI::displayConfigInfoBool("Texture Compression ASTC_LDR", _physicalDeviceInfo->features.textureCompressionASTC_LDR);
                            GUI::displayConfigInfoBool("Texture Compression BC", _physicalDeviceInfo->features.textureCompressionBC);
                            GUI::displayConfigInfoBool("Occlusion Query Precise", _physicalDeviceInfo->features.occlusionQueryPrecise);
                            GUI::displayConfigInfoBool("Pipeline Statistics Query", _physicalDeviceInfo->features.pipelineStatisticsQuery);
                            GUI::displayConfigInfoBool("Vertex Pipeline Stores And Atomics", _physicalDeviceInfo->features.vertexPipelineStoresAndAtomics);
                            GUI::displayConfigInfoBool("Fragment Stores And Atomics", _physicalDeviceInfo->features.fragmentStoresAndAtomics);
                            GUI::displayConfigInfoBool("Shader Tessellation And Geometry Point Size", _physicalDeviceInfo->features.shaderTessellationAndGeometryPointSize);
                            GUI::displayConfigInfoBool("Shader Image Gather Extended", _physicalDeviceInfo->features.shaderImageGatherExtended);
                            GUI::displayConfigInfoBool("Shader Storage Image Extended Formats", _physicalDeviceInfo->features.shaderStorageImageExtendedFormats);
                            GUI::displayConfigInfoBool("Shader Storage Image Multisample", _physicalDeviceInfo->features.shaderStorageImageMultisample);
                            GUI::displayConfigInfoBool("Shader Storage Image Read Without Format", _physicalDeviceInfo->features.shaderStorageImageReadWithoutFormat);
                            GUI::displayConfigInfoBool("Shader Storage Image Write Without Format", _physicalDeviceInfo->features.shaderStorageImageWriteWithoutFormat);
                            GUI::displayConfigInfoBool("Shader Uniform Buffer Array Dynamic Indexing", _physicalDeviceInfo->features.shaderUniformBufferArrayDynamicIndexing);
                            GUI::displayConfigInfoBool("Shader Sampled Image Array Dynamic Indexing", _physicalDeviceInfo->features.shaderSampledImageArrayDynamicIndexing);
                            GUI::displayConfigInfoBool("Shader Storage Buffer Array Dynamic Indexing", _physicalDeviceInfo->features.shaderStorageBufferArrayDynamicIndexing);
                            GUI::displayConfigInfoBool("Shader Storage Image Array Dynamic Indexing", _physicalDeviceInfo->features.shaderStorageImageArrayDynamicIndexing);
                            GUI::displayConfigInfoBool("Shader Clip Distance", _physicalDeviceInfo->features.shaderClipDistance);
                            GUI::displayConfigInfoBool("Shader Cull Distance", _physicalDeviceInfo->features.shaderCullDistance);
                            GUI::displayConfigInfoBool("Shader Float64", _physicalDeviceInfo->features.shaderFloat64);
                            GUI::displayConfigInfoBool("Shader Int64", _physicalDeviceInfo->features.shaderInt64);
                            GUI::displayConfigInfoBool("Shader Int16", _physicalDeviceInfo->features.shaderInt16);
                            GUI::displayConfigInfoBool("Shader ResourceResidency", _physicalDeviceInfo->features.shaderResourceResidency);
                            GUI::displayConfigInfoBool("Shader ResourceMinLod", _physicalDeviceInfo->features.shaderResourceMinLod);
                            GUI::displayConfigInfoBool("Sparse Binding", _physicalDeviceInfo->features.sparseBinding);
                            GUI::displayConfigInfoBool("Sparse Residency Buffer", _physicalDeviceInfo->features.sparseResidencyBuffer);
                            GUI::displayConfigInfoBool("Sparse Residency Image2D", _physicalDeviceInfo->features.sparseResidencyImage2D);
                            GUI::displayConfigInfoBool("Sparse Residency Image3D", _physicalDeviceInfo->features.sparseResidencyImage3D);
                            GUI::displayConfigInfoBool("Sparse Residency 2Samples", _physicalDeviceInfo->features.sparseResidency2Samples);
                            GUI::displayConfigInfoBool("Sparse Residency 4Samples", _physicalDeviceInfo->features.sparseResidency4Samples);
                            GUI::displayConfigInfoBool("Sparse Residency 8Samples", _physicalDeviceInfo->features.sparseResidency8Samples);
                            GUI::displayConfigInfoBool("Sparse Residency 16Samples", _physicalDeviceInfo->features.sparseResidency16Samples);
                            GUI::displayConfigInfoBool("Sparse Residency Aliased", _physicalDeviceInfo->features.sparseResidencyAliased);
                            GUI::displayConfigInfoBool("Variable Multisample Rate", _physicalDeviceInfo->features.variableMultisampleRate);
                            GUI::displayConfigInfoBool("Inherited Queries", _physicalDeviceInfo->features.inheritedQueries);
                        }
                        ImGui::Unindent();
                    }

                    if (ImGui::CollapsingHeader("Memory")) {
                        ImGui::Indent();
                        {
                            GUI::displayConfigInfoUnsignedLongValue("Memory Type Count", _physicalDeviceInfo->memoryProperties.memoryTypeCount);
                            GUI::displayConfigInfoUnsignedLongValue("Memory Heap Count", _physicalDeviceInfo->memoryProperties.memoryHeapCount);

                            if (ImGui::CollapsingHeader("Memory Types")) {
                                for (size_t i = 0; i < _physicalDeviceInfo->memoryProperties.memoryTypeCount; ++i) {
                                    ImGui::TextColored(ImVec4(0, 255, 0, 255), "Type %lu", i);
                                    ImGui::Indent();
                                    {
                                        std::vector<const char*> propertyFlags = lug::Graphics::Vulkan::API::RTTI::VkMemoryPropertyFlagsToStrVec(_physicalDeviceInfo->memoryProperties.memoryTypes[i].propertyFlags);
                                        if (propertyFlags.size() > 0) {
                                            ImGui::PushID(static_cast<int>(i));
                                            {
                                                GUI::displayConfigInfoArrayStr("Property Flags", propertyFlags);
                                            }
                                            ImGui::PopID();
                                        }
                                        GUI::displayConfigInfoUnsignedLongValue("Heap Index", _physicalDeviceInfo->memoryProperties.memoryTypes[i].heapIndex);
                                    }
                                    ImGui::Unindent();
                                }
                            }

                            if (ImGui::CollapsingHeader("Memory Heaps")) {
                                for (size_t i = 0; i < _physicalDeviceInfo->memoryProperties.memoryHeapCount; ++i) {
                                    ImGui::TextColored(ImVec4(0, 255, 0, 255), "Heap %lu", i);
                                    ImGui::Indent();
                                    {
                                        GUI::displayConfigInfoUnsignedLongValue("Size", _physicalDeviceInfo->memoryProperties.memoryHeaps[i].size);
                                        std::vector<const char*> flags = lug::Graphics::Vulkan::API::RTTI::VkMemoryHeapFlagsToStrVec(_physicalDeviceInfo->memoryProperties.memoryHeaps[i].flags);
                                        if (flags.size() > 0) {
                                            ImGui::PushID(static_cast<int>(i));
                                            {
                                                GUI::displayConfigInfoArrayStr("Flags", flags);
                                            }
                                            ImGui::PopID();
                                        }
                                    }
                                    ImGui::Unindent();
                                }
                            }
                        }
                        ImGui::Unindent();
                    }

                    if (ImGui::CollapsingHeader("Queues")) {
                        ImGui::Indent();
                        {
                            for (uint32_t i = 0; i < _physicalDeviceInfo->queueFamilies.size(); ++i) {
                                ImGui::PushID(i);
                                {
                                    ImGui::TextColored(ImVec4(0, 255, 0, 255), "Queue %d", i);
                                    ImGui::Indent();
                                    {
                                        VkQueueFamilyProperties queueFamilyProperties = _physicalDeviceInfo->queueFamilies[i];
                                        GUI::displayConfigInfoArrayStr("Queue Flags", lug::Graphics::Vulkan::API::RTTI::VkQueueFlagsToStrVec(queueFamilyProperties.queueFlags));
                                        GUI::displayConfigInfoUnsignedLongValue("Queue Count", queueFamilyProperties.queueCount);
                                        GUI::displayConfigInfoValue("Timestamp Valid Bits", queueFamilyProperties.timestampValidBits);
                                        if (ImGui::CollapsingHeader("Min Image Transfer Granularity")) {
                                            ImGui::Indent();
                                            {
                                                GUI::displayConfigInfoUnsignedLongValue("Width", queueFamilyProperties.minImageTransferGranularity.width);
                                                GUI::displayConfigInfoUnsignedLongValue("Height", queueFamilyProperties.minImageTransferGranularity.height);
                                                GUI::displayConfigInfoUnsignedLongValue("Depth", queueFamilyProperties.minImageTransferGranularity.depth);
                                            }
                                            ImGui::Unindent();
                                        }
                                    }
                                    ImGui::Unindent();
                                }
                                ImGui::PopID();
                            }
                        }
                        ImGui::Unindent();
                    }


                    if (ImGui::CollapsingHeader("Extensions")) {
                        ImGui::Indent();
                        for (auto extension : _physicalDeviceInfo->extensions) {
                            GUI::displayConfigInfoVersion(extension.extensionName, lug::Core::Version::fromInt(extension.specVersion));
                        }
                        ImGui::Unindent();
                    }

                    if (ImGui::CollapsingHeader("Formats")) {
                        ImGui::Indent();
                        for (auto format : _physicalDeviceInfo->formatProperties) {
                            if (ImGui::CollapsingHeader(lug::Graphics::Vulkan::API::RTTI::toStr(format.first))) {
                                ImGui::Indent();
                                GUI::displayConfigInfoArrayStr("Linear Tiling Features", lug::Graphics::Vulkan::API::RTTI::VkFormatFeatureFlagsToStrVec(format.second.linearTilingFeatures));
                                GUI::displayConfigInfoArrayStr("Optimal Tiling Features", lug::Graphics::Vulkan::API::RTTI::VkFormatFeatureFlagsToStrVec(format.second.optimalTilingFeatures));
                                GUI::displayConfigInfoArrayStr("Buffer Features", lug::Graphics::Vulkan::API::RTTI::VkFormatFeatureFlagsToStrVec(format.second.bufferFeatures));
                                ImGui::Unindent();
                            }
                        }
                        ImGui::Unindent();
                    }

                    if (ImGui::CollapsingHeader("Swapchain")) {
                        ImGui::Indent();
                        {
                            GUI::displayConfigInfoUnsignedLongValue("Min Image Count", _physicalDeviceInfo->swapchain.capabilities.minImageCount);
                            GUI::displayConfigInfoUnsignedLongValue("Max Image Count", _physicalDeviceInfo->swapchain.capabilities.maxImageCount);
                            if (ImGui::CollapsingHeader("Current Extent", ImGuiTreeNodeFlags_NoAutoOpenOnLog)) {
                                GUI::displayConfigInfoUnsignedLongValue("Width", _physicalDeviceInfo->swapchain.capabilities.currentExtent.width);
                                GUI::displayConfigInfoUnsignedLongValue("Height", _physicalDeviceInfo->swapchain.capabilities.currentExtent.height);
                            }
                            if (ImGui::CollapsingHeader("Min Image Extent", ImGuiTreeNodeFlags_NoAutoOpenOnLog)) {
                                GUI::displayConfigInfoUnsignedLongValue("Width", _physicalDeviceInfo->swapchain.capabilities.minImageExtent.width);
                                GUI::displayConfigInfoUnsignedLongValue("Height", _physicalDeviceInfo->swapchain.capabilities.minImageExtent.height);
                            }
                            if (ImGui::CollapsingHeader("Max Image Extent", ImGuiTreeNodeFlags_NoAutoOpenOnLog)) {
                                GUI::displayConfigInfoUnsignedLongValue("Width", _physicalDeviceInfo->swapchain.capabilities.maxImageExtent.width);
                                GUI::displayConfigInfoUnsignedLongValue("Height", _physicalDeviceInfo->swapchain.capabilities.maxImageExtent.height);
                            }
                            GUI::displayConfigInfoUnsignedLongValue("Max Image Array Layers", _physicalDeviceInfo->swapchain.capabilities.maxImageArrayLayers);
                        }
                        ImGui::Unindent();
                    }

                }
                ImGui::Unindent();
            }

            ImGui::Separator();
            ImGui::NewLine();
            if (ImGui::Button("< RETURN")) {
                display_info_screen = !display_info_screen;
            }
        }
        ImGui::End();
    } else if (display_result_screen == true) {
        ImGui::Begin("Result Display", &isOpen, window_flags);
        {
            lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

            // Sets the window to be at the bottom of the screen (1/3rd of the height)
            ImVec2 windowSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) };
            ImVec2 windowPos = { 0, 0 };
            ImGui::SetWindowSize(windowSize);
            ImGui::SetWindowPos(windowPos);

			GUI::displayScoreInCell(_physicalDeviceInfo->properties.deviceName, 68.7f, 0.7f);

			ImGui::BeginChild("Score list");
			for (int i = 0; i < 10; i++) {
				ImGui::PushID(i);
				GUI::displayScoreInCell(_physicalDeviceInfo->properties.deviceName, 68.7f, 1.0f - static_cast<float>(i) / 10.0f);
				ImGui::PopID();
			}
			ImGui::NewLine();
			if (ImGui::Button("< RETURN")) {
				display_result_screen = !display_result_screen;
			}
			ImGui::EndChild();
        }
        ImGui::End();
    }
    return true;
}

