#include <APIClient/Common.hpp>

#include <restclient-cpp/restclient.h>

APIClient::JSONResponse APIClient::getRequestResponse(const std::string& url) {
    RestClient::Response r = RestClient::get(url);
    nlohmann::json json = nlohmann::json(r.body);
    return std::make_tuple(r.code, json);
}

APIClient::JSONResponse APIClient::putRequest(const std::string& url, const std::string& json) {
    nlohmann::json reponseJson;
    RestClient::Response r = RestClient::put(url, "application/json;charset=utf-8", json);
    reponseJson = nlohmann::json(r.body);
    return std::make_tuple(r.code, reponseJson);
}