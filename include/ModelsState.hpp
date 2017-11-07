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
    };
    struct SkyBoxInfo{
        std::string positiveXFile;
        std::string negativeXFile;
        std::string positiveYFile;
        std::string negativeYFile;
        std::string positiveZFile;
        std::string negativeZFile;
        lug::Graphics::Resource::SharedPtr<lug::Graphics::Render::SkyBox> resource{nullptr};
    };

public:
    ModelsState() = delete;
    ModelsState(LugBench::Application &application);
    ~ModelsState();

    void onEvent(const lug::Window::Event& event) override;
    bool onFrame(const lug::System::Time& elapsedTime) override;
    bool onPop() override;
    bool onPush() override;

    static float getModelMenuWidth(float windowWidth);

private:
    bool loadModel(const ModelInfos& model);
    bool loadModelSkyBox(const ModelInfos& model);
    void pushButtonsStyle(const ImVec4& color, const ImVec4& hoveredColor, const ImVec4& activeColor, const ImVec4& textColor) const;
    void handleResize();

    ModelViewer _cameraMover;

    std::vector<ModelInfos> _models;
    std::unordered_map<std::string, SkyBoxInfo> _skyBoxes;
    bool _displaySkyBox{true};
    bool _displayFullscreen{false};

    const ModelInfos* _selectedModel{nullptr};

    // Set to true when a model is loading to ignore onFrame/onEvent calls
    bool _lockCamera{true};

    LoadingAnimation _loadingAnimation;
};
