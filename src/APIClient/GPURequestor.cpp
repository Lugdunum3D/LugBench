#include "APIClient/GPURequestor.hpp"

int APIClient::GPURequestor::putVulkanInfo(nlohmann::json vulkanInfosJson) {
    std::cout << vulkanInfosJson.dump() << std::endl;
    RestClient::Response r = RestClient::put(_rout.getUrlString(Router::Route::putVulkanInfo, 0), "application/json;charset=utf-8", vulkanInfosJson.dump(0));
    return r.code;
}

JSONResponse APIClient::GPURequestor::getAllVulkanInfo() {
    return	getRequestResponse(_rout.getUrlString(Router::Route::getAllVulkanInfo, 0));
}

JSONResponse APIClient::GPURequestor::getVulkanInfoWithID(uint16_t id) {
    return	getRequestResponse(_rout.getUrlString(Router::Route::getVulkanInfoWithId, id));
}

JSONResponse APIClient::GPURequestor::getRequestResponse(std::string request) {
    nlohmann::json json;
    RestClient::Response r = RestClient::get(request);

    if (r.code >= 300 && r.code < 400) {
        json = nlohmann::json(r.body);
    }
    return std::make_tuple(r.code, json);
}