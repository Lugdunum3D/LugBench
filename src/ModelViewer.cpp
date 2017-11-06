#include "ModelViewer.hpp"
#include <lug/System/Logger/Logger.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

void ModelViewer::onFrame(const lug::System::Time& elapsedTime) {
    if (_eventSource == nullptr) {
        LUG_LOG.warn("ModelViewer::onFrame: No event source, call setEventSource()");
        return;
    }

    _zoom += _zoomSpeed * _zoomUpdate * elapsedTime.getSeconds<float>();

    // GamePad View
    {
        float axisRightX = _eventSource->_gamePadState.axisRight.x();
        float axisRightY = _eventSource->_gamePadState.axisRight.y();

        if (axisRightX || axisRightY) {
            _lastRotationVelocity = {
                _rotationSpeed * axisRightX * elapsedTime.getSeconds<float>(),
                _rotationSpeed * axisRightY * elapsedTime.getSeconds<float>()
            };
            updateRotation();
        }
    }

    // TouchScreen
    {
        if (_eventSource->_touchScreenState.drag
            && _eventSource->_touchScreenState.state ==
               lug::Window::TouchScreenEvent::GestureState::Start) {
            _lastDragPosition = _eventSource->_touchScreenState.coordinates[0];
        } else if (_eventSource->_touchScreenState.drag &&
                   _eventSource->_touchScreenState.state ==
                   lug::Window::TouchScreenEvent::GestureState::Move) {
            _lastRotationVelocity = {
                    // TODO (gsabatie): Add a real touch coef
                    _rotationSpeed *
                    (_eventSource->_touchScreenState.coordinates[0].x() - _lastDragPosition.x()) /
                    100 * elapsedTime.getSeconds<float>(),
                    _rotationSpeed *
                    (_eventSource->_touchScreenState.coordinates[0].y() - _lastDragPosition.y()) /
                    100 * elapsedTime.getSeconds<float>()
            };

            updateRotation();
        } else if (_eventSource->_touchScreenState.drag
                   && _eventSource->_touchScreenState.state ==
                      lug::Window::TouchScreenEvent::GestureState::End) {
            _lastDragPosition = {0, 0};
        }
    }

    auto mousePos = _eventSource->getMousePos();
    if (_eventSource->isMousePressed(lug::Window::Mouse::Button::Left)) {

        // Only if the mouse moved since the last time
        if (_lastMousePos != mousePos) {
            lug::Math::Vec2i delta = mousePos - _lastMousePos;
            _lastMousePos = mousePos;
            _lastRotationVelocity = {
                _rotationSpeed * delta.x() * elapsedTime.getSeconds<float>(),
                _rotationSpeed * delta.y() * elapsedTime.getSeconds<float>()
            };
            updateRotation();

            lug::Math::Vec2i windowSize = _eventSource->getWindowSize();

            // If the mouse escapes a 30% zone on the edges of the screen, we reset-it on the middle
            if (mousePos.x() < windowSize.width() * 0.30 || mousePos.x() > windowSize.width() * 0.70
                || mousePos.y() < windowSize.height() * 0.30 || mousePos.y() > windowSize.height() * 0.70) {
                lug::Math::Vec2i middle{windowSize.width() / 2, windowSize.height() / 2};
                _eventSource->setMousePos(middle);
                _lastMousePos = middle;
            }
        }
        else {
            _lastRotationVelocity = {0.0f, 0.0f};
        }
    }

    // Rotate camera
    {
        float theta = lug::Math::Geometry::radians(-_rotation.x());
        float phi = lug::Math::Geometry::radians(_rotation.y());

        float z = _zoom * cos(theta) * cos(phi);
        float x = _zoom * sin(theta) * cos(phi);
        float y = _zoom * sin(phi);

        _target->setPosition({x, y, z}, lug::Graphics::Node::TransformSpace::World);
        _target->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
    }

    // Rotate model after mouse released or drag released
    if (_rotationVelocity.x() || _rotationVelocity.y()) {
        // Reduce rotation velocity by 0.05% each millisecond
        _rotationVelocity = _rotationVelocity * (1.0f - 0.005f * elapsedTime.getMilliseconds<float>());
        _lastRotationVelocity = _rotationVelocity;

        updateRotation();

        // Stop rotation if velocity is close to 0
        if (_rotationVelocity.x() < 0.01f &&
            _rotationVelocity.x() > -0.01f &&
            _rotationVelocity.y() < 0.01f &&
            _rotationVelocity.y() > -0.01f) {
            _rotationVelocity = {0.0f, 0.0f};
        }
    }

    _zoomUpdate = 0.0f;
}

