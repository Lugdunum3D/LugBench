#include "GUI.hpp"
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Graphics/Vulkan/Render/Texture.hpp>

#include <IconsFontAwesome.h>

#include <fstream>
#if defined(LUG_SYSTEM_ANDROID)
    #include <android/asset_manager.h>
    #include <lug/Window/Android/WindowImplAndroid.hpp>
#endif

static bool displayExtraDeviceInfo = false;
static bool displayFeatures = false;

void GUI::displayConfigInfoString(const char* title, const char* content, const ImVec4 color) {
    ImGui::BeginGroup();
    {
        ImGui::TextColored(color, "%s:", title);
        ImGui::SameLine();
        ImGui::Text("%s", content);
    }
    ImGui::EndGroup();
}

void GUI::displayConfigInfoVersion(const char* title, const lug::Core::Version& version, float windowWidth) {
    ImGui::Text("%s", title);
    if (windowWidth != 0.f) {
        ImGui::SameLine(windowWidth);
    }
    else {
        ImGui::SameLine();
    }
    ImGui::Text("%d.%d.%d", version.major, version.minor, version.patch);
}

void GUI::displayConfigInfoValue(const char* title, const int value, const ImVec4 color) {
    ImGui::TextColored(color, "%s:", title);
    ImGui::SameLine();
    ImGui::Text("%d", value);
}

void GUI::displayConfigInfoFloatValue(const char* title, const float value, const ImVec4 color) {
    ImGui::TextColored(color, "%s:", title);
    ImGui::SameLine();
    ImGui::Text("%.3f", value);
}

void GUI::displayConfigInfoUnsignedLongValue(const char* title, const uint64_t value, const ImVec4 /*color*/) {
    ImGui::Text("%s:", title);
    ImGui::SameLine();
    ImGui::Text("%lu", value);
}

void GUI::displayConfigInfoBool(const char* title, const bool isTrue, float windowWidth)
{
    ImGui::Text("%s", title);
    if (windowWidth != 0.f) {
        ImGui::SameLine(windowWidth);
    } else {
        ImGui::SameLine();
    }
    if (isTrue == true) { ImGui::TextColored(ImVec4(0, 1, 0, 1), ICON_FA_CHECK); }
    else { ImGui::TextColored(ImVec4(1, 0, 0, 1), ICON_FA_TIMES); }
}

void GUI::displayConfigInfoArrayUint8(const char* title, const std::vector<uint8_t>& dataArray) {
    if (ImGui::CollapsingHeader(title)) {
        ImGui::Indent();
        {
            for (auto element : dataArray) {
                ImGui::Text("%d", element);
            }
        }
        ImGui::Unindent();
    }
}

void GUI::displayConfigInfoArrayUint32(const char* title, const std::vector<uint32_t>& dataArray) {
    if (ImGui::CollapsingHeader(title)) {
        ImGui::Indent();
        {
            for (auto element : dataArray) {
                ImGui::Text("%d", element);
            }
        }
        ImGui::Unindent();
    }
}

void GUI::displayConfigInfoArrayFloat(const char* title, const std::vector<float>& dataArray) {
    if (ImGui::CollapsingHeader(title)) {
        ImGui::Indent();
        {
            for (auto element : dataArray) {
                ImGui::Text("%f.3", element);
            }
        }
        ImGui::Unindent();
    }
}

void GUI::displayConfigInfoArrayStr(const char* title, const std::vector<const char *>& dataArray) {
    ImGui::Text("%s", title);
    ImGui::Indent();
    {
        for (auto element : dataArray) {
        ImGui::Text("%s", element);
        }
    }
    ImGui::Unindent();
}

void GUI::displayScoreInCell(const char * deviceName, const int score, const float progressbarValue) {

    ImGui::BeginGroup();
    ImGui::Text("Your GPU: %s", deviceName);
    ImGui::Text("Your averge frames per seconds: %d", score);


    ImGui::ProgressBar(progressbarValue, ImVec2(-1, 50.0F));
    ImGui::Separator();
    ImGui::EndGroup();
}

