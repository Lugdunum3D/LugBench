#include "ModelViewer.hpp"
#include "vecmath.h"
#include <lug/System/Logger/Logger.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>


void ModelViewer::onFrame(const lug::System::Time& elapsedTime) {
    if (_eventSource == nullptr) {
        LUG_LOG.warn("ModelViewer::onFrame: No event source, call setEventSource()");
        return;
    }

    tap_camera_.Update(elapsedTime.getMilliseconds<double>());

    // glm::quat rotation = glm::quat_cast(glm::make_mat4(tap_camera_.GetRotationMatrix().Ptr()));
    // //x, y, z, w;
    // lug::Math::Quatf rotm(rotation.x, rotation.y, rotation.z, rotation.w);

    // _target->setRotation(rotm, lug::Graphics::Node::TransformSpace::World);

    // glm::mat4 trans(glm::make_mat3(tap_camera_.GetTransformMatrix().Ptr()));

//    lug::Math::Mat3x3f p(*tap_camera_.GetTransformMatrix().Ptr());

    {

        // glm::mat4 transformation(glm::make_mat4(tap_camera_.GetTransformMatrix().Ptr())); // your transformation matrix.
        // glm::vec3 scale;
        // glm::quat rotation;
        // glm::vec3 translation;
        // glm::vec3 skew;
        // glm::vec4 perspective;
        // glm::decompose(transformation, scale, rotation, translation, skew, perspective);
        float* m = tap_camera_.GetTransformMatrix().Ptr();
        // _target->translate(lug::Math::Vec3f({m[3], m[7], m[11]}), lug::Graphics::Node::TransformSpace::World);
        LUG_LOG.info("POSITION 1 : {} {} {} {}", m[0], m[1], m[2], m[3]);
        LUG_LOG.info("POSITION 2 : {} {} {} {}", m[4], m[5], m[6], m[7]);
        LUG_LOG.info("POSITION 3 : {} {} {} {}", m[8], m[9], m[10], m[11]);
        LUG_LOG.info("POSITION 4 : {} {} {} {}", m[12], m[13], m[14], m[15]);

        _target->setPosition(lug::Math::Vec3f({m[12], m[13], m[14]}), lug::Graphics::Node::TransformSpace::World);
        _target->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
    }


    // _zoom += _zoomSpeed * _zoomUpdate * elapsedTime.getSeconds<float>();
    // if (_zoom < 1.0f) {
    //     _zoom = 1.0f;
    // }

    // // GamePad View
    // {
    //     float axisRightX = _eventSource->_gamePadState.axisRight.x();
    //     float axisRightY = _eventSource->_gamePadState.axisRight.y();

    //     if (axisRightX || axisRightY) {
    //         _lastRotationVelocity = {
    //             _rotationSpeed * axisRightX * elapsedTime.getSeconds<float>(),
    //             _rotationSpeed * axisRightY * elapsedTime.getSeconds<float>()
    //         };
    //         updateRotation();
    //     }
    // }

    // // TouchScreen
    // {
    //     if (_eventSource->_touchScreenState.drag
    //         && _eventSource->_touchScreenState.state ==
    //            lug::Window::TouchScreenEvent::GestureState::Start) {
    //         _lastDragPosition = _eventSource->_touchScreenState.coordinates[0];
    //     } else if (_eventSource->_touchScreenState.drag &&
    //                _eventSource->_touchScreenState.state ==
    //                lug::Window::TouchScreenEvent::GestureState::Move) {
    //         _lastRotationVelocity = {
    //                 // TODO (gsabatie): Add a real touch coef
    //                 _rotationSpeed *
    //                 (_eventSource->_touchScreenState.coordinates[0].x() - _lastDragPosition.x()) /
    //                 100 * elapsedTime.getSeconds<float>(),
    //                 _rotationSpeed *
    //                 (_eventSource->_touchScreenState.coordinates[0].y() - _lastDragPosition.y()) /
    //                 100 * elapsedTime.getSeconds<float>()
    //         };

    //         updateRotation();
    //     } else if (_eventSource->_touchScreenState.drag
    //                && _eventSource->_touchScreenState.state ==
    //                   lug::Window::TouchScreenEvent::GestureState::End) {
    //         _lastDragPosition = {0, 0};
    //     }
    // }

    // auto mousePos = _eventSource->getMousePos();
    // if (_isMousePressed) {

    //     // Only if the mouse moved since the last time
    //     if (_lastMousePos != mousePos) {
    //         lug::Math::Vec2i delta = mousePos - _lastMousePos;
    //         _lastMousePos = mousePos;
    //         _lastRotationVelocity = {
    //             _rotationSpeed * delta.x() * elapsedTime.getSeconds<float>(),
    //             _rotationSpeed * delta.y() * elapsedTime.getSeconds<float>()
    //         };
    //         updateRotation();

    //         lug::Math::Vec2i windowSize = _eventSource->getWindowSize();

    //         // If the mouse escapes a 30% zone on the edges of the screen, we reset-it on the middle
    //         if (mousePos.x() < windowSize.width() * 0.30 || mousePos.x() > windowSize.width() * 0.70
    //             || mousePos.y() < windowSize.height() * 0.30 || mousePos.y() > windowSize.height() * 0.70) {
    //             lug::Math::Vec2i middle{windowSize.width() / 2, windowSize.height() / 2};
    //             _eventSource->setMousePos(middle);
    //             _lastMousePos = middle;
    //         }
    //     }
    //     else {
    //         _lastRotationVelocity = {0.0f, 0.0f};
    //     }
    // }

    // // Rotate camera
    // {
    //     float theta = lug::Math::Geometry::radians(-_rotation.x());
    //     float phi = lug::Math::Geometry::radians(_rotation.y());

    //     float z = _zoom * cos(theta) * cos(phi);
    //     float x = _zoom * sin(theta) * cos(phi);
    //     float y = _zoom * sin(phi);

    //     _target->setPosition({x, y, z}, lug::Graphics::Node::TransformSpace::World);
    //     _target->getCamera()->lookAt({0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, lug::Graphics::Node::TransformSpace::World);
    // }

    // // Rotate model after mouse released or drag released
    // if (_rotationVelocity.x() || _rotationVelocity.y()) {
    //     // Reduce rotation velocity by 0.05% each millisecond
    //     _rotationVelocity = _rotationVelocity * (1.0f - 0.005f * elapsedTime.getMilliseconds<float>());
    //     _lastRotationVelocity = _rotationVelocity;

    //     updateRotation();

    //     // Stop rotation if velocity is close to 0
    //     if (_rotationVelocity.x() < 0.01f &&
    //         _rotationVelocity.x() > -0.01f &&
    //         _rotationVelocity.y() < 0.01f &&
    //         _rotationVelocity.y() > -0.01f) {
    //         _rotationVelocity = {0.0f, 0.0f};
    //     }
    // }

    // _zoomUpdate = 0.0f;
}

