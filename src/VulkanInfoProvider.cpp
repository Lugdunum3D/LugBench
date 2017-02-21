
#include "VulkaninfoProvider.hpp"

VulkanInfoProvider::VulkanInfoProvider(lug::Graphics::Vulkan::InstanceInfo infos) : infos(infos)
{
}

VulkanInfoProvider::~VulkanInfoProvider()
{
}
const  nlohmann::json& VulkanInfoProvider::getJSONVulkAnInfo() {
    
    nlohmann::json json;

    
    // for (auto extension : info.extensions) {
    //     json[extension.extensionName] = extension.specVersion;
    // }


    return json;
}