void GUI::displayInfoScreen(bool* isOpen, ImGuiWindowFlags windowFlags, lug::Graphics::Render::Window* window, lug::Graphics::Vulkan::PhysicalDeviceInfo* physicalDeviceInfo) {
    *isOpen = true;

    // Sets the window to be at the bottom of the screen (1/3rd of the height)
    ImVec2 windowSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) };
    ImVec2 windowPos = { 0, 0 };

    ImGui::Begin("Info Display", isOpen, windowFlags);
    {
        ImGui::SetWindowSize(windowSize);
        ImGui::SetWindowPos(windowPos);
        windowSize.x -= 12;
        windowSize.y -= 150;

        ImGui::BeginChild("Info", windowSize);
        {
            ImGui::SetWindowFontScale(.67f);
            ImGui::Indent();
            {
                ImGui::NewLine();
                ImGui::BeginChild("Device Hidden", { 270, 50 });
                {
                    ImGui::Text(ICON_FA_COGS " DEVICE");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(230);
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
                    ImGui::Separator();
                }
                ImGui::EndChild(); // Device Hidden
                GUI::displayConfigInfoString("Name", physicalDeviceInfo->properties.deviceName);
                GUI::displayConfigInfoVersion("Driver Version", lug::Core::Version::fromInt(physicalDeviceInfo->properties.driverVersion));
                GUI::displayConfigInfoValue("API Version", lug::Core::Version::fromInt(physicalDeviceInfo->properties.apiVersion));
                if (displayExtraDeviceInfo == true) {
                    ImGui::Indent();
                    {
                        GUI::displayConfigInfoValue("Device ID", physicalDeviceInfo->properties.deviceID);
                        GUI::displayConfigInfoValue("Vendor ID", physicalDeviceInfo->properties.vendorID);
                        GUI::displayConfigInfoString("Device Type", lug::Graphics::Vulkan::API::RTTI::toStr(physicalDeviceInfo->properties.deviceType));
                        GUI::displayConfigInfoArrayUint8("Pipeline Cache UUID", std::vector<uint8_t>(std::begin(physicalDeviceInfo->properties.pipelineCacheUUID), std::end(physicalDeviceInfo->properties.pipelineCacheUUID)));
                        if (ImGui::CollapsingHeader("Limits")) {
                            ImGui::Indent();
                            {
                                GUI::displayDeviceLimits(physicalDeviceInfo);
                            }
                            ImGui::Unindent();
                        }
                        if (ImGui::CollapsingHeader("Sparse Properties")) {
                            ImGui::Indent();
                            {
                                GUI::displayConfigInfoBool("Residency Standard 2D Block Shape", physicalDeviceInfo->properties.sparseProperties.residencyStandard2DBlockShape);
                                GUI::displayConfigInfoBool("Residency Standard 2D Multisample Block Shape", physicalDeviceInfo->properties.sparseProperties.residencyStandard2DMultisampleBlockShape);
                                GUI::displayConfigInfoBool("Residency Standard 3D Block Shape", physicalDeviceInfo->properties.sparseProperties.residencyStandard3DBlockShape);
                                GUI::displayConfigInfoBool("Residency Aligned Mip Size", physicalDeviceInfo->properties.sparseProperties.residencyAlignedMipSize);
                                GUI::displayConfigInfoBool("Residency Non Resident Strict", physicalDeviceInfo->properties.sparseProperties.residencyNonResidentStrict);

                            }
                            ImGui::Unindent();
                        }
                    }
                    ImGui::Unindent();
                }
                ImGui::BeginChild("Features Hidden", { 270, 50 });
                {
                    ImGui::Text(ICON_FA_WRENCH " FEATURES");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(230);
                    if (displayFeatures == false) {
                        if (ImGui::Button(ICON_FA_PLUS_SQUARE)) {
                            displayFeatures = true;
                        }
                    }
                    else {
                        if (ImGui::Button(ICON_FA_MINUS_SQUARE)) {
                            displayFeatures = false;
                        }
                    }
                    ImGui::Separator();
                }
                ImGui::EndChild(); // Features Hidden
                if (displayFeatures == true) {
                    ImGui::Indent();
                    {
//                        GUI::displayDeviceFeatures(physicalDeviceInfo);
                    }
                    ImGui::Unindent();
                }

                if (ImGui::CollapsingHeader("Memory")) {
                    ImGui::Indent();
                    {
                        GUI::displayConfigInfoUnsignedLongValue("Memory Type Count", physicalDeviceInfo->memoryProperties.memoryTypeCount);
                        GUI::displayConfigInfoUnsignedLongValue("Memory Heap Count", physicalDeviceInfo->memoryProperties.memoryHeapCount);

                        if (ImGui::CollapsingHeader("Memory Types")) {
                            for (size_t i = 0; i < physicalDeviceInfo->memoryProperties.memoryTypeCount; ++i) {
                                ImGui::TextColored(ImVec4(.64f, .87f, .29f, 1.f), "Type %lu", i);
                                ImGui::Indent();
                                {
                                    std::vector<const char*> propertyFlags = lug::Graphics::Vulkan::API::RTTI::VkMemoryPropertyFlagsToStrVec(physicalDeviceInfo->memoryProperties.memoryTypes[i].propertyFlags);
                                    if (propertyFlags.size() > 0) {
                                        ImGui::PushID(static_cast<int>(i));
                                        {
                                            GUI::displayConfigInfoArrayStr("Property Flags", propertyFlags);
                                        }
                                        ImGui::PopID();
                                    }
                                    GUI::displayConfigInfoUnsignedLongValue("Heap Index", physicalDeviceInfo->memoryProperties.memoryTypes[i].heapIndex);
                                }
                                ImGui::Unindent();
                            }
                        }

                        if (ImGui::CollapsingHeader("Memory Heaps")) {
                            for (size_t i = 0; i < physicalDeviceInfo->memoryProperties.memoryHeapCount; ++i) {
                                ImGui::TextColored(ImVec4(0, 255, 0, 255), "Heap %lu", i);
                                ImGui::Indent();
                                {
                                    GUI::displayConfigInfoUnsignedLongValue("Size", physicalDeviceInfo->memoryProperties.memoryHeaps[i].size);
                                    std::vector<const char*> flags = lug::Graphics::Vulkan::API::RTTI::VkMemoryHeapFlagsToStrVec(physicalDeviceInfo->memoryProperties.memoryHeaps[i].flags);
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
                        for (uint32_t i = 0; i < physicalDeviceInfo->queueFamilies.size(); ++i) {
                            ImGui::PushID(i);
                            {
                                ImGui::TextColored(ImVec4(0, 255, 0, 255), "Queue %d", i);
                                ImGui::Indent();
                                {
                                    VkQueueFamilyProperties queueFamilyProperties = physicalDeviceInfo->queueFamilies[i];
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
                    {
                        for (auto extension : physicalDeviceInfo->extensions) {
                            GUI::displayConfigInfoVersion(extension.extensionName, lug::Core::Version::fromInt(extension.specVersion));
                        }
                    }
                    ImGui::Unindent();
                }

                if (ImGui::CollapsingHeader("Formats")) {
                    ImGui::Indent();
                    {
                        for (auto format : physicalDeviceInfo->formatProperties) {
                            if (ImGui::CollapsingHeader(lug::Graphics::Vulkan::API::RTTI::toStr(format.first))) {
                                ImGui::Indent();
                                GUI::displayConfigInfoArrayStr("Linear Tiling Features", lug::Graphics::Vulkan::API::RTTI::VkFormatFeatureFlagsToStrVec(format.second.linearTilingFeatures));
                                GUI::displayConfigInfoArrayStr("Optimal Tiling Features", lug::Graphics::Vulkan::API::RTTI::VkFormatFeatureFlagsToStrVec(format.second.optimalTilingFeatures));
                                GUI::displayConfigInfoArrayStr("Buffer Features", lug::Graphics::Vulkan::API::RTTI::VkFormatFeatureFlagsToStrVec(format.second.bufferFeatures));
                                ImGui::Unindent();
                            }
                        }
                    }
                    ImGui::Unindent();
                }

                if (ImGui::CollapsingHeader("Swapchain")) {
                    ImGui::Indent();
                    {
                        GUI::displayConfigInfoUnsignedLongValue("Min Image Count", physicalDeviceInfo->swapchain.capabilities.minImageCount);
                        GUI::displayConfigInfoUnsignedLongValue("Max Image Count", physicalDeviceInfo->swapchain.capabilities.maxImageCount);
                        if (ImGui::CollapsingHeader("Current Extent", ImGuiTreeNodeFlags_NoAutoOpenOnLog)) {
                            GUI::displayConfigInfoUnsignedLongValue("Width", physicalDeviceInfo->swapchain.capabilities.currentExtent.width);
                            GUI::displayConfigInfoUnsignedLongValue("Height", physicalDeviceInfo->swapchain.capabilities.currentExtent.height);
                        }
                        if (ImGui::CollapsingHeader("Min Image Extent", ImGuiTreeNodeFlags_NoAutoOpenOnLog)) {
                            GUI::displayConfigInfoUnsignedLongValue("Width", physicalDeviceInfo->swapchain.capabilities.minImageExtent.width);
                            GUI::displayConfigInfoUnsignedLongValue("Height", physicalDeviceInfo->swapchain.capabilities.minImageExtent.height);
                        }
                        if (ImGui::CollapsingHeader("Max Image Extent", ImGuiTreeNodeFlags_NoAutoOpenOnLog)) {
                            GUI::displayConfigInfoUnsignedLongValue("Width", physicalDeviceInfo->swapchain.capabilities.maxImageExtent.width);
                            GUI::displayConfigInfoUnsignedLongValue("Height", physicalDeviceInfo->swapchain.capabilities.maxImageExtent.height);
                        }
                        GUI::displayConfigInfoUnsignedLongValue("Max Image Array Layers", physicalDeviceInfo->swapchain.capabilities.maxImageArrayLayers);
                    }
                    ImGui::Unindent();
                }

            }
            ImGui::Unindent();
        }
        ImGui::EndChild(); // Info
        if (GUI::displayReturnButton() == true) {
            *isOpen = false;
        }
    }
    ImGui::End(); // Info Display
}

void GUI::displayResultScreen(bool * isOpen, ImGuiWindowFlags windowFlags, lug::Graphics::Render::Window* window, lug::Graphics::Vulkan::PhysicalDeviceInfo* physicalDeviceInfo, nlohmann::json* devices) {
    *isOpen = true;
    ImGui::Begin("Result Display", isOpen, windowFlags);
    {
        // Sets the window to be at the bottom of the screen (1/3rd of the height)
        ImVec2 windowSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) };
        ImVec2 windowPos = { 0, 0 };
        ImGui::SetWindowSize(windowSize);
        ImGui::SetWindowPos(windowPos);
        windowSize.x -= 12;
        windowSize.y -= 150;
        ImGui::BeginChild("Result", windowSize);
        {
            ImGui::SetWindowFontScale(.67f);
            size_t deviceCount = devices->size();
            if (deviceCount > 0) {
                //			GUI::displayScoreInCell(_physicalDeviceInfo->properties.deviceName, 68.7f, 0.7f);
                nlohmann::json highestScoreDevice = (*devices)[0];
                float bigestStore = highestScoreDevice["averageFps"].get<float>();
                GUI::displayScoreInCell(physicalDeviceInfo->properties.deviceName, 68, 0.0f);

                ImGui::BeginChild("Score list");
                {
                    ImGui::SetWindowFontScale(.54f);
                    for (uint32_t i = 0; i < deviceCount; i++) {
                        ImGui::PushID(i);
                        nlohmann::json device = (*devices)[i];
                        GUI::displayScoreInCell((*devices)[i]["device"].get<std::string>().c_str(), 68, bigestStore / (*devices)[i]["averageFps"].get<float>());
                        //				GUI::displayScoreInCell(_physicalDeviceInfo->properties.deviceName, 68.7f, 1.0f - static_cast<float>(i) / 10.0f);
                        ImGui::PopID();
                    }
                }
                ImGui::EndChild();
            }
        }
        ImGui::EndChild();
        if (GUI::displayReturnButton() == true) {
            *isOpen = false;
        }
    }
    ImGui::End();
}

void GUI::displayDeviceLimits(lug::Graphics::Vulkan::PhysicalDeviceInfo * physicalDeviceInfo)
{
    GUI::displayConfigInfoValue("Max Image Dimension 1D", physicalDeviceInfo->properties.limits.maxImageDimension1D);
    GUI::displayConfigInfoValue("Max Image Dimension 2D", physicalDeviceInfo->properties.limits.maxImageDimension2D);
    GUI::displayConfigInfoValue("Max Image Dimension 3D", physicalDeviceInfo->properties.limits.maxImageDimension3D);
    GUI::displayConfigInfoValue("Max Image Dimension Cube", physicalDeviceInfo->properties.limits.maxImageDimensionCube);
    GUI::displayConfigInfoValue("Max Image Array Layers", physicalDeviceInfo->properties.limits.maxImageArrayLayers);
    GUI::displayConfigInfoValue("Max Texel Buffer Elements", physicalDeviceInfo->properties.limits.maxTexelBufferElements);
    GUI::displayConfigInfoValue("Max Uniform Buffer Range", physicalDeviceInfo->properties.limits.maxUniformBufferRange);
    GUI::displayConfigInfoValue("Max Storage Buffer Range", physicalDeviceInfo->properties.limits.maxStorageBufferRange);
    GUI::displayConfigInfoValue("Max Push Constants Size", physicalDeviceInfo->properties.limits.maxPushConstantsSize);
    GUI::displayConfigInfoValue("Max Memory Allocation Count", physicalDeviceInfo->properties.limits.maxMemoryAllocationCount);
    GUI::displayConfigInfoValue("Max Sampler Allocation Count", physicalDeviceInfo->properties.limits.maxSamplerAllocationCount);
    GUI::displayConfigInfoUnsignedLongValue("Buffer Image Granularity", physicalDeviceInfo->properties.limits.bufferImageGranularity);
    GUI::displayConfigInfoUnsignedLongValue("Sparse Address Space Size", physicalDeviceInfo->properties.limits.sparseAddressSpaceSize);
    GUI::displayConfigInfoValue("Max Bound Descriptor Sets", physicalDeviceInfo->properties.limits.maxBoundDescriptorSets);
    GUI::displayConfigInfoValue("Max Per Stage Descriptor Samplers", physicalDeviceInfo->properties.limits.maxPerStageDescriptorSamplers);
    GUI::displayConfigInfoValue("Max Per Stage Descriptor Uniform Buffers", physicalDeviceInfo->properties.limits.maxPerStageDescriptorUniformBuffers);
    GUI::displayConfigInfoValue("Max Per Stage Descriptor Storage Buffers", physicalDeviceInfo->properties.limits.maxPerStageDescriptorStorageBuffers);
    GUI::displayConfigInfoValue("Max Per Stage Descriptor Sampled Images", physicalDeviceInfo->properties.limits.maxPerStageDescriptorSampledImages);
    GUI::displayConfigInfoValue("Max Per Stage Descriptor Storage Images", physicalDeviceInfo->properties.limits.maxPerStageDescriptorStorageImages);
    GUI::displayConfigInfoValue("Max Per Stage Descriptor Input Attachments", physicalDeviceInfo->properties.limits.maxPerStageDescriptorInputAttachments);
    GUI::displayConfigInfoValue("Max Per Stage Resources", physicalDeviceInfo->properties.limits.maxPerStageResources);
    GUI::displayConfigInfoValue("Max Descriptor Set Samplers", physicalDeviceInfo->properties.limits.maxDescriptorSetSamplers);
    GUI::displayConfigInfoValue("Max Descriptor Set Uniform Buffers", physicalDeviceInfo->properties.limits.maxDescriptorSetUniformBuffers);
    GUI::displayConfigInfoValue("Max Descriptor Set Uniform Buffers Dynamic", physicalDeviceInfo->properties.limits.maxDescriptorSetUniformBuffersDynamic);
    GUI::displayConfigInfoValue("Max Descriptor Set Storage Buffers", physicalDeviceInfo->properties.limits.maxDescriptorSetStorageBuffers);
    GUI::displayConfigInfoValue("Max Descriptor Set Storage Buffers Dynamic", physicalDeviceInfo->properties.limits.maxDescriptorSetStorageBuffersDynamic);
    GUI::displayConfigInfoValue("Max Descriptor Set Sampled Images", physicalDeviceInfo->properties.limits.maxDescriptorSetSampledImages);
    GUI::displayConfigInfoValue("Max Descriptor Set Storage Images", physicalDeviceInfo->properties.limits.maxDescriptorSetStorageImages);
    GUI::displayConfigInfoValue("Max Descriptor Set Input Attachments", physicalDeviceInfo->properties.limits.maxDescriptorSetInputAttachments);
    GUI::displayConfigInfoValue("Max Vertex Input Attributes", physicalDeviceInfo->properties.limits.maxVertexInputAttributes);
    GUI::displayConfigInfoValue("Max Vertex Input Bindings", physicalDeviceInfo->properties.limits.maxVertexInputBindings);
    GUI::displayConfigInfoValue("Max Vertex Input AttributeOffset", physicalDeviceInfo->properties.limits.maxVertexInputAttributeOffset);
    GUI::displayConfigInfoValue("Max Vertex Input BindingStride", physicalDeviceInfo->properties.limits.maxVertexInputBindingStride);
    GUI::displayConfigInfoValue("Max Vertex Output Components", physicalDeviceInfo->properties.limits.maxVertexOutputComponents);
    GUI::displayConfigInfoValue("Max Tessellation GenerationLevel", physicalDeviceInfo->properties.limits.maxTessellationGenerationLevel);
    GUI::displayConfigInfoValue("Max Tessellation PatchSize", physicalDeviceInfo->properties.limits.maxTessellationPatchSize);
    GUI::displayConfigInfoValue("Max Tessellation Control Per Vertex Input Components", physicalDeviceInfo->properties.limits.maxTessellationControlPerVertexInputComponents);
    GUI::displayConfigInfoValue("Max Tessellation Control Per Vertex Output Components", physicalDeviceInfo->properties.limits.maxTessellationControlPerVertexOutputComponents);
    GUI::displayConfigInfoValue("Max Tessellation Control Per Patch Output Components", physicalDeviceInfo->properties.limits.maxTessellationControlPerPatchOutputComponents);
    GUI::displayConfigInfoValue("Max Tessellation Control Total Output Components", physicalDeviceInfo->properties.limits.maxTessellationControlTotalOutputComponents);
    GUI::displayConfigInfoValue("Max Tessellation Evaluation Input Components", physicalDeviceInfo->properties.limits.maxTessellationEvaluationInputComponents);
    GUI::displayConfigInfoValue("Max Tessellation Evaluation Output Components", physicalDeviceInfo->properties.limits.maxTessellationEvaluationOutputComponents);
    GUI::displayConfigInfoValue("Max Geometry Shader Invocations", physicalDeviceInfo->properties.limits.maxGeometryShaderInvocations);
    GUI::displayConfigInfoValue("Max Geometry Input Components", physicalDeviceInfo->properties.limits.maxGeometryInputComponents);
    GUI::displayConfigInfoValue("Max Geometry Output Components", physicalDeviceInfo->properties.limits.maxGeometryOutputComponents);
    GUI::displayConfigInfoValue("Max Geometry Output Vertices", physicalDeviceInfo->properties.limits.maxGeometryOutputVertices);
    GUI::displayConfigInfoValue("Max Geometry Total Output Components", physicalDeviceInfo->properties.limits.maxGeometryTotalOutputComponents);
    GUI::displayConfigInfoValue("Max Fragment Input Components", physicalDeviceInfo->properties.limits.maxFragmentInputComponents);
    GUI::displayConfigInfoValue("Max Fragment Output Attachments", physicalDeviceInfo->properties.limits.maxFragmentOutputAttachments);
    GUI::displayConfigInfoValue("Max Fragment Dual SrcAttachments", physicalDeviceInfo->properties.limits.maxFragmentDualSrcAttachments);
    GUI::displayConfigInfoValue("Max Fragment Combined Output Resources", physicalDeviceInfo->properties.limits.maxFragmentCombinedOutputResources);
    GUI::displayConfigInfoValue("Max Compute Shared Memory Size", physicalDeviceInfo->properties.limits.maxComputeSharedMemorySize);
    GUI::displayConfigInfoArrayUint32("Max Compute Work Group Count", std::vector<uint32_t>(std::begin(physicalDeviceInfo->properties.limits.maxComputeWorkGroupCount), std::end(physicalDeviceInfo->properties.limits.maxComputeWorkGroupCount)));
    GUI::displayConfigInfoValue("Max Compute Work Group Invocations", physicalDeviceInfo->properties.limits.maxComputeWorkGroupInvocations);
    GUI::displayConfigInfoArrayUint32("Max Compute Work Group Size", std::vector<uint32_t>(std::begin(physicalDeviceInfo->properties.limits.maxComputeWorkGroupSize), std::end(physicalDeviceInfo->properties.limits.maxComputeWorkGroupSize)));
    GUI::displayConfigInfoValue("Sub Pixel Precision Bits", physicalDeviceInfo->properties.limits.subPixelPrecisionBits);
    GUI::displayConfigInfoValue("Sub Texel Precision Bits", physicalDeviceInfo->properties.limits.subTexelPrecisionBits);
    GUI::displayConfigInfoValue("Mipmap Precision Bits", physicalDeviceInfo->properties.limits.mipmapPrecisionBits);
    GUI::displayConfigInfoValue("Max Draw Indexed Index Value", physicalDeviceInfo->properties.limits.maxDrawIndexedIndexValue);
    GUI::displayConfigInfoValue("Max Draw Indirect Count", physicalDeviceInfo->properties.limits.maxDrawIndirectCount);
    GUI::displayConfigInfoFloatValue("Max Sampler Lod Bias", physicalDeviceInfo->properties.limits.maxSamplerLodBias);
    GUI::displayConfigInfoFloatValue("Max Sampler Anisotropy", physicalDeviceInfo->properties.limits.maxSamplerAnisotropy);
    GUI::displayConfigInfoValue("Max Viewports", physicalDeviceInfo->properties.limits.maxViewports);
    GUI::displayConfigInfoArrayUint32("Max Viewport Dimensions", std::vector<uint32_t>(std::begin(physicalDeviceInfo->properties.limits.maxViewportDimensions), std::end(physicalDeviceInfo->properties.limits.maxViewportDimensions)));
    GUI::displayConfigInfoArrayFloat("Viewport Bounds Range", std::vector<float>(std::begin(physicalDeviceInfo->properties.limits.viewportBoundsRange), std::end(physicalDeviceInfo->properties.limits.viewportBoundsRange)));
    GUI::displayConfigInfoValue("Viewport Sub Pixel Bits", physicalDeviceInfo->properties.limits.viewportSubPixelBits);
    GUI::displayConfigInfoUnsignedLongValue("Min Memory Map Alignment", physicalDeviceInfo->properties.limits.minMemoryMapAlignment);
    GUI::displayConfigInfoUnsignedLongValue("Min Texel Buffer Offset Alignment", physicalDeviceInfo->properties.limits.minTexelBufferOffsetAlignment);
    GUI::displayConfigInfoUnsignedLongValue("Min Uniform Buffer Offset Alignment", physicalDeviceInfo->properties.limits.minUniformBufferOffsetAlignment);
    GUI::displayConfigInfoUnsignedLongValue("Min Storage Buffer Offset Alignment", physicalDeviceInfo->properties.limits.minStorageBufferOffsetAlignment);
    GUI::displayConfigInfoValue("Min Texel Offset", physicalDeviceInfo->properties.limits.minTexelOffset);
    GUI::displayConfigInfoValue("Max Texel Offset", physicalDeviceInfo->properties.limits.maxTexelOffset);
    GUI::displayConfigInfoValue("Min Texel Gather Offset", physicalDeviceInfo->properties.limits.minTexelGatherOffset);
    GUI::displayConfigInfoValue("Max Texel Gather Offset", physicalDeviceInfo->properties.limits.maxTexelGatherOffset);
    GUI::displayConfigInfoFloatValue("Min Interpolation Offset", physicalDeviceInfo->properties.limits.minInterpolationOffset);
    GUI::displayConfigInfoFloatValue("Max InterpolationOffset", physicalDeviceInfo->properties.limits.maxInterpolationOffset);
    GUI::displayConfigInfoValue("Sub Pixel Interpolation Offset Bits", physicalDeviceInfo->properties.limits.subPixelInterpolationOffsetBits);
    GUI::displayConfigInfoValue("Max FramebufferWidth", physicalDeviceInfo->properties.limits.maxFramebufferWidth);
    GUI::displayConfigInfoValue("Max FramebufferHeight", physicalDeviceInfo->properties.limits.maxFramebufferHeight);
    GUI::displayConfigInfoValue("Max FramebufferLayers", physicalDeviceInfo->properties.limits.maxFramebufferLayers);
    GUI::displayConfigInfoValue("Framebuffer Color Sample Counts", physicalDeviceInfo->properties.limits.framebufferColorSampleCounts);
    GUI::displayConfigInfoValue("Framebuffer Depth Sample Counts", physicalDeviceInfo->properties.limits.framebufferDepthSampleCounts);
    GUI::displayConfigInfoValue("Framebuffer Stencil Sample Counts", physicalDeviceInfo->properties.limits.framebufferStencilSampleCounts);
    GUI::displayConfigInfoValue("Framebuffer No Attachments Sample Counts", physicalDeviceInfo->properties.limits.framebufferNoAttachmentsSampleCounts);
    GUI::displayConfigInfoValue("Max Color Attachments", physicalDeviceInfo->properties.limits.maxColorAttachments);
    GUI::displayConfigInfoValue("Sampled Image Color Sample Counts", physicalDeviceInfo->properties.limits.sampledImageColorSampleCounts);
    GUI::displayConfigInfoValue("Sampled Image Integer Sample Counts", physicalDeviceInfo->properties.limits.sampledImageIntegerSampleCounts);
    GUI::displayConfigInfoValue("Sampled Image Depth Sample Counts", physicalDeviceInfo->properties.limits.sampledImageDepthSampleCounts);
    GUI::displayConfigInfoValue("Sampled Image Stencil Sample Counts", physicalDeviceInfo->properties.limits.sampledImageStencilSampleCounts);
    GUI::displayConfigInfoValue("Storage Image Sample Counts", physicalDeviceInfo->properties.limits.storageImageSampleCounts);
    GUI::displayConfigInfoValue("Max SampleMaskWords", physicalDeviceInfo->properties.limits.maxSampleMaskWords);
    GUI::displayConfigInfoValue("Timestamp Compute And Graphics", physicalDeviceInfo->properties.limits.timestampComputeAndGraphics);
    GUI::displayConfigInfoFloatValue("Timestamp Period", physicalDeviceInfo->properties.limits.timestampPeriod);
    GUI::displayConfigInfoValue("Max Clip Distances", physicalDeviceInfo->properties.limits.maxClipDistances);
    GUI::displayConfigInfoValue("Max Cull Distances", physicalDeviceInfo->properties.limits.maxCullDistances);
    GUI::displayConfigInfoValue("Max Combined Clip And Cull Distances", physicalDeviceInfo->properties.limits.maxCombinedClipAndCullDistances);
    GUI::displayConfigInfoValue("Discrete Queue Priorities", physicalDeviceInfo->properties.limits.discreteQueuePriorities);
    GUI::displayConfigInfoArrayFloat("Point Size Range", std::vector<float>(std::begin(physicalDeviceInfo->properties.limits.pointSizeRange), std::end(physicalDeviceInfo->properties.limits.pointSizeRange)));
    GUI::displayConfigInfoArrayFloat("Line Width Range", std::vector<float>(std::begin(physicalDeviceInfo->properties.limits.lineWidthRange), std::end(physicalDeviceInfo->properties.limits.lineWidthRange)));
    GUI::displayConfigInfoFloatValue("Point Size Granularity", physicalDeviceInfo->properties.limits.pointSizeGranularity);
    GUI::displayConfigInfoFloatValue("Line Width Granularity", physicalDeviceInfo->properties.limits.lineWidthGranularity);
    GUI::displayConfigInfoValue("Strict Lines", physicalDeviceInfo->properties.limits.strictLines);
    GUI::displayConfigInfoValue("Standard Sample Locations", physicalDeviceInfo->properties.limits.standardSampleLocations);
    GUI::displayConfigInfoUnsignedLongValue("Optimal Buffer Copy Offset Alignment", physicalDeviceInfo->properties.limits.optimalBufferCopyOffsetAlignment);
    GUI::displayConfigInfoUnsignedLongValue("Optimal Buffer Copy Row Pitch Alignment", physicalDeviceInfo->properties.limits.optimalBufferCopyRowPitchAlignment);
    GUI::displayConfigInfoUnsignedLongValue("Non Coherent AtomSize", physicalDeviceInfo->properties.limits.nonCoherentAtomSize);
}

void GUI::displayDeviceFeatures(lug::Graphics::Vulkan::PhysicalDeviceInfo * physicalDeviceInfo, float windowWith)
{
    GUI::displayConfigInfoBool("Robust Buffer Access", physicalDeviceInfo->features.robustBufferAccess, windowWith);
    GUI::displayConfigInfoBool("Full Draw Index Uint32", physicalDeviceInfo->features.fullDrawIndexUint32, windowWith);
    GUI::displayConfigInfoBool("Image Cube Array", physicalDeviceInfo->features.imageCubeArray, windowWith);
    GUI::displayConfigInfoBool("Independent Blend", physicalDeviceInfo->features.independentBlend, windowWith);
    GUI::displayConfigInfoBool("Geometry Shader", physicalDeviceInfo->features.geometryShader, windowWith);
    GUI::displayConfigInfoBool("Tessellation Shader", physicalDeviceInfo->features.tessellationShader, windowWith);
    GUI::displayConfigInfoBool("Sample Rate Shading", physicalDeviceInfo->features.sampleRateShading, windowWith);
    GUI::displayConfigInfoBool("Dual Src Blend", physicalDeviceInfo->features.dualSrcBlend, windowWith);
    GUI::displayConfigInfoBool("LogicOp", physicalDeviceInfo->features.logicOp, windowWith);
    GUI::displayConfigInfoBool("Multi Draw Indirect", physicalDeviceInfo->features.multiDrawIndirect, windowWith);
    GUI::displayConfigInfoBool("Draw Indirect First Instance", physicalDeviceInfo->features.drawIndirectFirstInstance, windowWith);
    GUI::displayConfigInfoBool("Depth Clamp", physicalDeviceInfo->features.depthClamp, windowWith);
    GUI::displayConfigInfoBool("Depth Bias Clamp", physicalDeviceInfo->features.depthBiasClamp, windowWith);
    GUI::displayConfigInfoBool("Fill Mode Non Solid", physicalDeviceInfo->features.fillModeNonSolid, windowWith);
    GUI::displayConfigInfoBool("Depth Bounds", physicalDeviceInfo->features.depthBounds, windowWith);
    GUI::displayConfigInfoBool("Wide Lines", physicalDeviceInfo->features.wideLines, windowWith);
    GUI::displayConfigInfoBool("Large Points", physicalDeviceInfo->features.largePoints, windowWith);
    GUI::displayConfigInfoBool("Alpha To One", physicalDeviceInfo->features.alphaToOne, windowWith);
    GUI::displayConfigInfoBool("Multi Viewport", physicalDeviceInfo->features.multiViewport, windowWith);
    GUI::displayConfigInfoBool("Sampler Anisotropy", physicalDeviceInfo->features.samplerAnisotropy, windowWith);
    GUI::displayConfigInfoBool("Texture Compression ETC2", physicalDeviceInfo->features.textureCompressionETC2, windowWith);
    GUI::displayConfigInfoBool("Texture Compression ASTC_LDR", physicalDeviceInfo->features.textureCompressionASTC_LDR, windowWith);
    GUI::displayConfigInfoBool("Texture Compression BC", physicalDeviceInfo->features.textureCompressionBC, windowWith);
    GUI::displayConfigInfoBool("Occlusion Query Precise", physicalDeviceInfo->features.occlusionQueryPrecise, windowWith);
    GUI::displayConfigInfoBool("Pipeline Statistics Query", physicalDeviceInfo->features.pipelineStatisticsQuery, windowWith);
    GUI::displayConfigInfoBool("Vertex Pipeline Stores And Atomics", physicalDeviceInfo->features.vertexPipelineStoresAndAtomics, windowWith);
    GUI::displayConfigInfoBool("Fragment Stores And Atomics", physicalDeviceInfo->features.fragmentStoresAndAtomics, windowWith);
    GUI::displayConfigInfoBool("Shader Tessellation And Geometry Point Size", physicalDeviceInfo->features.shaderTessellationAndGeometryPointSize, windowWith);
    GUI::displayConfigInfoBool("Shader Image Gather Extended", physicalDeviceInfo->features.shaderImageGatherExtended, windowWith);
    GUI::displayConfigInfoBool("Shader Storage Image Extended Formats", physicalDeviceInfo->features.shaderStorageImageExtendedFormats, windowWith);
    GUI::displayConfigInfoBool("Shader Storage Image Multisample", physicalDeviceInfo->features.shaderStorageImageMultisample, windowWith);
    GUI::displayConfigInfoBool("Shader Storage Image Read Without Format", physicalDeviceInfo->features.shaderStorageImageReadWithoutFormat, windowWith);
    GUI::displayConfigInfoBool("Shader Storage Image Write Without Format", physicalDeviceInfo->features.shaderStorageImageWriteWithoutFormat, windowWith);
    GUI::displayConfigInfoBool("Shader Uniform Buffer Array Dynamic Indexing", physicalDeviceInfo->features.shaderUniformBufferArrayDynamicIndexing, windowWith);
    GUI::displayConfigInfoBool("Shader Sampled Image Array Dynamic Indexing", physicalDeviceInfo->features.shaderSampledImageArrayDynamicIndexing, windowWith);
    GUI::displayConfigInfoBool("Shader Storage Buffer Array Dynamic Indexing", physicalDeviceInfo->features.shaderStorageBufferArrayDynamicIndexing, windowWith);
    GUI::displayConfigInfoBool("Shader Storage Image Array Dynamic Indexing", physicalDeviceInfo->features.shaderStorageImageArrayDynamicIndexing, windowWith);
    GUI::displayConfigInfoBool("Shader Clip Distance", physicalDeviceInfo->features.shaderClipDistance, windowWith);
    GUI::displayConfigInfoBool("Shader Cull Distance", physicalDeviceInfo->features.shaderCullDistance, windowWith);
    GUI::displayConfigInfoBool("Shader Float64", physicalDeviceInfo->features.shaderFloat64, windowWith);
    GUI::displayConfigInfoBool("Shader Int64", physicalDeviceInfo->features.shaderInt64, windowWith);
    GUI::displayConfigInfoBool("Shader Int16", physicalDeviceInfo->features.shaderInt16, windowWith);
    GUI::displayConfigInfoBool("Shader ResourceResidency", physicalDeviceInfo->features.shaderResourceResidency, windowWith);
    GUI::displayConfigInfoBool("Shader ResourceMinLod", physicalDeviceInfo->features.shaderResourceMinLod, windowWith);
    GUI::displayConfigInfoBool("Sparse Binding", physicalDeviceInfo->features.sparseBinding, windowWith);
    GUI::displayConfigInfoBool("Sparse Residency Buffer", physicalDeviceInfo->features.sparseResidencyBuffer, windowWith);
    GUI::displayConfigInfoBool("Sparse Residency Image2D", physicalDeviceInfo->features.sparseResidencyImage2D, windowWith);
    GUI::displayConfigInfoBool("Sparse Residency Image3D", physicalDeviceInfo->features.sparseResidencyImage3D, windowWith);
    GUI::displayConfigInfoBool("Sparse Residency 2Samples", physicalDeviceInfo->features.sparseResidency2Samples, windowWith);
    GUI::displayConfigInfoBool("Sparse Residency 4Samples", physicalDeviceInfo->features.sparseResidency4Samples, windowWith);
    GUI::displayConfigInfoBool("Sparse Residency 8Samples", physicalDeviceInfo->features.sparseResidency8Samples, windowWith);
    GUI::displayConfigInfoBool("Sparse Residency 16Samples", physicalDeviceInfo->features.sparseResidency16Samples, windowWith);
    GUI::displayConfigInfoBool("Sparse Residency Aliased", physicalDeviceInfo->features.sparseResidencyAliased, windowWith);
    GUI::displayConfigInfoBool("Variable Multisample Rate", physicalDeviceInfo->features.variableMultisampleRate, windowWith);
    GUI::displayConfigInfoBool("Inherited Queries", physicalDeviceInfo->features.inheritedQueries, windowWith);
}

bool GUI::displayReturnButton() {
    ImGui::Separator();
    ImGui::SetWindowFontScale(.67f);
    ImGui::NewLine();
    if (ImGui::Button(ICON_FA_ARROW_LEFT " RETURN", ImVec2{ 120, 50 })) {
        return true;
    }
    return false;
}

void GUI::setDefaultStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.ChildWindowRounding = 0.f;
    style.WindowRounding = 0.f;
    style.FrameRounding = 0.f;
    style.WindowPadding.x = 0.f;
    style.WindowPadding.y = 0.f;

    style.Colors[ImGuiCol_Text]                 = V4_WHITE;
    style.Colors[ImGuiCol_WindowBg]             = V4_WHITE;
    style.Colors[ImGuiCol_ChildWindowBg]        = V4_SKYBLUE;
    style.Colors[ImGuiCol_Border]               = V4_DARKGRAY;
    style.Colors[ImGuiCol_ScrollbarBg]          = V4_WHITE;
    style.Colors[ImGuiCol_ScrollbarGrab]        = V4_LIGHTGRAY;
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = V4_GRAY;
    style.Colors[ImGuiCol_ScrollbarGrabActive]  = V4_GRAY;
    style.Colors[ImGuiCol_Button]               = V4_SKYBLUE;
    style.Colors[ImGuiCol_ButtonHovered]        = V4_DARKGRAY;
    style.Colors[ImGuiCol_ButtonActive]         = V4_DARKGRAY;
}

float GUI::displayMenu(LugBench::Application &application) {
    State currentState = application.getCurrentState();

    lug::Graphics::Render::Window* window = application.getGraphics().getRenderer()->getWindow();

    float mainMenuHeight = Utilities::getMainMenuHeight(window->getHeight());

    ImGui::Begin("Main Menu", 0, application._window_flags);
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
                    // This button does nothing but we use a button instead of a text so that it lines up perfectly
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, V4_SKYBLUE);
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, V4_SKYBLUE);
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

                    if (currentState == State::BENCHMARKS) {
                        ImGui::PushStyleColor(ImGuiCol_Text, V4_DARKGRAY);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, V4_SKYBLUE);
                    }
                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 150.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 150.f, headerSize.y };
