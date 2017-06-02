#include <APIClient/GPU.hpp>

#include <APIClient/Router.hpp>

APIClient::JSONResponse APIClient::GPU::put(const nlohmann::json& json) {
	return APIClient::putRequest(APIClient::Router::getUrlString(APIClient::Router::Route::putGpu), json.dump());
}

APIClient::JSONResponse APIClient::GPU::getAll() {
    return APIClient::getRequestResponse(APIClient::Router::getUrlString(Router::Route::getAllGpus));
}

APIClient::JSONResponse APIClient::GPU::get(const std::string& id) {
    return APIClient::getRequestResponse(APIClient::Router::getUrlString(Router::Route::getGpu, id));
}
