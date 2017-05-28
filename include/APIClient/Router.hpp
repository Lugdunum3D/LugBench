#pragma once

#include <string>
#include <map>

using PutFormData = std::map<std::string, std::string>;

namespace APIClient {
class Router {
public:
    enum class Route : uint8_t {
        getAllVulkanInfo,
        getVulkanInfoWithId,
        putVulkanInfo
    };

    static constexpr const char* baseAPIUri = LUGBENCH_API_URI;
    static constexpr const char* apiVersion = LUGBENCH_API_VERSION;

    Router() = default;

    Router(const Router&) = delete;

    Router& operator=(const Router&) = delete;
    Router& operator=(Router&&) = delete;

    ~Router() = default;

    std::string getUrlString(Route route, uint16_t id);
};
} // APIClient