#endif
                        ImGui::SameLine();
                        if (ImGui::Button("BENCHMARKS", buttonSize) && currentState != State::BENCHMARKS) {
                            std::shared_ptr<AState> benchmarksState;
                            benchmarksState = std::make_shared<BenchmarksState>(application);
                            application.popState();
                            application.pushState(benchmarksState);
                        }
                    }
                    if (currentState == State::BENCHMARKS) {
                        ImGui::PopStyleColor(2);
                    }
                    if (currentState == State::MODELS) {
                        ImGui::PushStyleColor(ImGuiCol_Text, V4_DARKGRAY);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, V4_SKYBLUE);
                    }
                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 100.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 100.f, headerSize.y };
#endif
                        ImGui::SameLine();
                        if (ImGui::Button("MODELS", buttonSize) && currentState != State::MODELS) {
                            std::shared_ptr<AState> modelsState;
                            modelsState = std::make_shared<ModelsState>(application);
                            application.popState();
                            application.pushState(modelsState);
                        }
                    }
                    if (currentState == State::MODELS) {
                        ImGui::PopStyleColor(2);
                    }


                    if (currentState == State::INFO) {
                        ImGui::PushStyleColor(ImGuiCol_Text, V4_DARKGRAY);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, V4_SKYBLUE);
                    }
                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 60.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 60.f, headerSize.y };
