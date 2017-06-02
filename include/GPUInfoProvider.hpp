#pragma once

#include <lug/Graphics/Vulkan/Vulkan.hpp>

#include <json/json.hpp>

namespace GPUInfoProvider {

nlohmann::json get(const lug::Graphics::Vulkan::PhysicalDeviceInfo& physicalDeviceInfo);

} // GPUInfoProvider
