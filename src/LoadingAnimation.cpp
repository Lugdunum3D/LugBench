#include "LoadingAnimation.hpp"

#include <imgui.h>

#include "Application.hpp"
#include <lug/Graphics/Builder/Texture.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

bool LoadingAnimation::init(LugBench::Application& application, const lug::Math::Vec2f& size, const lug::Math::Vec2f& offset) {
    _application = &application;
    _size = size;
    _offset = offset;

    return true;
}

void LoadingAnimation::update(const lug::System::Time& elapsedTime) {
    if (!_display) {
        _progress = 0.0f;
        return;
    }

    lug::Graphics::Renderer* renderer = _application->getGraphics().getRenderer();
    lug::Graphics::Render::Window* window = renderer->getWindow();
    uint16_t windowHeight = window->getHeight();
    uint16_t windowWidth = window->getWidth();

    _progress += _speed * elapsedTime.getSeconds<float>();
    if (_progress >= 1.0f) {
        _progress = 0.0f;
    }

    ImGui::Begin("Progress window", 0, _application->_window_flags);
    {
       ImVec2 center {
            (windowWidth / 2) - (_size.x() / 2) + _offset.x(),
            (windowHeight / 2) - (_size.y() / 2) + _offset.y()
        };

        // Setup window
        ImGui::SetWindowSize({ _size.x(), _size.y() });
        ImGui::SetWindowPos(center);

        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, {0.36f, 0.74f, 1.0f, 1.0f});
        {
            ImGui::ProgressBar(_progress, ImVec2(_size.x(), _size.y()), "");
        }
        ImGui::PopStyleColor();
    }
    ImGui::End();
}

void LoadingAnimation::display(bool display) {
    _display = display;
}
