#pragma once

#include <lug/Graphics/Resource.hpp>
#include <lug/Graphics/Vulkan/Render/Texture.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <lug/Math/Vector.hpp>

namespace LugBench {
    class Application;
} // LugBench

class LoadingAnimation {
public:
    LoadingAnimation() = default;
    ~LoadingAnimation() = default;

    bool init(LugBench::Application& application, const lug::Math::Vec2f& size, const lug::Math::Vec2f& offset);
    void update(const lug::System::Time& elapsedTime);
    void display(bool display);

private:
    LugBench::Application* _application{nullptr};

    lug::Math::Vec2f _size{0.0f, 0.0f};
    lug::Math::Vec2f _offset{0.0f, 0.0f};
    bool _display{true};
    float _speed{2.0f};

    float _progress{0.0f};
};
