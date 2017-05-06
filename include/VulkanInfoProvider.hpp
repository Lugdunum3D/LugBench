#pragma once

#include <lug/Config.hpp>
#include <lug/Graphics/Renderer.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>

#if defined(LUG_SYSTEM_WINDOWS)
    #pragma warning(push)
    #pragma warning(disable : 4003)
#endif
#include <json/json.hpp>
#if defined(LUG_SYSTEM_WINDOWS)
    #pragma warning(pop)
#endif

class VulkanInfoProvider
{
public:
    VulkanInfoProvider(lug::Graphics::Vulkan::InstanceInfo infos);
    ~VulkanInfoProvider();

    lug::Graphics::Vulkan::InstanceInfo infos;

    const  nlohmann::json &getJSONVulkAnInfo();
private:

};

