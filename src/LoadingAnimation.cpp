#include "LoadingAnimation.hpp"

#include <imgui.h>

#include "Application.hpp"
#include <lug/Graphics/Builder/Texture.hpp>
#include <lug/System/Logger/Logger.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

bool LoadingAnimation::init(LugBench::Application& application, const std::string& loaderImage, const lug::Math::Vec2f& size, const lug::Math::Vec2f& offset) {
    _application = &application;
    _size = size;
    _offset = offset;

    lug::Graphics::Renderer* renderer = _application->getGraphics().getRenderer();
    lug::Graphics::Builder::Texture textureBuilder(*renderer);

    textureBuilder.addLayer(loaderImage);
    textureBuilder.setMinFilter(lug::Graphics::Render::Texture::Filter::Linear);
    textureBuilder.setMagFilter(lug::Graphics::Render::Texture::Filter::Linear);

    auto image = textureBuilder.build();
    if (!image) {
        LUG_LOG.error("Application: Can't create the loader texture");
        return false;
    }

    _loaderImage = lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(image);
    return true;
}

void LoadingAnimation::update(const lug::System::Time& elapsedTime) {
    if (_loaderImage.get() == nullptr) {
        LUG_LOG.warn("LoadingAnimation::update: No image, call init()");
        return;
    }

    lug::Graphics::Renderer* renderer = _application->getGraphics().getRenderer();
    lug::Graphics::Render::Window* window = renderer->getWindow();
    uint16_t windowHeight = window->getHeight();
    uint16_t windowWidth = window->getWidth();

    _rotation += _speed * elapsedTime.getSeconds<float>();
    if (_rotation >= 360.0f) {
        _rotation = 0.0f;
    }

    //ImGui::PushStyleColor(ImGuiCol_WindowBg, {0.0f, 0.0f, 0.0f, 0.0f});
    {
        ImGui::Begin("Image window", 0, _application->_window_flags);
        {
           ImVec2 imageCenter {
                (windowWidth / 2) - (_size.x() / 2) + _offset.x(),
                (windowHeight / 2) - (_size.y() / 2) + _offset.y()
            };

            // Setup window
            ImGui::SetWindowSize({ _size.x(), _size.y() });
            ImGui::SetWindowPos(imageCenter);

            ImVec2 uv1 {
                (-0.5f * cos(lug::Math::Geometry::radians(_rotation))) - (-0.5f * sin(lug::Math::Geometry::radians(_rotation))) + 0.5f,
                (-0.5f * cos(lug::Math::Geometry::radians(_rotation))) + (-0.5f * sin(lug::Math::Geometry::radians(_rotation))) + 0.5f
            };
            ImVec2 uv2 {
                (0.5f * cos(lug::Math::Geometry::radians(_rotation))) - (0.5f * sin(lug::Math::Geometry::radians(_rotation))) + 0.5f,
                (0.5f * cos(lug::Math::Geometry::radians(_rotation))) + (0.5f * sin(lug::Math::Geometry::radians(_rotation))) + 0.5f
            };
            ImGui::Image(_loaderImage.get(), ImVec2(_size.x(), _size.y()), uv1, uv2, ImVec4(1, 1, 1, 1));
        }
        ImGui::End();
    }
    //ImGui::PopStyleColor();
}

void LoadingAnimation::display(bool display) {
    _display = display;
}
