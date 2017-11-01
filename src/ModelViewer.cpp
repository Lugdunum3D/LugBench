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
            _model->rotate(_speed * axisRightX, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
            _model->rotate(_speed * axisRightY, {1.0f, 0.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
        }
    }
    // TouchScreen
    {
        if (_eventSource->_touchScreenState.drag) {
            _model->rotate(_speed * _eventSource->_touchScreenState.coordinates[0].x(), {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
            _model->rotate(_speed * _eventSource->_touchScreenState.coordinates[0].y(), {1.0f, 0.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
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

            _model->rotate(_speed * delta.x(), {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
            _model->rotate(_speed * delta.y(), {1.0f, 0.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);

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

    if (_zoom) {
        _camera->translate({0.0f, 0.0f, _speed * _zoom * elapsedTime.getMilliseconds<float>() * 4.0f});
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
}
