#include <APIClient/Common.hpp>

#include <lug/Config.hpp>

#if defined(LUG_SYSTEM_ANDROID)

#include <jni.h>
#include <lug/Window/Android/WindowImplAndroid.hpp>
#include <lug/System/Logger/Logger.hpp>

APIClient::JSONResponse APIClient::getRequestResponse(const std::string&) {
    // TODO
    return {};
}

APIClient::JSONResponse APIClient::putRequest(const std::string& url, const std::string& json) {
    LUG_LOG.info("url is : {}", url);

    JNIEnv *jni = lug::Window::priv::WindowImpl::activity->env;

    lug::Window::priv::WindowImpl::activity->vm->AttachCurrentThread(&jni, NULL);

    jclass clazz = jni->GetObjectClass(lug::Window::priv::WindowImpl::activity->clazz);
    if (clazz == 0) {
        LUG_LOG.error("FindClass error");
        return {};
    }
    jmethodID javamethod = jni->GetMethodID(clazz, "sendRequest", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (javamethod == 0) {
        LUG_LOG.info("GetMethodID error");
        return {};
    }

    jstring nativeUrl = jni->NewStringUTF(url.c_str());
    jstring nativeJson = jni->NewStringUTF(json.c_str());


    jni->CallVoidMethod(lug::Window::priv::WindowImpl::activity->clazz, javamethod, nativeUrl, nativeJson);

    lug::Window::priv::WindowImpl::activity->vm->DetachCurrentThread();
    return {};
}

#else

#include <restclient-cpp/restclient.h>
#include <restclient-cpp/connection.h>

APIClient::JSONResponse APIClient::getRequestResponse(const std::string& url) {
    RestClient::Response r = RestClient::get(url);
    nlohmann::json json = nlohmann::json(r.body);
    return std::make_tuple(r.code, json);
}

APIClient::JSONResponse APIClient::putRequest(const std::string& url, const std::string& json) {

	// get a connection object
    RestClient::Connection* conn = new RestClient::Connection(url);
    conn->SetUserAgent("LugBench/0.1.0"); // TODO(Yoann) better version handling

	RestClient::Response r = conn->post("", json);
    nlohmann::json reponseJson = nlohmann::json(nlohmann::json::parse(r.body));
    return std::make_tuple(r.code, reponseJson);
}

#endif
