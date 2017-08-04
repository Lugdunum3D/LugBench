#include <Network.hpp>

#include <lug/Config.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <json/json.hpp>

#if defined(LUG_SYSTEM_ANDROID)
    #include <jni.h>
    #include <lug/Window/Android/WindowImplAndroid.hpp>
#else
    #include <restclient-cpp/restclient.h>
    #include <restclient-cpp/connection.h>
#endif

namespace LugBench {

Network::Network() {
#if !defined(LUG_SYSTEM_ANDROID)
    RestClient::init();
#endif
}

Network::~Network() {
#if !defined(LUG_SYSTEM_ANDROID)
    RestClient::disable();
#endif
}

void Network::putDevice(const nlohmann::json& json) {
    _networkThread = std::thread(&Network::put, this, getUrlString(Route::putDevice), json.dump());
    _networkThread.detach();
}

void Network::putScore(const nlohmann::json& json) {
    _networkThread = std::thread(&Network::put, this, getUrlString(Route::putScore), json.dump());
    _networkThread.detach();
}

void Network::getDevice(const std::string& id) {
    return get(getUrlString(Route::getDevice, id));
}

void Network::getDevices() {
    return get(getUrlString(Route::getDevices));
}

void Network::getScore(const std::string& id) {
    return get(getUrlString(Route::getScore, id));
}

void Network::getScores() {
    return get(getUrlString(Route::getScores));
}

void Network::getScenario(const std::string& id) {
    return get(getUrlString(Route::getScenario, id));
}

void Network::getScenarios() {
    return get(getUrlString(Route::getScenarios));
}

std::string Network::getUrlString(Route route, const std::string& id) {
    switch (route) {
        case Route::getDevice:
            return std::string(baseNetworkUri) + "/" + "devices" + "/" + id;
        case Route::getDevices:
            return std::string(baseNetworkUri) + "/" + "devices";

        case Route::getScore:
            return std::string(baseNetworkUri) + "/" + "scores" + "/" + id;
        case Route::getScores:
            return std::string(baseNetworkUri) + "/" + "scores";

        case Route::getScenario:
            return std::string(baseNetworkUri) + "/" + "scenarios" + "/" + id;
        case Route::getScenarios:
            return std::string(baseNetworkUri) + "/" + "scenarios";

        case Route::putDevice:
            return std::string(baseNetworkUri) + "/" + "devices";
        case Route::putScore:
            return std::string(baseNetworkUri) + "/" + "scores";

        default:
            return "";
    }
}

#if defined(LUG_SYSTEM_ANDROID)

void Network::get(const std::string& url) {
    LUG_LOG.info("url is : {}", url);

    JNIEnv *jni = lug::Window::priv::WindowImpl::activity->env;

    lug::Window::priv::WindowImpl::activity->vm->AttachCurrentThread(&jni, NULL);

    jclass clazz = jni->GetObjectClass(lug::Window::priv::WindowImpl::activity->clazz);
    if (clazz == 0) {
        LUG_LOG.error("FindClass error");
        return {};
    }
    jmethodID javamethod = jni->GetMethodID(clazz, "getRequest", "(Ljava/lang/String;)Ljava/lang/String;");
    if (javamethod == 0) {
        LUG_LOG.info("GetMethodID error");
        return {};
    }

    jstring nativeUrl = jni->NewStringUTF(url.c_str());

    jstring rv = (jstring)jni->CallObjectMethod(lug::Window::priv::WindowImpl::activity->clazz, javamethod, nativeUrl);

    LUG_LOG.info("getRequest CPP SIDE : {}", jni->GetStringUTFChars(rv, 0));

    lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();

    return {};
}

void Network::put(const std::string& url, const std::string& json) {
    LUG_LOG.info("url is : {}", url);

    JNIEnv *jni = lug::Window::priv::WindowImpl::activity->env;

    lug::Window::priv::WindowImpl::activity->vm->AttachCurrentThread(&jni, NULL);

    jclass clazz = jni->GetObjectClass(lug::Window::priv::WindowImpl::activity->clazz);
    if (clazz == 0) {
        LUG_LOG.error("FindClass error");
        return {};
    }
    jmethodID javamethod = jni->GetMethodID(clazz, "sendRequest", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    if (javamethod == 0) {
        LUG_LOG.info("GetMethodID error");
        return {};
    }

    jstring nativeUrl = jni->NewStringUTF(url.c_str());
    jstring nativeJson = jni->NewStringUTF(json.c_str());

    jstring rv = (jstring)jni->CallObjectMethod(lug::Window::priv::WindowImpl::activity->clazz, javamethod, nativeUrl, nativeJson);

    LUG_LOG.info("putRequest CPP SIDE : {}", jni->GetStringUTFChars(rv, 0));

    lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();
    return {};
}

#else

void Network::get(const std::string& url) {
    RestClient::Response r = RestClient::get(url);
    nlohmann::json json = nlohmann::json::parse(r.body);
    _response = std::make_tuple(r.code, json);
}

void Network::put(const std::string& url, const std::string& json) {
    _mutex.lock();
    // get a connection object
    RestClient::Connection* conn = new RestClient::Connection(url);
    conn->SetUserAgent("LugBench/0.1.0"); // TODO(Yoann) better version handling

    while (1) {
        RestClient::Response r = conn->post("", json);
        if (r.code != -1) {
            nlohmann::json reponseJson = nlohmann::json::parse(r.body);
            _response = std::make_tuple(r.code, reponseJson);
            _mutex.unlock();
            return;
        }
    }
}

#endif

} // LugBench