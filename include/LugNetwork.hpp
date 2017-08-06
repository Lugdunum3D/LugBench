#pragma once

#include <string>
#if !defined(LUG_SYSTEM_ANDROID)
    #include <mutex>
#endif

static constexpr const char* baseNetworkUri = LUGBENCH_API_URI;
static constexpr const char* NetworkVersion = LUGBENCH_API_VERSION;

namespace LugBench {

enum  Route : uint8_t {
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

    int getLastResquestStatusCode() {
        return _lastResquestStatusCode;
    }

    std::string getLastRequestBody() {
        return _lastResquestBody;
    }

private:
    void get(const std::string& url);
    void put(const std::string& url, const std::string& json);
    std::string getUrlString(Route route, const std::string& id = "");

#if !defined(LUG_SYSTEM_ANDROID)
    std::mutex _mutex;
#endif
    std::string _lastResquestBody{};
    int _lastResquestStatusCode{0};
};

} // LugBench