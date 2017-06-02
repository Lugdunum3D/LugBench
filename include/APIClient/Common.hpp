#pragma once

#include <string>
#include <tuple>

#include <json/json.hpp>

namespace APIClient {

using ResponseCode = int;
using JSONResponse = std::tuple<ResponseCode, nlohmann::json>;

JSONResponse getRequestResponse(const std::string& url);
JSONResponse putRequest(const std::string& url, const std::string& json);


} // APIClient
