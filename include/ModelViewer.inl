inline void ModelViewer::setTargetNode(lug::Graphics::Scene::Node& target) {
    _target = &target;
}

inline void ModelViewer::setEventSource(lug::Window::Window& eventSource) {
    _eventSource = &eventSource;
    _lastMousePos = _eventSource->getMousePos();
}