void ModelViewer::onEvent(const lug::Window::Event& event) {
    // Zoom
    // Windows
    if (event.type == lug::Window::Event::Type::MouseWheel) {
        _zoomUpdate = static_cast<float>(event.mouse.scrollOffset.xOffset);
    }
    // Android
    if (event.touchScreen.pinch
        && event.touchScreen.state ==
           lug::Window::TouchScreenEvent::GestureState::Start) {
        _lastPinchGap = event.touchScreen.coordinates[1].x() -
                        event.touchScreen.coordinates[0].x() +
                        event.touchScreen.coordinates[1].y() -
                        event.touchScreen.coordinates[0].y();
    } else if (event.touchScreen.pinch &&
               event.touchScreen.state ==
               lug::Window::TouchScreenEvent::GestureState::Move) {
            _zoomUpdate = _lastPinchGap - (event.touchScreen.coordinates[1].x() -
                            event.touchScreen.coordinates[0].x() +
                            event.touchScreen.coordinates[1].y() -
                            event.touchScreen.coordinates[0].y());
            _zoomUpdate /= 100;
    } else if (event.touchScreen.pinch
               && _eventSource->_touchScreenState.state ==
                  lug::Window::TouchScreenEvent::GestureState::End) {
        _lastPinchGap = 0;
    }

    // Windows mouse button pressed/released
    if (event.type == lug::Window::Event::Type::ButtonPressed &&
        event.mouse.code == lug::Window::Mouse::Button::Left) {
        _lastMousePos = _eventSource->getMousePos();
        _eventSource->setMouseCursorVisible(false);
    }
    else if (event.type == lug::Window::Event::Type::ButtonReleased &&
        event.mouse.code == lug::Window::Mouse::Button::Left) {
        _eventSource->setMouseCursorVisible(true);
    }

    // Rotate the model after mouse/drag released
    if (isRotationEnd(event)) {
        // Rotate only on one axis or we get strange rotations
        if (std::abs(_lastRotationVelocity.x()) > std::abs(_lastRotationVelocity.y())) {
            _rotationVelocity = {_lastRotationVelocity.x() , 0.0f};
        }
        else {
            _rotationVelocity = {0.0f, _lastRotationVelocity.y()};
        }
    }
}

void ModelViewer::rotate(float x, float y) {
    lug::Math::Vec2f tmp = _lastRotationVelocity;
    _lastRotationVelocity = {x, y};
    updateRotation();
    _lastRotationVelocity = tmp;
}

bool ModelViewer::isRotationEnd(const lug::Window::Event& event) {
#if defined(LUG_SYSTEM_ANDROID)
    return event.touchScreen.drag &&
        event.touchScreen.state == lug::Window::TouchScreenEvent::GestureState::End;
#else
    return event.type == lug::Window::Event::Type::ButtonReleased &&
        event.mouse.code == lug::Window::Mouse::Button::Left;
#endif
}

void ModelViewer::updateRotation() {
    // Rotate in world space to freeze the rotation on the X axis
    _rotation.x() += _lastRotationVelocity.x() * 30.0f;
    _rotation.y() += _lastRotationVelocity.y() * 30.0f;

    // Prevent overflow/underflow
    if (_rotation.x() > 360.0f) {
        _rotation.x() -= 360.0f;
    }
    else if (_rotation.x() < 360.0f) {
        _rotation.x() += 360.0f;
    }

    // Lock rotation on top and bottom or it will reverse the camera
    if (_rotation.y() >= 89.0f) {
        _rotation.y() = 89.0f;
    }
    else if (_rotation.y() <= -89.0f) {
        _rotation.y() = -89.0f;
    }
}
