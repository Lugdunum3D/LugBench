#pragma once

#include <string>

namespace APIClient {
namespace Router {

enum class Route : uint8_t {
    getAllGpus,
    getGpu,
    putGpu
};

static constexpr const char* baseAPIUri = LUGBENCH_API_URI;
static constexpr const char* apiVersion = LUGBENCH_API_VERSION;
static constexpr const char* apiVersionUri = "api";

std::string getUrlString(Route route, const std::string& id = "");

} // Router
} // APIClient
