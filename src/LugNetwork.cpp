#include <LugNetwork.hpp>

#include <lug/Config.hpp>
#include <lug/System/Logger/Logger.hpp>

#include <thread>

#if defined(LUG_SYSTEM_ANDROID)
    #include <jni.h>
    #include <lug/Window/Android/WindowImplAndroid.hpp>
#endif

namespace LugBench {

LugNetwork::LugNetwork() {
#if !defined(LUG_SYSTEM_ANDROID)
    RestClient::init();
#endif
}

LugNetwork::~LugNetwork() {
#if !defined(LUG_SYSTEM_ANDROID)
    RestClient::disable();
#endif
}

void LugNetwork::putDevice(const std::string& json) {
    std::thread networkThread = std::thread(&LugNetwork::put, this, getUrlString(Route::putDevice), json);
    networkThread.detach();
}

void LugNetwork::putScore(const std::string& json) {
    std::thread networkThread = std::thread(&LugNetwork::put, this, getUrlString(Route::putScore), json);
    networkThread.detach();
}

void LugNetwork::getDevice(const std::string& id) {
    return get(getUrlString(Route::getDevice, id));
}

void LugNetwork::getDevices() {
    return get(getUrlString(Route::getDevices));
}

void LugNetwork::getScore(const std::string& id) {
    return get(getUrlString(Route::getScore, id));
}

void LugNetwork::getScores() {
    return get(getUrlString(Route::getScores));
}

void LugNetwork::getScenario(const std::string& id) {
    return get(getUrlString(Route::getScenario, id));
}

void LugNetwork::getScenarios() {
    return get(getUrlString(Route::getScenarios));
}

std::string LugNetwork::getUrlString(Route route, const std::string& id) {
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

    }
}

#if defined(LUG_SYSTEM_ANDROID)

void LugNetwork::get(const std::string&) {
    // TODO
}

void LugNetwork::put(const std::string& url, const std::string& json) {
    LUG_LOG.info("url is : {}", url);

    _lastResquestBody = {};
    _lastResquestStatusCode = 0;

    JNIEnv *jni = lug::Window::priv::WindowImpl::activity->env;

    lug::Window::priv::WindowImpl::activity->vm->AttachCurrentThread(&jni, NULL);

    jclass clazz = jni->GetObjectClass(lug::Window::priv::WindowImpl::activity->clazz);
    if (clazz == 0) {
        LUG_LOG.error("FindClass error");
        lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();
        return;
    }
    jmethodID javamethod = jni->GetMethodID(clazz, "put", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (javamethod == 0) {
        LUG_LOG.info("GetMethodID error");
        lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();
        return;
    }

    jstring nativeUrl = jni->NewStringUTF(url.c_str());
    jstring nativeJson = jni->NewStringUTF(json.c_str());

    jni->CallVoidMethod(lug::Window::priv::WindowImpl::activity->clazz, javamethod, nativeUrl, nativeJson);

    lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();

    {
        while (1) {
            JNIEnv *jni = lug::Window::priv::WindowImpl::activity->env;
            lug::Window::priv::WindowImpl::activity->vm->AttachCurrentThread(&jni, NULL);

            jclass clazz = jni->GetObjectClass(lug::Window::priv::WindowImpl::activity->clazz);
            if (!clazz) {
                LUG_LOG.error("FindClass error");
                lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();
                return;
            }

            jmethodID javamethod = jni->GetMethodID(clazz, "getLastRequestBody", "()Ljava/lang/String;");
            if (!javamethod) {
                LUG_LOG.error("GetMethodID error");
                lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();
                return;
            }

            jstring rv = (jstring)jni->CallObjectMethod(lug::Window::priv::WindowImpl::activity->clazz, javamethod);
            if (rv) {
                _lastResquestBody = jni->GetStringUTFChars(rv, 0);

                javamethod = jni->GetMethodID(clazz, "getLastRequestStatusCode", "()I");
                if (!javamethod) {
                    LUG_LOG.error("GetMethodID error");
                    lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();
                    return;
                }

                int statusCode = (int)jni->CallIntMethod(lug::Window::priv::WindowImpl::activity->clazz, javamethod);

                _lastResquestStatusCode = statusCode;

                lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();
                return;
            }

            lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    }

}

#else

void Network::get(const std::string& url) {
    // RestClient::Response r = RestClient::get(url);
    // nlohmann::json json = nlohmann::json::parse(r.body);
    // _response = std::make_tuple(r.code, json);
}

void Network::put(const std::string& url, const std::string& json) {
    _mutex.lock();
    RestClient::Connection* conn = new RestClient::Connection(url);
    conn->SetUserAgent("LugBench/0.1.0"); // TODO(Yoann) better version handling

    while (1) {
        RestClient::Response r = conn->post("", json);
        if (r.code != -1) {
            _lastResquestBody = r.body;
            _lastResquestStatusCode = r.code;
            _mutex.unlock();
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

#endif

} // LugBench