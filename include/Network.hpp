#pragma once

#include <json/json.hpp>
#include <mutex>

static constexpr const char* baseNetworkUri = LUGBENCH_API_URI;
static constexpr const char* NetworkVersion = LUGBENCH_API_VERSION;

using JSONResponse = std::tuple<int, nlohmann::json>;

namespace LugBench {

enum Route : uint8_t {
    getDevice,
    getDevices,
    getScore,
    getScores,
    getScenario,
    getScenarios,
    putDevice,
    putScore
};

class Network {
public:
    Network();
    ~Network();

    void putDevice(const nlohmann::json& json);
    void putScore(const nlohmann::json& json);

    void getDevice(const std::string& id);
    void getDevices();

    void getScore(const std::string& id);
    void getScores();

    void getScenario(const std::string& id);
    void getScenarios();

    JSONResponse _response;
    std::mutex _mutex;

private:
    void get(const std::string& url);
    void put(const std::string& url, const std::string& json);
    std::string getUrlString(Route route, const std::string& id = "");

    std::thread _networkThread;
};

} // LugBench