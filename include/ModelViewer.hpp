#pragma once

#include <lug/Core/Export.hpp>
#include <lug/Graphics/Scene/Node.hpp>
#include <lug/Math/Vector.hpp>
#include <lug/Window/Window.hpp>
#include <lug/Window/Event.hpp>
#include <lug/System/Time.hpp>

class ModelViewer {
public:
    explicit ModelViewer() = default;
    ~ModelViewer() = default;

    /**
     * @brief      Sets the camera node (i.e. the node to which the camera is attached to).
     */
    void setCameraNode(lug::Graphics::Scene::Node& camera);

    /**
     * @brief      Sets the model node (i.e. the node to which the model is attached to).
     */
    void setModelNode(lug::Graphics::Scene::Node& model);

    /**
     * @brief      Sets the event source, a Window instance.
     *
     * @param[in]  eventSource  The event source
     */
    void setEventSource(lug::Window::Window& eventSource);

    void onFrame(const lug::System::Time& elapsedTime);
    void onEvent(const lug::Window::Event& event);

private:
    lug::Graphics::Scene::Node* _camera{nullptr};
    lug::Graphics::Scene::Node* _model{nullptr};
    lug::Window::Window* _eventSource{nullptr};
    float _speed{0.005f};
    float _zoom{0.0f};

    bool _hasFocus{false};
    lug::Math::Vec2i _lastMousePos{0, 0};
};

#include "ModelViewer.inl"
