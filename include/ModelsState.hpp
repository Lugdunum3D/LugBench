#pragma once

#include "AState.hpp"
#include "Application.hpp"
#include "ModelViewer.hpp"

#include <lug/Graphics/Scene/Scene.hpp>
#include <json/json.hpp>
#include <imgui.h>

class ModelsState : public AState {
private:
    struct ModelInfos {
        std::string name;
        std::string path;
        std::string modelNodeName;
        lug::Math::Vec3f rotation{0.0f, 0.0f, 0.0f};
    };

public:
    ModelsState() = delete;
    ModelsState(LugBench::Application &application);
    ~ModelsState();

    void onEvent(const lug::Window::Event& event) override;
    bool onFrame(const lug::System::Time& elapsedTime) override;
    bool onPop() override;
    bool onPush() override;

private:
    bool loadModel(const ModelInfos& model);
    void pushButtonsStyle(const ImVec4& color, const ImVec4& hoveredColor, const ImVec4& activeColor, const ImVec4& textColor) const;
    float getModelMenuWidth(float windowWidth);
    void handleResize();

    ModelViewer _cameraMover;

    std::vector<ModelInfos> _models;
    const ModelInfos* _selectedModel{nullptr};
};
