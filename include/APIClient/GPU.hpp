#pragma once

#include <json/json.hpp>

#include <APIClient/Common.hpp>

namespace APIClient {
namespace GPU {

JSONResponse put(const nlohmann::json& json);
JSONResponse getAll();
JSONResponse get(const std::string& id);

} // GPU
} // APIClient
