#include "ModelViewer.hpp"
#include <lug/System/Logger/Logger.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

void ModelViewer::onFrame(const lug::System::Time& elapsedTime) {
    if (_eventSource == nullptr) {
        LUG_LOG.warn("ModelViewer::onFrame: No event source, call setEventSource()");
        return;
    }

    // GamePad Movement
    {
        float axisLeftX = _eventSource->_gamePadState.axisLeft.x();
        float axisLeftY = _eventSource->_gamePadState.axisLeft.y();

        if (axisLeftX) {
            _target->translate({ ((axisLeftX > 0) ? 1 : -1) * _speed * 0.5f * elapsedTime.getMilliseconds<float>(), 0.0f, 0.0f});
        }
        if (axisLeftY) {
            _target->translate({0.0f, 0.0f, ((axisLeftY > 0) ? 1 : -1) * _speed * 0.5f * elapsedTime.getMilliseconds<float>()});
        }
    }
    // GamePad View
    {
        float axisRightX = _eventSource->_gamePadState.axisRight.x();
        float axisRightY = _eventSource->_gamePadState.axisRight.y();

        if (axisRightX || axisRightY) {
            _target->rotate(-_speed * axisRightX, {0, 1, 0}, lug::Graphics::Node::TransformSpace::World);
            _target->rotate(-_speed * axisRightY, {1, 0, 0});
        }
    }
    // TouchScreen
    {
        if (_eventSource->_touchScreenState.drag) {
            _target->rotate(-_speed * _eventSource->_touchScreenState.coordinates[0].x(), {0, 1, 0}, lug::Graphics::Node::TransformSpace::World);
            _target->rotate(-_speed * _eventSource->_touchScreenState.coordinates[0].y(), {1, 0, 0});
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

            _rotation.x() += _speed * -delta.x() * 10.0f;
            _rotation.y() += _speed * -delta.y() * 10.0f;

            float x = 4.0f * (cos(lug::Math::Geometry::radians(-_rotation.x())) * sin(lug::Math::Geometry::radians(_rotation.y())));
            float y = 4.0f * (sin(lug::Math::Geometry::radians(-_rotation.x())) * sin(lug::Math::Geometry::radians(_rotation.y())));
            float z = 4.0f * cos(lug::Math::Geometry::radians(_rotation.y()));

            _target->setPosition({x, z, y}, lug::Graphics::Node::TransformSpace::World);
            _target->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);


            lug::Math::Vec2i windowSize = _eventSource->getWindowSize();

            // If the mouse escapes a 30% zone on the edges of the screen, we reset-it on the middle
            if (mousePos.x() < windowSize.width() * 0.30 || mousePos.x() > windowSize.width() * 0.70
                || mousePos.y() < windowSize.height() * 0.30 || mousePos.y() > windowSize.height() * 0.70) {
                lug::Math::Vec2i middle{windowSize.width() / 2, windowSize.height() / 2};
                _eventSource->setMousePos(middle);
                _lastMousePos = middle;
            }
        }
    }

    if (_scroll) {
        _target->translate({0.0f, 0.0f, _speed * _scroll * elapsedTime.getMilliseconds<float>() * 4.0f});
    }

    _scroll = 0.0f;
}

void ModelViewer::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::MouseWheel) {
        _scroll = static_cast<float>(event.mouse.scrollOffset.xOffset);
    }
}