#endif
                        ImGui::SameLine();
                        if (ImGui::Button("INFO", buttonSize) && currentState != State::INFO) {
                            std::shared_ptr<AState> infoState;
                            infoState = std::make_shared<InfoState>(application);
                            application.popState();
                            application.pushState(infoState);
                        }
                    }
                    if (currentState == State::INFO) {
                        ImGui::PopStyleColor(2);
                    }

                    /*
                    if (currentState == State::RESULTS) {
                        ImGui::PushStyleColor(ImGuiCol_Text, V4_DARKGRAY);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, V4_SKYBLUE);
                    }
                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 110.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 110.f, headerSize.y };
#endif
                        ImGui::SameLine();
                        if (ImGui::Button("RESULTS", buttonSize) && currentState != State::RESULTS) {
                            std::shared_ptr<AState> resultState;
                            resultState = std::make_shared<ResultsState>(application);
                            application.popState();
                            application.pushState(resultState);
                        }
                    }
                    if (currentState == State::RESULTS) {
                        ImGui::PopStyleColor(2);
                    }
                    */
                    if (currentState == State::CONTACT) {
                        ImGui::PushStyleColor(ImGuiCol_Text, V4_DARKGRAY);
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, V4_SKYBLUE);
                    }
                    {
#if defined(LUG_SYSTEM_ANDROID)
                        ImVec2 buttonSize{ 110.f * 2.75f, headerSize.y };
#else
                        ImVec2 buttonSize{ 110.f, headerSize.y };
#endif
                        ImGui::SameLine();
                        if (ImGui::Button("CONTACT", buttonSize) && currentState != State::CONTACT) {
                            std::shared_ptr<AState> contactState;
                            contactState = std::make_shared<ContactState>(application);
                            application.popState();
                            application.pushState(contactState);
                        }
                    }
                    if (currentState == State::CONTACT) {
                        ImGui::PopStyleColor(2);
                    }
                }
                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
        ImGui::PopStyleVar();
    }
    ImGui::End();
    return mainMenuHeight;
}

