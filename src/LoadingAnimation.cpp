#include "LoadingAnimation.hpp"

#include <imgui.h>

#include "Application.hpp"
#include <lug/Graphics/Builder/Texture.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

bool LoadingAnimation::init(LugBench::Application& application, const std::string& loadingDotImage, const lug::Math::Vec2f& size, const lug::Math::Vec2f& offset) {
    _application = &application;
    _size = size;
    _offset = offset;


    lug::Graphics::Renderer* renderer = _application->getGraphics().getRenderer();
    lug::Graphics::Builder::Texture textureBuilder(*renderer);

    textureBuilder.addLayer(loadingDotImage);
    textureBuilder.setMinFilter(lug::Graphics::Render::Texture::Filter::Linear);
    textureBuilder.setMagFilter(lug::Graphics::Render::Texture::Filter::Linear);

    auto image = textureBuilder.build();
    if (!image) {
        LUG_LOG.error("Application: Can't create the loader texture");
        return false;
    }

    _loadingDot = lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(image);
    _loadingDots.resize(3, 0.0f);
    return true;
}

void LoadingAnimation::update(const lug::System::Time& elapsedTime) {
    if (!_display) {
        _progress = 0;
        return;
    }

    lug::Graphics::Renderer* renderer = _application->getGraphics().getRenderer();
    lug::Graphics::Render::Window* window = renderer->getWindow();
    uint16_t windowHeight = window->getHeight();
    uint16_t windowWidth = window->getWidth();

    _elapsedSecs += _speed * elapsedTime.getSeconds<float>();
    // Fade in
    if (_progress < 3) {
        _loadingDots[_progress] = _elapsedSecs / 1.0f;
    }
    // Fade out
    {
        if (_progress) {
            _loadingDots[_progress - 1] = 1.0f - (_elapsedSecs / 1.0f);
        }
        else if (_loadingDots[2]) {
            _loadingDots[2] = 1.0f - (_elapsedSecs / 1.0f);
        }
    }
    if (_elapsedSecs >= 1.0f) {
        _elapsedSecs = 0.0f;
        ++_progress;
    }
    if (_progress >= 3) {
        _progress = 0;
    }

    ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0f, 0.0f, 0.0f, 0.0f});
    ImGui::Begin("Loading window", 0, _application->_window_flags);
    {
        float dotsOffet{10.0f};
        ImVec2 center {
            (windowWidth / 2) + _offset.x(),
            (windowHeight / 2) + _offset.y()
        };
        // Center the 3 dots
        center.x -= ((_size.x() * 3) + (dotsOffet * 2)) / 2;

        // Setup window
        ImGui::SetWindowSize({ (_size.x() + dotsOffet) * 3, _size.y() });
        ImGui::SetWindowPos(center);

        {
            for (uint8_t i = 0; i < 3; ++i) {
                ImGui::Image(_loadingDot.get(), ImVec2(_size.x(), _size.y()), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, _loadingDots[i]));
                ImGui::SameLine();
                ImGui::SetCursorPosX((_size.x() + dotsOffet) * (i + 1));
            }
        }
    }
    ImGui::End();
    ImGui::PopStyleColor();
}

void LoadingAnimation::display(bool display) {
    _display = display;
}
