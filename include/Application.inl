inline lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::Camera::Camera> Application::getCamera() {
    return _camera;
}

inline bool Application::isSending() const {
    return (_isSendingDevice || _isSendingScore);
}
