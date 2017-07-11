#include <APIClient/Router.hpp>

std::string APIClient::Router::getUrlString(Route route, const std::string& id) {
    switch (route) {
        case Route::getDevice:
            return std::string(baseAPIUri) + "/" + "devices" + "/" + id;
        case Route::getDevices:
            return std::string(baseAPIUri) + "/" + "devices";

        case Route::getScore:
            return std::string(baseAPIUri) + "/" + "scores" + "/" + id;
        case Route::getScores:
            return std::string(baseAPIUri) + "/" + "scores";

        case Route::getScenario:
            return std::string(baseAPIUri) + "/" + "scenarios" + "/" + id;
        case Route::getScenarios:
            return std::string(baseAPIUri) + "/" + "scenarios";

        case Route::postDevice:
            return std::string(baseAPIUri) + "/" + "devices";
        case Route::postScore:
            return std::string(baseAPIUri) + "/" + "scores";

        default:
            return "";
    }
}
