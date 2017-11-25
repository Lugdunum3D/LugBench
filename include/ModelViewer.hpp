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
    void setTargetNode(lug::Graphics::Scene::Node& camera);

    /**
     * @brief      Sets the event source, a Window instance.
     *
     * @param[in]  eventSource  The event source
     */
    void setEventSource(lug::Window::Window& eventSource);

    void onFrame(const lug::System::Time& elapsedTime);
    void onEvent(const lug::Window::Event& event);

    void rotate(float x, float y);

private:
    bool isRotationEnd(const lug::Window::Event& event);
    void updateRotation();

private:
    lug::Graphics::Scene::Node* _target{nullptr};
    lug::Window::Window* _eventSource{nullptr};
    float _rotationSpeed{0.25f};
    float _zoomSpeed{1.0f};
    float _zoom{5.0f};
    lug::Math::Vec2f _rotation{0.0f, 0.0f};

    float _zoomUpdate{0.0f};
    bool _isMousePressed{false};

    lug::Math::Vec2i _lastMousePos{0, 0};

    float _lastPinchGap = 0;

    lug::Math::Vec2f _lastDragPosition{0,0};
    lug::Math::Vec2f _lastRotationVelocity{0, 0};
    lug::Math::Vec2f _rotationVelocity{0.0f, 0.0f};
};

#include "ModelViewer.inl"
