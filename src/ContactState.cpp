#include "ContactState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

#include "BenchmarkingState.hpp"

#include "ModelsState.hpp"
#include "ResultsState.hpp"
#include "InfoState.hpp"
#include "BenchmarksState.hpp"

#include <IconsFontAwesome.h>

ContactState::ContactState(LugBench::Application &application) : AState(application) {
    GUI::setDefaultStyle();
}

ContactState::~ContactState() {
}

bool ContactState::onPush() {
    return true;
}

bool ContactState::onPop() {
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
    _scene = nullptr;
    return true;
}

void ContactState::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
        _application.close();
    }
}

bool ContactState::onFrame(const lug::System::Time& /*elapsedTime*/) {

    _application.setCurrentState(State::CONTACT);

    GUI::displayMenu(_application);

    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_ShowBorders;

    lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

    float mainMenuHeight = static_cast<float>(window->getHeight()) / 18.f;

#if defined(LUG_SYSTEM_ANDROID)
    mainMenuHeight = (mainMenuHeight < 60.f * 2.f) ? 60.f * 2.f : mainMenuHeight;
#else
    mainMenuHeight = (mainMenuHeight < 60.f) ? 60.f : mainMenuHeight;
#endif

    ImGui::Begin("Contact Menu", &_isOpen, window_flags);
    {
        ImVec2 modelMenuSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) - (mainMenuHeight * 2)};

        ImGui::SetWindowSize(modelMenuSize);
        ImGui::SetWindowPos(ImVec2{ 0.f, mainMenuHeight });
    }
    ImGui::End();

    GUI::displayFooter(_application);

    return true;
}