float GUI::displayFooter(LugBench::Application& /*application*/)
{
    return 0;
    /*
    lug::Graphics::Render::Window* window = application.getGraphics().getRenderer()->getWindow();

    float footerHeight = Utilities::getFooterHeight(window->getHeight());

    ImGui::PushStyleColor(ImGuiCol_WindowBg, V4_GRAY);
    {
            ImGui::Begin("Footer", 0, application._window_flags);
            {
                ImVec2 footerSize{ static_cast<float>(window->getWidth()), footerHeight };
                ImVec2 footerPos = { 0, window->getHeight() - footerHeight };

                ImGui::SetWindowSize(footerSize);
                ImGui::SetWindowPos(footerPos);

                ImGui::SetCursorPos(ImVec2(5, 5));
                {
                    auto vkTexture = lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(application._epitechLogo);
#if defined(LUG_SYSTEM_ANDROID)
                    ImGui::Image(vkTexture.get(), ImVec2(193.f * 2, 70 * 2), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
#else
                    ImGui::Image(vkTexture.get(), ImVec2(193.f, 70), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
#endif
                }
                ImGui::SameLine();
#if defined(LUG_SYSTEM_ANDROID)
                float width = (float)(window->getWidth() - ((134.f * 2) + (261.f * 2) + 30));
#else
                float width = (float)(window->getWidth() - (134.f + 261.f + 15));
#endif
                ImGui::SetCursorPos(ImVec2(width, 5));
                {
                    auto vkTexture = lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(application._gltfLogo);
#if defined(LUG_SYSTEM_ANDROID)
                    ImGui::Image(vkTexture.get(), ImVec2(134.f * 2, 70 * 2), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
#else
                    ImGui::Image(vkTexture.get(), ImVec2(134.f, 70), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
#endif
                }
                ImGui::SameLine();
                {
                    auto vkTexture = lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(application._vulkanLogo);
#if defined(LUG_SYSTEM_ANDROID)
                    ImGui::Image(vkTexture.get(), ImVec2(261.f * 2, 70 * 2), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
#else
                    ImGui::Image(vkTexture.get(), ImVec2(261.f, 70), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
#endif
                }
            }
            ImGui::End();
    }
    ImGui::PopStyleColor();
    return footerHeight;
    */
}

