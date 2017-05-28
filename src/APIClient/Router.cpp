#include "APIClient/Router.hpp"

constexpr const char* APIClient::Router::baseAPIUri;

std::string APIClient::Router::getUrlString(Route route, uint16_t id = 0) {
    std::string	URLString;

    switch (route) {
        case Router::Route::getAllVulkanInfo:
            URLString = std::string(baseAPIUri) + "/" + std::string(apiVersion) + "/gpus";
            break;
        case Router::Route::getVulkanInfoWithId:
            URLString = std::string(baseAPIUri) + "/" + std::string(apiVersion) + "/gpus" + std::to_string(id);
            break;
        case Router::Route::putVulkanInfo:
            URLString = std::string(baseAPIUri) + "/" + std::string(apiVersion) + "/gpus";
            break;
    }

    (void)id;
    return URLString;
}

