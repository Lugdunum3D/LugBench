inline int LugNetwork::getLastRequestStatusCode() {
    return _lastRequestStatusCode;
}

inline std::string LugNetwork::getLastRequestBody() {
    return _lastRequestBody;
}

inline std::mutex &LugNetwork::getMutex() {
    return _mutex;
}

inline void LugNetwork::setLastRequestStatusCode(int code) {
    _lastRequestStatusCode = code;
}

inline void LugNetwork::setLastRequestBody(std::string body) {
    _lastRequestBody = body;   
}