float GUI::Utilities::getPercentage(float fullSize, float percentage, float minSize)
{
    float retVal;

    retVal = fullSize * percentage;
    if (retVal < minSize) { retVal = minSize; }

    return retVal;
}

float GUI::Utilities::getMainMenuHeight(float windowHeight) {
#if defined(LUG_SYSTEM_ANDROID)
    return Utilities::getPercentage(windowHeight, 0.06f, 120.f);
#else
    return Utilities::getPercentage(windowHeight, 0.06f, 60.f);
#endif
}

float GUI::Utilities::getFooterHeight(float windowHeight) {
#if defined(LUG_SYSTEM_ANDROID)
    return Utilities::getPercentage(windowHeight, 0.06f, 160.f);
#else
    return Utilities::getPercentage(windowHeight, 0.06f, 80.f);
#endif
}

std::string GUI::Utilities::ReadWholeFile(std::string filename) {
#if defined(LUG_SYSTEM_ANDROID)
    AAsset *asset = AAssetManager_open((lug::Window::priv::WindowImpl::activity)->assetManager,
                                       filename.c_str(), AASSET_MODE_STREAMING);

    if (!asset) {
        LUG_LOG.error("GUI::Utilities::ReadWholeFile Can't open Android asset \"{}\"",
                      "fonts/fontawesome-webfont.ttf");
        return "";
    }

    size_t size = AAsset_getLength(asset);

    if (size <= 0) {
        LUG_LOG.error("GUI::Utilities::ReadWholeFile Android asset \"{}\" is empty",
                      "fonts/fontawesome-webfont.ttf");
        return "";
    }
    char *buff(new char[size]);

    AAsset_read(asset, buff, size);
    AAsset_close(asset);
    std::string ret(buff);
    free(buff);
    return ret;
#else
    std::string resultBuffer = {};
    std::string line;
    std::ifstream myfile(filename);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            resultBuffer += line + '\n';
        }
        myfile.close();
    }
    return resultBuffer;
#endif
}
