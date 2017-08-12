#pragma once

#include <string>
#include <mutex>

static constexpr const char* baseNetworkUri = LUGBENCH_API_URI;
static constexpr const char* NetworkVersion = LUGBENCH_API_VERSION;

namespace LugBench {

enum class Route : uint8_t {
    getDevice,
    getDevices,
    getScore,
    getScores,
    getScenario,
    getScenarios,
    putDevice,
    putScore
};

class LugNetwork {
public:
    LugNetwork();
    ~LugNetwork();

    void putDevice(const std::string& json);
    void putScore(const std::string& json);

    void getDevice(const std::string& id);
    void getDevices();

    void getScore(const std::string& id);
    void getScores();

    void getScenario(const std::string& id);
    void getScenarios();

    int getLastRequestStatusCode() {
        return _lastRequestStatusCode;
    }

    std::string getLastRequestBody() {
        return _lastRequestBody;
    }

    std::mutex &getMutex() {
        return _mutex;
    }

    static LugNetwork                     &getInstance();
    std::string _lastRequestBody{};
    int _lastRequestStatusCode{0};

private:
    void get(const std::string& url);
    void put(const std::string& url, const std::string& json);
    std::string getUrlString(Route route, const std::string& id = "");

    std::mutex _mutex;
};

} // LugBench