void ModelViewer::TransformPosition(ndk_helper::Vec2& vec) {
  vec = ndk_helper::Vec2(2.0f, 2.0f) * vec /
              ndk_helper::Vec2(_eventSource->getWindowSize().width(),
                _eventSource->getWindowSize().height()) -
        ndk_helper::Vec2(1.f, 1.f);
}

void ModelViewer::onEvent(const lug::Window::Event& event) {
    // Android
    if (event.touchScreen.doubleTap == true) {
        LUG_LOG.info("event.touchScreen.doubleTap == true");
        // reset camera to initial state
        tap_camera_.Reset(true);
    }
    if (event.touchScreen.drag
        && event.touchScreen.state ==
           lug::Window::TouchScreenEvent::GestureState::Start) {

        LUG_LOG.info("drag start");
        ndk_helper::Vec2 v(event.touchScreen.coordinates[0].x(), event.touchScreen.coordinates[0].y());
        TransformPosition(v);
        tap_camera_.BeginDrag(v);

    } else if (event.touchScreen.drag &&
               event.touchScreen.state ==
               lug::Window::TouchScreenEvent::GestureState::Move) {

        LUG_LOG.info("drag Move");

        ndk_helper::Vec2 v(event.touchScreen.coordinates[0].x(), event.touchScreen.coordinates[0].y());
        LUG_LOG.info("v1 x : {} y : {}", event.touchScreen.coordinates[0].x(), event.touchScreen.coordinates[0].y());

        TransformPosition(v);
        tap_camera_.Drag(v);

    } else if (event.touchScreen.drag
               && _eventSource->_touchScreenState.state ==
                  lug::Window::TouchScreenEvent::GestureState::End) {
        LUG_LOG.info("drag End");
        tap_camera_.EndDrag();
       // _lastPinchGap = 0;
    }
    if (event.touchScreen.pinch
        && event.touchScreen.state ==
            lug::Window::TouchScreenEvent::GestureState::Start) {

        LUG_LOG.info("pinch Start");
        ndk_helper::Vec2 v1(event.touchScreen.coordinates[0].x(), event.touchScreen.coordinates[0].y());
        ndk_helper::Vec2 v2(event.touchScreen.coordinates[1].x(), event.touchScreen.coordinates[1].y());
        TransformPosition(v1);
        TransformPosition(v2);
        tap_camera_.BeginPinch(v1, v2);
    } else if (event.touchScreen.pinch
        && event.touchScreen.state ==
            lug::Window::TouchScreenEvent::GestureState::Move) {
        LUG_LOG.info("pinch Move");

        ndk_helper::Vec2 v1(event.touchScreen.coordinates[0].x(), event.touchScreen.coordinates[0].y());
        ndk_helper::Vec2 v2(event.touchScreen.coordinates[1].x(), event.touchScreen.coordinates[1].y());

        TransformPosition(v1);
        TransformPosition(v2);
        tap_camera_.Pinch(v1, v2);
    }

    // const lug::Math::Quatf rotm(*tap_camera_.GetRotationMatrix().Ptr());

    // // Windows mouse button pressed/released
    // if (event.type == lug::Window::Event::Type::ButtonPressed &&
    //     event.mouse.code == lug::Window::Mouse::Button::Left) {
    //     _isMousePressed = true;
    //     _lastMousePos = _eventSource->getMousePos();
    //     _eventSource->setMouseCursorVisible(false);
    // }
    // else if (event.type == lug::Window::Event::Type::ButtonReleased &&
    //     event.mouse.code == lug::Window::Mouse::Button::Left) {
    //     _isMousePressed = false;
    //     _eventSource->setMouseCursorVisible(true);
    // }

    // // Rotate the model after mouse/drag released
    // if (isRotationEnd(event)) {
    //     // Rotate only on one axis or we get strange rotations
    //     if (std::abs(_lastRotationVelocity.x()) > std::abs(_lastRotationVelocity.y())) {
    //         _rotationVelocity = {_lastRotationVelocity.x() , 0.0f};
    //     }
    //     else {
    //         _rotationVelocity = {0.0f, _lastRotationVelocity.y()};
    //     }
    // }
}

