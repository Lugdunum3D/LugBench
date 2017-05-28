#pragma once

#include <lug/Config.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>

#if defined(LUG_SYSTEM_WINDOWS)
#pragma warning(push)
#pragma warning(disable : 4003)
#endif
#include <json/json.hpp>
#if defined(LUG_SYSTEM_WINDOWS)
#pragma warning(pop)
#endif

class VulkanInfoProvider {
public:
    VulkanInfoProvider(lug::Graphics::Vulkan::PhysicalDeviceInfo *physicalDeviceInfo);

    VulkanInfoProvider(const VulkanInfoProvider&) = delete;
    VulkanInfoProvider(VulkanInfoProvider&&) = delete;

    VulkanInfoProvider& operator=(const VulkanInfoProvider&) = delete;
    VulkanInfoProvider& operator=(VulkanInfoProvider&&) = delete;

    ~VulkanInfoProvider() = default;

    nlohmann::json getJSONVulkanInfo();

private:
    lug::Graphics::Vulkan::PhysicalDeviceInfo *_physicalDeviceInfo;
};

