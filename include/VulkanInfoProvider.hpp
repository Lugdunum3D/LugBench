#pragma once

#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#pragma warning(push)  
#pragma warning( disable : 4003)
#include <json/json.hpp>
#pragma warning(pop)

class VulkanInfoProvider
{
public:
    VulkanInfoProvider(lug::Graphics::Vulkan::InstanceInfo infos);
    ~VulkanInfoProvider();

    lug::Graphics::Vulkan::InstanceInfo infos;

    const  nlohmann::json &getJSONVulkAnInfo();
private:

};

