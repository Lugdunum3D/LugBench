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
    sendDevice,
    sendScore
};

enum class Method : uint8_t {
    POST,
    GET
};

class LugNetwork {
public:
    LugNetwork();
    ~LugNetwork();

    void makeRequest(Method method, std::string url, const std::string& json = "");

    int getLastRequestStatusCode();
    std::string getLastRequestBody();
    void setLastRequestStatusCode(int code);
    void setLastRequestBody(std::string body);

    std::mutex &getMutex();

    static LugNetwork &getInstance();
    static std::string urlToString(Route route, const std::string& id = "");

    std::string _lastRequestBody{};
    int _lastRequestStatusCode{0};

private:
    void get(const std::string& url);
    void post(const std::string& url, const std::string& json);

    std::mutex _mutex;
};

#include "LugNetwork.inl"

} // LugBench