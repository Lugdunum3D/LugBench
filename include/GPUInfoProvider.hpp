#pragma once

#include <lug/Graphics/Vulkan/Vulkan.hpp>

#include <json/json.hpp>

namespace GPUInfoProvider {

    //TODO (4) utiliser type json provenant de cpprestsdk
nlohmann::json get(const lug::Graphics::Vulkan::PhysicalDeviceInfo& physicalDeviceInfo);

} // GPUInfoProvider
