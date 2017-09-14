#include <LugNetwork.hpp>

#include <lug/Config.hpp>
#include <lug/System/Logger/Logger.hpp>

#include <thread>

#if defined(LUG_SYSTEM_ANDROID)
    #include <jni.h>
    #include <lug/Window/Android/WindowImplAndroid.hpp>
#else
    #include <restclient-cpp/restclient.h>
    #include <restclient-cpp/connection.h>
#endif

#if defined(LUG_SYSTEM_ANDROID)
extern "C" JNIEXPORT void JNICALL
Java_com_lugdunum_lugbench_LugBenchNativeActivity_nativeCallback(JNIEnv* env, jobject, jint statusCode, jstring body) {
    LugBench::LugNetwork::getInstance().setLastRequestStatusCode((int)statusCode);
    LugBench::LugNetwork::getInstance().setLastRequestBody(env->GetStringUTFChars(body, 0));
    LugBench::LugNetwork::getInstance().getMutex().unlock();
}
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

LugNetwork &LugNetwork::getInstance()
{
    static LugNetwork lugNetwork;
    return lugNetwork;
}

std::string LugNetwork::urlToString(Route route, const std::string& id) {
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

        case Route::sendDevice:
            return std::string(baseNetworkUri) + "/" + "devices";
        case Route::sendScore:
            return std::string(baseNetworkUri) + "/" + "scores";

        default:
            return "";
    }
}

void LugNetwork::makeRequest(Method method, std::string url, const std::string& json) {
    if (!_mutex.try_lock()) {
        LUG_LOG.error("Already performing a request");
        return;
    }
    _lastRequestBody = {};
    _lastRequestStatusCode = 0;
    _mutex.unlock();
    LUG_LOG.info("sending {} request with url {}", method == Method::POST ? "POST" : "GET", url);
    std::thread networkThread;
    if (method == Method::POST) {
        networkThread = std::thread(&LugNetwork::post, this, url, json);
    } else {
        networkThread = std::thread(&LugNetwork::get, this, url);
    }
    networkThread.detach();
}

#if defined(LUG_SYSTEM_ANDROID)

void LugNetwork::get(const std::string& url) {
    _mutex.lock();

    JNIEnv *jni = lug::Window::priv::WindowImpl::activity->env;

    lug::Window::priv::WindowImpl::activity->vm->AttachCurrentThread(&jni, NULL);

    jclass clazz = jni->GetObjectClass(lug::Window::priv::WindowImpl::activity->clazz);
    if (clazz == 0) {
        LUG_LOG.error("FindClass error");
        lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();
        return;
    }
    jmethodID javamethod = jni->GetMethodID(clazz, "get", "(Ljava/lang/String;)V");
    if (javamethod == 0) {
        LUG_LOG.info("GetMethodID error");
        lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();
        return;
    }

    jstring nativeUrl = jni->NewStringUTF(url.c_str());

    jni->CallVoidMethod(lug::Window::priv::WindowImpl::activity->clazz, javamethod, nativeUrl);

    lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();
}

void LugNetwork::post(const std::string& url, const std::string& json) {
    _mutex.lock();

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
}

#else

void LugNetwork::get(const std::string& url) {
    _mutex.lock();
    RestClient::Connection* conn = new RestClient::Connection(url);
    conn->SetUserAgent("LugBench/0.1.0");
    conn->AppendHeader("Content-Type", "application/json"); // TODO(Yoann) : to remove when API is fixed

    while (1) {
        RestClient::Response r = conn->get("");
        if (r.code != -1) {
            _lastRequestBody = r.body;
            _lastRequestStatusCode = r.code;
            _mutex.unlock();
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void LugNetwork::post(const std::string& url, const std::string& json) {
    _mutex.lock();
    RestClient::Connection* conn = new RestClient::Connection(url);
    conn->SetUserAgent("LugBench/0.1.0");
    conn->AppendHeader("Content-Type", "application/json");
    while (1) {
        RestClient::Response r = conn->post("", json);
        if (r.code != -1) {
            _lastRequestBody = r.body;
            _lastRequestStatusCode = r.code;
            _mutex.unlock();
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

#endif

} // LugBench
