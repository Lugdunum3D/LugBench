#include <APIClient/Router.hpp>

std::string APIClient::Router::getUrlString(Route route, const std::string& id) {
    switch (route) {
        case Route::getAllGpus:
            return std::string(baseAPIUri) + "/" + std::string(apiVersionUri) + "/v" + std::string(apiVersion) + "/" + "gpus";
        case Route::getGpu:
            return std::string(baseAPIUri) + "/" + std::string(apiVersionUri) + "/v" + std::string(apiVersion) + "/" + "gpus" + "/" + id;
        case Route::putGpu:
            return std::string(baseAPIUri) + "/" + std::string(apiVersionUri) + "/v" + std::string(apiVersion) + "/" + "gpus";
        default:
            return "";
    }
}
