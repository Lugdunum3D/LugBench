#pragma once

#include <cstdint>

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

    bool init(LugBench::Application& application, const std::string& loadingDotImage, const lug::Math::Vec2f& size, const lug::Math::Vec2f& offset);
    void update(const lug::System::Time& elapsedTime);
    void display(bool display);

private:
    LugBench::Application* _application{nullptr};
    lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture> _loadingDot{nullptr};

    lug::Math::Vec2f _size{0.0f, 0.0f};
    lug::Math::Vec2f _offset{0.0f, 0.0f};
    bool _display{false};
    float _speed{1.0f};

    uint8_t _progress{0};
    float _elapsedSecs{0.0f};

    std::vector<float> _loadingDots;
};
