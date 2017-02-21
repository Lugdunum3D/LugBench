#pragma once

#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <json.hpp>

class VulkanInfoProvider
{
public:
    VulkanInfoProvider(lug::Graphics::Vulkan::InstanceInfo infos);
    ~VulkanInfoProvider();

    lug::Graphics::Vulkan::InstanceInfo infos;

    const  nlohmann::json &getJSONVulkAnInfo();
private:
    
};

