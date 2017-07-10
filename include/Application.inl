inline std::unique_ptr<lug::Graphics::Render::Camera>& Application::getCamera() {
    return _camera;
}

inline void Application::setCamera(std::unique_ptr<lug::Graphics::Render::Camera> camera) {
    _camera = std::move(camera);
}
