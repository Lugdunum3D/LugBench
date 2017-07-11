#pragma once

#include <json/json.hpp>

#include <APIClient/Common.hpp>

namespace APIClient {

JSONResponse putDevice(const nlohmann::json& json);
JSONResponse putScore(const nlohmann::json& json);

JSONResponse getDevice(const std::string& id);
JSONResponse getDevices();

JSONResponse getScore(const std::string& id);
JSONResponse getScores();

JSONResponse getScenario(const std::string& id);
JSONResponse getScenarios();

} // APIClient