//void ModelViewer::setRotation(const lug::Math::Vec2f& rotation) {
//    _lastRotationVelocity = {0, 0};
//    _rotation.x() = rotation.x();
//    _rotation.y() = rotation.y();
//}

// bool ModelViewer::isRotationEnd(const lug::Window::Event& event) {
// #if defined(LUG_SYSTEM_ANDROID)
//     return event.touchScreen.drag &&
//         event.touchScreen.state == lug::Window::TouchScreenEvent::GestureState::End;
// #else
//     return event.type == lug::Window::Event::Type::ButtonReleased &&
//         event.mouse.code == lug::Window::Mouse::Button::Left;
// #endif
// }

// void ModelViewer::updateRotation() {
//     // Rotate in world space to freeze the rotation on the X axis
//     _rotation.x() += _lastRotationVelocity.x() * 30.0f;
//     _rotation.y() += _lastRotationVelocity.y() * 30.0f;

//     // Prevent overflow/underflow
//     if (_rotation.x() > 360.0f) {
//         _rotation.x() -= 360.0f;
//     }
//     else if (_rotation.x() < 360.0f) {
//         _rotation.x() += 360.0f;
//     }

//     // Lock rotation on top and bottom or it will reverse the camera
//     if (_rotation.y() >= 89.0f) {
//         _rotation.y() = 89.0f;
//     }
//     else if (_rotation.y() <= -89.0f) {
//         _rotation.y() = -89.0f;
//     }
// }
