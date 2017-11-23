#pragma once

#include "AState.hpp"
#include "Application.hpp"
#include "ModelViewer.hpp"
#include "LoadingAnimation.hpp"

#include <unordered_map>

#include <lug/Graphics/Scene/Scene.hpp>
#include <json/json.hpp>
#include <imgui.h>


class ModelsState : public AState {
private:
    struct ModelInfos {
        std::string name;
        std::string path;
        std::string modelNodeName;
        std::string skyboxName;
        lug::Math::Vec2f rotation{0.0f, 0.0f};
        lug::Graphics::Resource::SharedPtr<lug::Graphics::Resource> sceneResource{nullptr};
    };
    struct SkyBoxInfo{
        std::string backgroundFile;
        std::string environmentFile;
        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> resource{nullptr};
    };

private:
    static std::vector<ModelInfos> _models;
    static std::unordered_map<std::string, SkyBoxInfo> _skyBoxes;

public:
    ModelsState() = delete;
    ModelsState(LugBench::Application &application);
    ~ModelsState();

    void onEvent(const lug::Window::Event& event) override;
    bool onFrame(const lug::System::Time& elapsedTime) override;
    bool onPop() override;
    bool onPush() override;

private:
    void attachCameraToMover();
    bool loadModel(ModelInfos& model);
    bool loadModelSkyBox(
        const ModelInfos& model,
        lug::Graphics::Resource::SharedPtr<lug::Graphics::Resource> sceneResource,
        lug::Graphics::Renderer* renderer,
        bool displaySkyBox
    );
    void pushButtonsStyle(const ImVec4& color, const ImVec4& hoveredColor, const ImVec4& activeColor, const ImVec4& textColor) const;
    void handleResize();
    float getModelMenuWidth(float windowWidth);

    ModelViewer _cameraMover;

    bool _displaySkyBox{true};
    bool _displayFullscreen{false};

    const ModelInfos* _selectedModel{nullptr};
    bool _loadingModel{false};

    LoadingAnimation _loadingAnimation;
};
