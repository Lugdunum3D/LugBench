#include "ModelViewer.hpp"
#include <lug/System/Logger/Logger.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

void ModelViewer::onFrame(const lug::System::Time& elapsedTime) {
    if (_eventSource == nullptr) {
        LUG_LOG.warn("ModelViewer::onFrame: No event source, call setEventSource()");
        return;
    }

    // GamePad View
    {
        float axisRightX = _eventSource->_gamePadState.axisRight.x();
        float axisRightY = _eventSource->_gamePadState.axisRight.y();

        if (axisRightX || axisRightY) {
            _model->rotate(
                _rotationSpeed * axisRightX * elapsedTime.getSeconds<float>(),
                {0.0f, 1.0f, 0.0f},
                lug::Graphics::Node::TransformSpace::World
            );
            _model->rotate(
                _rotationSpeed * axisRightY * elapsedTime.getSeconds<float>(),
                {1.0f, 0.0f, 0.0f},
                lug::Graphics::Node::TransformSpace::World
            );
        }
    }
    // TouchScreen
    {
        if (_eventSource->_touchScreenState.drag) {
            _model->rotate(
                _rotationSpeed * _eventSource->_touchScreenState.coordinates[0].x() * elapsedTime.getSeconds<float>(),
                {0.0f, 1.0f, 0.0f},
                lug::Graphics::Node::TransformSpace::World
            );
            _model->rotate(_rotationSpeed * _eventSource->_touchScreenState.coordinates[0].y() * elapsedTime.getSeconds<float>(),
                {1.0f, 0.0f, 0.0f},
                lug::Graphics::Node::TransformSpace::World
            );
        }
    }

    // Capture / Release the mouse cursor
    if (_eventSource->isKeyPressed(lug::Window::Keyboard::Key::C) && !_hasFocus) {
        _lastMousePos = _eventSource->getMousePos();
        _hasFocus = true;
        _eventSource->setMouseCursorVisible(false);
    }
    if (_eventSource->isKeyPressed(lug::Window::Keyboard::Key::LAlt) && _hasFocus) {
        _hasFocus = false;
        _eventSource->setMouseCursorVisible(true);
    }

    auto mousePos = _eventSource->getMousePos();
    if (_hasFocus && _eventSource->isMousePressed(lug::Window::Mouse::Button::Left)) {

        // Only if the mouse moved since the last time
        if (_lastMousePos != mousePos) {
            lug::Math::Vec2i delta = mousePos - _lastMousePos;
            _lastMousePos = mousePos;
            _lastRotationVelocity = {
                _rotationSpeed * delta.x() * elapsedTime.getSeconds<float>(),
                _rotationSpeed * delta.y() * elapsedTime.getSeconds<float>()
            };

            // Rotate in world space to freeze the rotation on the X axis
            _model->rotate(_lastRotationVelocity.x(), {0.0f, 1.0f, 0.0f});
            _model->rotate(_lastRotationVelocity.y(), {1.0f, 0.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);

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

    if (_zoom) {
        _camera->translate({0.0f, 0.0f, _zoomSpeed * _zoom * elapsedTime.getSeconds<float>() * 4.0f});
    }
    // Rotate model after mouse released or drag released
    else if (_rotationVelocity.x() || _rotationVelocity.y()) {
        // Rotate in world space to freeze the rotation on the X axis
        _model->rotate(_rotationVelocity.x(), {0.0f, 1.0f, 0.0f});
        _model->rotate(_rotationVelocity.y(), {1.0f, 0.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);

        // Reduce rotation velocity by 0.05% each millisecond
        _rotationVelocity = _rotationVelocity * (1.0f - 0.005f * elapsedTime.getMilliseconds<float>());

        // Stop rotation if velocity is close to 0
        if (_rotationVelocity.x() < 0.01f &&
            _rotationVelocity.x() > -0.01f &&
            _rotationVelocity.y() < 0.01f &&
            _rotationVelocity.y() > -0.01f) {
            _rotationVelocity = {0.0f, 0.0f};
        }
    }

    _zoom = 0.0f;
}

void ModelViewer::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::MouseWheel) {
        _zoom = static_cast<float>(event.mouse.scrollOffset.xOffset);
    }
    else if (event.type == lug::Window::Event::Type::ButtonPressed &&
        event.mouse.code == lug::Window::Mouse::Button::Left) {
        _lastMousePos = _eventSource->getMousePos();
    }
    else if (isRotationEnd(event)) {
        // Rotate only on one axis or we get strange rotations
        if (std::abs(_lastRotationVelocity.x()) > std::abs(_lastRotationVelocity.y())) {
            _rotationVelocity = {_lastRotationVelocity.x(), 0.0f};
        }
        else {
            _rotationVelocity = {0.0f, _lastRotationVelocity.y()};
        }
    }
}

bool ModelViewer::isRotationEnd(const lug::Window::Event& event) {
    return (event.type == lug::Window::Event::Type::ButtonReleased &&
        event.mouse.code == lug::Window::Mouse::Button::Left);
}
