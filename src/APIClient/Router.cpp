#include <APIClient/Router.hpp>

std::string APIClient::Router::getUrlString(Route route, const std::string& id) {
    switch (route) {
        case Route::getAllGpus:
            return std::string(baseAPIUri) + "/" + "devices";
        case Route::getGpu:
            return std::string(baseAPIUri) + "/" + "devices" + "/" + id;
        case Route::putGpu:
            return std::string(baseAPIUri) + "/" + "devices";
        default:
            return "";
    }
}
