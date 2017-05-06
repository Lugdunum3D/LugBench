#pragma once

#include <string>
#include <map>

using PutFormData = std::map<std::string, std::string>;

class Router {
public:
    Router() = default;

    Router(const Router&) = delete;
    Router(Router&&) = delete;

    Router& operator=(const Router&) = delete;
    Router& operator=(Router&&) = delete;

    ~Router() = default;

    enum class Route : uint8_t {
        getAllVulkanInfo,
        getVulkanInfoWithId,
        putVulkanInfo
    };

public:
    static constexpr const char* baseAPIUri = LUGBENCH_API_URI;
};
