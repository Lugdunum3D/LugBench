inline void ModelViewer::setCameraNode(lug::Graphics::Scene::Node& camera) {
    _camera = &camera;
}

inline void ModelViewer::setModelNode(lug::Graphics::Scene::Node& model) {
    _model = &model;
}

inline void ModelViewer::setEventSource(lug::Window::Window& eventSource) {
    _eventSource = &eventSource;
    _lastMousePos = _eventSource->getMousePos();
}
