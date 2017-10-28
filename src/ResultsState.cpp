#include "ResultsState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

#include "BenchmarkingState.hpp"

#include "ModelsState.hpp"
#include "ContactState.hpp"
#include "InfoState.hpp"
#include "BenchmarksState.hpp"


#include <IconsFontAwesome.h>

ResultsState::ResultsState(LugBench::Application &application) : AState(application) {
    GUI::setDefaultStyle();
}

ResultsState::~ResultsState() {
}

bool ResultsState::onPush() {
    _application.setCurrentState(State::RESULTS);
    return true;
}

bool ResultsState::onPop() {
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
    _scene = nullptr;
    return true;
}

void ResultsState::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
        _application.close();
    }
}

bool ResultsState::onFrame(const lug::System::Time&) {

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
        ImVec2 modelMenuSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) - (mainMenuHeight * 2) };

        ImGui::SetWindowSize(modelMenuSize);
        ImGui::SetWindowPos(ImVec2{ 0.f, mainMenuHeight });        
    }
    ImGui::End();


    // if (_application.isSending()) {
    //     _sending_log_timer = 1.f;
    //     _sending_end_log_timer = 0.f;
    //     _display_sending_screen = true;
    // }
    // else if (_display_sending_screen == true) {
    //     _sending_log_timer = 2.f;
    //     _sending_end_log_timer = 3.f;
    //     _display_sending_screen = false;
    // }

    // if (_sending_log_timer > 0.f) {
    //     ImGui::Begin("Send Display", &_isOpen, window_flags);
    //     {
    //         //            lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

    //         //            // Sets the window to be at the bottom of the screen (1/3rd of the height)
    //         //            ImVec2 windowSize{ static_cast<float>(window->getWidth()), 30.f };
    //         //            ImVec2 windowPos = { 0, 0 };
    //         //            ImGui::SetWindowSize(windowSize);
    //         //            ImGui::SetWindowPos(windowPos);
    //         //            // Centers the button and keeps it square at all times
    //         //            ImVec2 buttonSize{ windowSize.x - 10.0f , windowSize.y - 10.0f };        
    //         //            ImGui::Button("Sending data in progress...", buttonSize);
    //     }
    //     ImGui::End();
    //     _sending_log_timer -= elapsedTime.getSeconds();
    // }
    // if (_sending_end_log_timer > 0.f) {
    //     ImGui::Begin("Send End Display", &_isOpen, window_flags);
    //     {
    //         //            lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();

    //         // Sets the window to be at the bottom of the screen (1/3rd of the height)
    //         float height = 0;
    //         if (_sending_log_timer > 0.f) {
    //             height += 25.f;
    //         }
    //         //            ImVec2 windowSize{ static_cast<float>(window->getWidth()), 30.f };
    //         //            ImVec2 windowPos = { 0, height };
    //         //            ImGui::SetWindowSize(windowSize);
    //         //            ImGui::SetWindowPos(windowPos);
    //         //            // Centers the button and keeps it square at all times
    //         //            ImVec2 buttonSize{ windowSize.x - 10.0f , windowSize.y - 10.0f };
    //         //            ImGui::Button("Sending data completed!", buttonSize);
    //     }
    //     ImGui::End();
    //     _sending_end_log_timer -= elapsedTime.getSeconds();
    // }


    // if (_display_info_screen == false && _display_result_screen == false) {
            // {
            //     bool displayResults = true;
            //     if (_devices.size() == 0 && !_isReceiving) {
            //         _isReceiving = true;
            //         LugBench::LugNetwork::getInstance().makeRequest(LugBench::Method::GET,
            //             LugBench::LugNetwork::urlToString(LugBench::Route::getScores));
            //         displayResults = false;
            //     }
            //     if (_isReceiving && LugBench::LugNetwork::getInstance().getMutex().try_lock()) {
            //         _isReceiving = false;
            //         nlohmann::json response = nlohmann::json::parse(
            //             LugBench::LugNetwork::getInstance().getLastRequestBody());
            //         _devices = response["data"];
            //         LugBench::LugNetwork::getInstance().getMutex().unlock();
            //         displayResults = false;
            //     }

            //     if (displayResults == true) {
            //         // Sets the window to be at the bottom of the screen (1/3rd of the height)
            //         ImVec2 windowSize{ static_cast<float>(window->getWidth()), static_cast<float>(window->getHeight()) };
            //         windowSize.x -= 30.f;
            //         windowSize.y -= headerHeight;

            //         ImGui::SetCursorPos(ImVec2{ 15.f, headerHeight });

            //         ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
            //         {
            //             ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(1.f, 1.f, 1.f, 1.00f));
            //             ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 0.f, 0.f, 1.00f));
            //             ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(.31f, .67f, .98f, 1.00f));
            //             ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(.88f, .88f, .88f, 1.00f));
            //             {
            //                 ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
            //                 {
            //                     ImGui::BeginChild("Result", windowSize);
            //                     {
            //                         size_t deviceCount = _devices.size();
            //                         if (deviceCount > 0) {
            //                             //			GUI::displayScoreInCell(_physicalDeviceInfo->properties.deviceName, 68.7f, 0.7f);
            //                             nlohmann::json highestScoreDevice = _devices[0];
            //                             float biggestStore = highestScoreDevice["averageFps"].get<float>();
            //                             GUI::displayScoreInCell(_physicalDeviceInfo->properties.deviceName, 68.7f, 0.0f);

            //                             ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
            //                             {
            //                                 ImGui::BeginChild("Score list");
            //                                 {
            //                                     for (uint32_t i = 0; i < deviceCount; i++) {
            //                                         ImGui::PushID(i);
            //                                         nlohmann::json device = _devices[i];
            //                                         GUI::displayScoreInCell(device["device"].get<std::string>().c_str(), 68.7f, biggestStore / device["averageFps"].get<float>());
            //                                         //				GUI::displayScoreInCell(_physicalDeviceInfo->properties.deviceName, 68.7f, 1.0f - static_cast<float>(i) / 10.0f);
            //                                         ImGui::PopID();
            //                                     }
            //                                 }
            //                                 ImGui::EndChild();
            //                             }
            //                             ImGui::PopFont();
            //                         }
            //                     }
            //                     ImGui::EndChild();
            //                 }
            //                 ImGui::PopFont();
            //             }
            //             ImGui::PopStyleColor(4);
            //         }
            //         ImGui::PopStyleVar();
            //     }
            // }
    //}

    GUI::displayFooter(_application);

    return true;
}
