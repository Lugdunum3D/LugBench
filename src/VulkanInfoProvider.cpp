#include "VulkanInfoProvider.hpp"

VulkanInfoProvider::VulkanInfoProvider(lug::Graphics::Vulkan::InstanceInfo& instanceInfo) : _instanceInfo(instanceInfo) {}

nlohmann::json VulkanInfoProvider::getJSONVulkanInfo() {
    nlohmann::json json;

    (void)_instanceInfo;

    return json;
}
