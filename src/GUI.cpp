#include "GUI.hpp"
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>

void GUI::displayConfigInfoString(const char* title, const char* content, const ImVec4 color) {
    ImGui::TextColored(color, "%s", title);
    ImGui::Indent();
    {
        ImGui::Text("%s", content);
    }
    ImGui::Unindent();
}

void GUI::displayConfigInfoVersion(const char* title, const lug::Core::Version& version, const ImVec4 color) {
    ImGui::TextColored(color, "%s", title);
    ImGui::Indent();
    {
        ImGui::Text("%d.%d.%d", version.major, version.minor, version.patch);
    }
    ImGui::Unindent();
}

void GUI::displayConfigInfoValue(const char* title, const int value, const ImVec4 color) {
    ImGui::TextColored(color, "%s", title);
    ImGui::Indent();
    {
        ImGui::Text("%d", value);
    }
    ImGui::Unindent();
}

void GUI::displayConfigInfoFloatValue(const char* title, const float value, const ImVec4 color) {
    ImGui::TextColored(color, "%s", title);
    ImGui::Indent();
    {
        ImGui::Text("%.3f", value);
    }
    ImGui::Unindent();
}

void GUI::displayConfigInfoUnsignedLongValue(const char* title, const uint64_t value, const ImVec4 color) {
    ImGui::TextColored(color, "%s", title);
    ImGui::Indent();
    {
        ImGui::Text("%lu", value);
    }
    ImGui::Unindent();
}

void GUI::displayConfigInfoBool(const char* title, const bool isTrue, const ImVec4 color)
{
    ImGui::TextColored(color, "%s", title);
    ImGui::Indent();
    {
        if (isTrue == true) { ImGui::Text("True"); }
        else { ImGui::Text("False"); }
    }
    ImGui::Unindent();
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
    if (ImGui::CollapsingHeader(title)) {
        ImGui::Indent();
        {
            for (auto element : dataArray) {
                ImGui::Text("%s", element);
            }
        }
        ImGui::Unindent();
    }
}

void GUI::displayScoreInCell(const char * deviceName, const float score, const float progressbarValue) {

	ImGui::BeginGroup();
	ImGui::Text("Your GPU: %s",deviceName);
	ImGui::Value("Your score", score);


	ImGui::ProgressBar(progressbarValue, ImVec2(-1, 50.0F));
	ImGui::Separator();
	ImGui::EndGroup();
}

