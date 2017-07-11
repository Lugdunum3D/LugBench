#include <APIClient/GPU.hpp>

#include <APIClient/Router.hpp>

APIClient::JSONResponse APIClient::putDevice(const nlohmann::json& json) {
	return APIClient::putRequest(APIClient::Router::getUrlString(APIClient::Router::Route::postDevice), json.dump());
}

APIClient::JSONResponse APIClient::putScore(const nlohmann::json& json) {
	return APIClient::putRequest(APIClient::Router::getUrlString(APIClient::Router::Route::postScore), json.dump());
}

APIClient::JSONResponse APIClient::getDevice(const std::string& id) {
    return APIClient::getRequestResponse(APIClient::Router::getUrlString(Router::Route::getDevice, id));
}

APIClient::JSONResponse APIClient::getDevices() {
    return APIClient::getRequestResponse(APIClient::Router::getUrlString(Router::Route::getDevice));
}

APIClient::JSONResponse APIClient::getScore(const std::string& id) {
    return APIClient::getRequestResponse(APIClient::Router::getUrlString(Router::Route::getScore, id));
}

APIClient::JSONResponse APIClient::getScores() {
    return APIClient::getRequestResponse(APIClient::Router::getUrlString(Router::Route::getScores));
}

APIClient::JSONResponse APIClient::getScenario(const std::string& id) {
    return APIClient::getRequestResponse(APIClient::Router::getUrlString(Router::Route::getScenario, id));
}

APIClient::JSONResponse APIClient::getScenarios() {
    return APIClient::getRequestResponse(APIClient::Router::getUrlString(Router::Route::getScenarios));
}