void GUI::displayInfoScreen(bool* isOpen, ImGuiWindowFlags windowFlags, lug::Graphics::Render::Window* window, lug::Graphics::Vulkan::PhysicalDeviceInfo* physicalDeviceInfo) {
    *isOpen = true;
    ImGui::Begin("Info Display", isOpen, windowFlags);
    {
        // Sets the window to be at the bottom of the screen (1/3rd of the height)
        ImVec2 windowSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) };
        ImVec2 windowPos = { 0, 0 };
        ImGui::SetWindowSize(windowSize);
        ImGui::SetWindowPos(windowPos);

        if (ImGui::CollapsingHeader("Device", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::Indent();
            {
                GUI::displayConfigInfoString("Name", physicalDeviceInfo->properties.deviceName);
                GUI::displayConfigInfoVersion("Driver Version", lug::Core::Version::fromInt(physicalDeviceInfo->properties.driverVersion));
                GUI::displayConfigInfoValue("API Version", lug::Core::Version::fromInt(physicalDeviceInfo->properties.apiVersion));
                if (ImGui::CollapsingHeader("Extra Info")) {
                    ImGui::Indent();
                    {
                        GUI::displayConfigInfoValue("Device ID", physicalDeviceInfo->properties.deviceID);
                        GUI::displayConfigInfoValue("Vendor ID", physicalDeviceInfo->properties.vendorID);
                        GUI::displayConfigInfoString("Device Type", lug::Graphics::Vulkan::API::RTTI::toStr(physicalDeviceInfo->properties.deviceType));
                        GUI::displayConfigInfoArrayUint8("Pipeline Cache UUID", std::vector<uint8_t>(std::begin(physicalDeviceInfo->properties.pipelineCacheUUID), std::end(physicalDeviceInfo->properties.pipelineCacheUUID)));
                        if (ImGui::CollapsingHeader("Limits")) {
                            ImGui::Indent();
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
                        ImGui::Unindent();
                    }
                    ImGui::Unindent();
                }
                if (ImGui::CollapsingHeader("Features")) {
                    ImGui::Indent();
                    {
                        GUI::displayConfigInfoBool("Robust Buffer Access", physicalDeviceInfo->features.robustBufferAccess);
                        GUI::displayConfigInfoBool("Full Draw Index Uint32", physicalDeviceInfo->features.fullDrawIndexUint32);
                        GUI::displayConfigInfoBool("Image Cube Array", physicalDeviceInfo->features.imageCubeArray);
                        GUI::displayConfigInfoBool("Independent Blend", physicalDeviceInfo->features.independentBlend);
                        GUI::displayConfigInfoBool("Geometry Shader", physicalDeviceInfo->features.geometryShader);
                        GUI::displayConfigInfoBool("Tessellation Shader", physicalDeviceInfo->features.tessellationShader);
                        GUI::displayConfigInfoBool("Sample Rate Shading", physicalDeviceInfo->features.sampleRateShading);
                        GUI::displayConfigInfoBool("Dual Src Blend", physicalDeviceInfo->features.dualSrcBlend);
                        GUI::displayConfigInfoBool("LogicOp", physicalDeviceInfo->features.logicOp);
                        GUI::displayConfigInfoBool("Multi Draw Indirect", physicalDeviceInfo->features.multiDrawIndirect);
                        GUI::displayConfigInfoBool("Draw Indirect First Instance", physicalDeviceInfo->features.drawIndirectFirstInstance);
                        GUI::displayConfigInfoBool("Depth Clamp", physicalDeviceInfo->features.depthClamp);
                        GUI::displayConfigInfoBool("Depth Bias Clamp", physicalDeviceInfo->features.depthBiasClamp);
                        GUI::displayConfigInfoBool("Fill Mode Non Solid", physicalDeviceInfo->features.fillModeNonSolid);
                        GUI::displayConfigInfoBool("Depth Bounds", physicalDeviceInfo->features.depthBounds);
                        GUI::displayConfigInfoBool("Wide Lines", physicalDeviceInfo->features.wideLines);
                        GUI::displayConfigInfoBool("Large Points", physicalDeviceInfo->features.largePoints);
                        GUI::displayConfigInfoBool("Alpha To One", physicalDeviceInfo->features.alphaToOne);
                        GUI::displayConfigInfoBool("Multi Viewport", physicalDeviceInfo->features.multiViewport);
                        GUI::displayConfigInfoBool("Sampler Anisotropy", physicalDeviceInfo->features.samplerAnisotropy);
                        GUI::displayConfigInfoBool("Texture Compression ETC2", physicalDeviceInfo->features.textureCompressionETC2);
                        GUI::displayConfigInfoBool("Texture Compression ASTC_LDR", physicalDeviceInfo->features.textureCompressionASTC_LDR);
                        GUI::displayConfigInfoBool("Texture Compression BC", physicalDeviceInfo->features.textureCompressionBC);
                        GUI::displayConfigInfoBool("Occlusion Query Precise", physicalDeviceInfo->features.occlusionQueryPrecise);
                        GUI::displayConfigInfoBool("Pipeline Statistics Query", physicalDeviceInfo->features.pipelineStatisticsQuery);
                        GUI::displayConfigInfoBool("Vertex Pipeline Stores And Atomics", physicalDeviceInfo->features.vertexPipelineStoresAndAtomics);
                        GUI::displayConfigInfoBool("Fragment Stores And Atomics", physicalDeviceInfo->features.fragmentStoresAndAtomics);
                        GUI::displayConfigInfoBool("Shader Tessellation And Geometry Point Size", physicalDeviceInfo->features.shaderTessellationAndGeometryPointSize);
                        GUI::displayConfigInfoBool("Shader Image Gather Extended", physicalDeviceInfo->features.shaderImageGatherExtended);
                        GUI::displayConfigInfoBool("Shader Storage Image Extended Formats", physicalDeviceInfo->features.shaderStorageImageExtendedFormats);
                        GUI::displayConfigInfoBool("Shader Storage Image Multisample", physicalDeviceInfo->features.shaderStorageImageMultisample);
                        GUI::displayConfigInfoBool("Shader Storage Image Read Without Format", physicalDeviceInfo->features.shaderStorageImageReadWithoutFormat);
                        GUI::displayConfigInfoBool("Shader Storage Image Write Without Format", physicalDeviceInfo->features.shaderStorageImageWriteWithoutFormat);
                        GUI::displayConfigInfoBool("Shader Uniform Buffer Array Dynamic Indexing", physicalDeviceInfo->features.shaderUniformBufferArrayDynamicIndexing);
                        GUI::displayConfigInfoBool("Shader Sampled Image Array Dynamic Indexing", physicalDeviceInfo->features.shaderSampledImageArrayDynamicIndexing);
                        GUI::displayConfigInfoBool("Shader Storage Buffer Array Dynamic Indexing", physicalDeviceInfo->features.shaderStorageBufferArrayDynamicIndexing);
                        GUI::displayConfigInfoBool("Shader Storage Image Array Dynamic Indexing", physicalDeviceInfo->features.shaderStorageImageArrayDynamicIndexing);
                        GUI::displayConfigInfoBool("Shader Clip Distance", physicalDeviceInfo->features.shaderClipDistance);
                        GUI::displayConfigInfoBool("Shader Cull Distance", physicalDeviceInfo->features.shaderCullDistance);
                        GUI::displayConfigInfoBool("Shader Float64", physicalDeviceInfo->features.shaderFloat64);
                        GUI::displayConfigInfoBool("Shader Int64", physicalDeviceInfo->features.shaderInt64);
                        GUI::displayConfigInfoBool("Shader Int16", physicalDeviceInfo->features.shaderInt16);
                        GUI::displayConfigInfoBool("Shader ResourceResidency", physicalDeviceInfo->features.shaderResourceResidency);
                        GUI::displayConfigInfoBool("Shader ResourceMinLod", physicalDeviceInfo->features.shaderResourceMinLod);
                        GUI::displayConfigInfoBool("Sparse Binding", physicalDeviceInfo->features.sparseBinding);
                        GUI::displayConfigInfoBool("Sparse Residency Buffer", physicalDeviceInfo->features.sparseResidencyBuffer);
                        GUI::displayConfigInfoBool("Sparse Residency Image2D", physicalDeviceInfo->features.sparseResidencyImage2D);
                        GUI::displayConfigInfoBool("Sparse Residency Image3D", physicalDeviceInfo->features.sparseResidencyImage3D);
                        GUI::displayConfigInfoBool("Sparse Residency 2Samples", physicalDeviceInfo->features.sparseResidency2Samples);
                        GUI::displayConfigInfoBool("Sparse Residency 4Samples", physicalDeviceInfo->features.sparseResidency4Samples);
                        GUI::displayConfigInfoBool("Sparse Residency 8Samples", physicalDeviceInfo->features.sparseResidency8Samples);
                        GUI::displayConfigInfoBool("Sparse Residency 16Samples", physicalDeviceInfo->features.sparseResidency16Samples);
                        GUI::displayConfigInfoBool("Sparse Residency Aliased", physicalDeviceInfo->features.sparseResidencyAliased);
                        GUI::displayConfigInfoBool("Variable Multisample Rate", physicalDeviceInfo->features.variableMultisampleRate);
                        GUI::displayConfigInfoBool("Inherited Queries", physicalDeviceInfo->features.inheritedQueries);
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
                                ImGui::TextColored(ImVec4(0, 255, 0, 255), "Type %lu", i);
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
                    for (auto extension : physicalDeviceInfo->extensions) {
                        GUI::displayConfigInfoVersion(extension.extensionName, lug::Core::Version::fromInt(extension.specVersion));
                    }
                    ImGui::Unindent();
                }

                if (ImGui::CollapsingHeader("Formats")) {
                    ImGui::Indent();
                    for (auto format : physicalDeviceInfo->formatProperties) {
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

        ImGui::Separator();
        ImGui::NewLine();
        if (ImGui::Button("< RETURN")) {
            *isOpen = false;
        }
    }
    ImGui::End();
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

        size_t deviceCount = devices->size();
        if (deviceCount > 0) {
            //			GUI::displayScoreInCell(_physicalDeviceInfo->properties.deviceName, 68.7f, 0.7f);
            nlohmann::json highestScoreDevice = (*devices)[0];
            float bigestStore = highestScoreDevice["averageFps"].get<float>();
            GUI::displayScoreInCell(physicalDeviceInfo->properties.deviceName, 68.7f, 0.0f);
    
            ImGui::BeginChild("Score list"); {
                for (uint32_t i = 0; i < deviceCount; i++) {
                    ImGui::PushID(i);
                    nlohmann::json device = (*devices)[i];
                    GUI::displayScoreInCell((*devices)[i]["device"].get<std::string>().c_str(), 68.7f, bigestStore / (*devices)[i]["averageFps"].get<float>());
                    //				GUI::displayScoreInCell(_physicalDeviceInfo->properties.deviceName, 68.7f, 1.0f - static_cast<float>(i) / 10.0f);
                    ImGui::PopID();
                }
            }
            ImGui::EndChild();
        }
        ImGui::NewLine();
        if (ImGui::Button("< RETURN")) {
            *isOpen = false;
        }
    }
    ImGui::End();
}
