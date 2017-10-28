#include "InfoState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Math/Geometry/Trigonometry.hpp>

#include "BenchmarkingState.hpp"

#include "ModelsState.hpp"
#include "ContactState.hpp"
#include "ResultsState.hpp"
#include "BenchmarksState.hpp"


#include <IconsFontAwesome.h>

InfoState::InfoState(LugBench::Application &application) : AState(application) {
    GUI::setDefaultStyle();
}

InfoState::~InfoState() {
}

bool InfoState::onPush() {
    _application.setCurrentState(State::INFO);

    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);

    _physicalDeviceInfo = vkRender->getPhysicalDeviceInfo();

    if (!_physicalDeviceInfo) {
        return false;
    }
    return true;
}

bool InfoState::onPop() {
    lug::Graphics::Renderer* renderer = _application.getGraphics().getRenderer();
    lug::Graphics::Vulkan::Renderer* vkRender = static_cast<lug::Graphics::Vulkan::Renderer*>(renderer);
    vkRender->getDevice().waitIdle();
    _scene = nullptr;
    return true;
}

void InfoState::onEvent(const lug::Window::Event& event) {
    if (event.type == lug::Window::Event::Type::Close) {
        _application.close();
    }
}

bool InfoState::onFrame(const lug::System::Time& /*elapsedTime*/) {
    lug::Graphics::Render::Window* window = _application.getGraphics().getRenderer()->getWindow();
    uint16_t windowHeight = window->getHeight();
    uint16_t windowWidth = window->getWidth();
    float widowHeightOffset = GUI::displayMenu(_application);

    ImVec2 infoWindowSize{ GUI::Utilities::getPercentage(windowWidth, 0.35f, 300.f), windowHeight - (widowHeightOffset * 2) };

    ImGui::Begin("Info Window", 0, _application._window_flags);
    {
        ImGui::SetWindowSize(infoWindowSize);
        ImGui::SetWindowPos(ImVec2{ 0.f, widowHeightOffset });

#if defined(LUG_SYSTEM_ANDROID)
        ImVec2 deviceWindowSize{ GUI::Utilities::getPercentage(ImGui::GetWindowWidth(), 0.9f), 250.f };
#else
        ImVec2 deviceWindowSize{ GUI::Utilities::getPercentage(ImGui::GetWindowWidth(), 0.9f), 150.f };
#endif
        float padding = (ImGui::GetWindowWidth() - deviceWindowSize.x) / 2.f;
        ImGui::SetCursorPos(ImVec2{ padding , 20.f });
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 5.f, 2.f });
            {
                ImGui::BeginChild("Device Window", deviceWindowSize);
                {
                    ImGui::SetCursorPos(ImVec2{ 20.f , 0.f });
                    ImGui::BeginChild("Device Info");
                    {
                        ImGui::Text("Device");
                        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
                        {
                            ImGui::Text("Name: Geforce gtx 960M");
                            ImGui::Text("Driver Version: 385.164.0");
                            ImGui::Text("API Version: 41953667");
                        }
                        ImGui::PopFont();
                    }
                    ImGui::EndChild();
                }
                ImGui::EndChild();

                ImVec2 childWindowHeaderSize;
#if defined(LUG_SYSTEM_ANDROID)
                const float childHeaderHeight = 70.f;
                const float childHeight = 400.f;
                ImGui::SetCursorPos(ImVec2{ padding , 310.f });
#else
                const float childHeaderHeight = 40.f;
                const float childHeight = 300.f;
                ImGui::SetCursorPos(ImVec2{ padding , 190.f });
#endif

                childWindowHeaderSize = ImVec2{ deviceWindowSize.x, childHeaderHeight };

                ImVec2 childWindowSize{ childWindowHeaderSize.x , childHeight };

                ImGui::BeginGroup();
                {
                    // First window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(.31f, .67f, .98f, 1.00f));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Title Main", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("DEVICE");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Info Main", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::Text("Plop");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();
                    }
                    ImGui::PopFont();
                }
                ImGui::EndGroup();
            }
            ImGui::PopStyleVar(2);
        }
        ImGui::PopStyleColor(2);
    }
    ImGui::End();

    ImGui::Begin("Info Extra Window", 0, _application._window_flags);
    {
        ImVec2 infoExtraWindowSize{ windowWidth - infoWindowSize.x, infoWindowSize.y };

        ImGui::SetWindowSize(infoExtraWindowSize);
        ImGui::SetWindowPos(ImVec2{ infoWindowSize.x, widowHeightOffset });

        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.33f, 0.33f, 0.33f, 1.00f));
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 5.f, 2.f });
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });
            {
                ImVec2 uiSize = ImGui::GetWindowSize();
                ImVec2 childWindowHeaderSize;
#if defined(LUG_SYSTEM_ANDROID)
                const float childMinWidth = 1000.f;
                const float childHeaderHeight = 70.f;
                const float childHeight = 400.f;
                const float childSpacing = 30.f;
                //const float minSizeDoubleElements = (childMinWidth * 2.f) + (childSpacing * 3.f);
#else
                const float childMinWidth = 405.f;
                const float childHeaderHeight = 40.f;
                const float childHeight = 400.f;
                const float childSpacing = 20.f;
                const float minSizeDoubleElements = (childMinWidth * 2.f) + (childSpacing * 3.f);
#endif

#if defined(LUG_SYSTEM_ANDROID)
                {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, 20.f });
                }
#else

                if (uiSize.x >= minSizeDoubleElements) {
                    childWindowHeaderSize = ImVec2{ GUI::Utilities::getPercentage(uiSize.x - (childSpacing * 3.f), 0.5f, childMinWidth), childHeaderHeight };
                    ImGui::SetCursorPos(ImVec2{ childSpacing, 20.f });
                }
                else {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, 20.f });
                }
#endif

                ImVec2 childWindowSize{ childWindowHeaderSize.x, childHeight };

                ImGui::BeginGroup();
                {
                    // First window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(.31f, .67f, .98f, 1.00f));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Title 1", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("DEVICE");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Info 1", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                static bool displayExtraDeviceInfo = false;

                                if (displayExtraDeviceInfo == false) {
                                    if (ImGui::Button(ICON_FA_PLUS_SQUARE)) {
                                        displayExtraDeviceInfo = true;
                                    }
                                }
                                else {
                                    if (ImGui::Button(ICON_FA_MINUS_SQUARE)) {
                                        displayExtraDeviceInfo = false;
                                    }
                                }
                                GUI::displayConfigInfoString("Name", _physicalDeviceInfo->properties.deviceName);
                                GUI::displayConfigInfoVersion("Driver Version", lug::Core::Version::fromInt(_physicalDeviceInfo->properties.driverVersion));
                                GUI::displayConfigInfoValue("API Version", lug::Core::Version::fromInt(_physicalDeviceInfo->properties.apiVersion));
                                if (displayExtraDeviceInfo == true) {
                                    GUI::displayConfigInfoValue("Device ID", _physicalDeviceInfo->properties.deviceID);
                                    GUI::displayConfigInfoValue("Vendor ID", _physicalDeviceInfo->properties.vendorID);
                                    GUI::displayConfigInfoString("Device Type", lug::Graphics::Vulkan::API::RTTI::toStr(_physicalDeviceInfo->properties.deviceType));
                                    GUI::displayConfigInfoArrayUint8("Pipeline Cache UUID", std::vector<uint8_t>(std::begin(_physicalDeviceInfo->properties.pipelineCacheUUID), std::end(_physicalDeviceInfo->properties.pipelineCacheUUID)));
                                    if (ImGui::CollapsingHeader("Limits")) {
                                        ImGui::Indent();
                                        {
                                            GUI::displayDeviceLimits(_physicalDeviceInfo);
                                        }
                                        ImGui::Unindent();
                                    }
                                    if (ImGui::CollapsingHeader("Sparse Properties")) {
                                        ImGui::Indent();
                                        {
                                            GUI::displayConfigInfoBool("Residency Standard 2D Block Shape", _physicalDeviceInfo->properties.sparseProperties.residencyStandard2DBlockShape);
                                            GUI::displayConfigInfoBool("Residency Standard 2D Multisample Block Shape", _physicalDeviceInfo->properties.sparseProperties.residencyStandard2DMultisampleBlockShape);
                                            GUI::displayConfigInfoBool("Residency Standard 3D Block Shape", _physicalDeviceInfo->properties.sparseProperties.residencyStandard3DBlockShape);
                                            GUI::displayConfigInfoBool("Residency Aligned Mip Size", _physicalDeviceInfo->properties.sparseProperties.residencyAlignedMipSize);
                                            GUI::displayConfigInfoBool("Residency Non Resident Strict", _physicalDeviceInfo->properties.sparseProperties.residencyNonResidentStrict);

                                        }
                                        ImGui::Unindent();
                                    }
                                }
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();
                    }
                    ImGui::PopFont();
                }
                ImGui::EndGroup();

#if defined(LUG_SYSTEM_ANDROID)
                {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, childHeaderHeight + childHeight + (childSpacing * 2.f) });
                }
#else
                if (uiSize.x >= minSizeDoubleElements) {
                    childWindowHeaderSize = ImVec2{ GUI::Utilities::getPercentage(uiSize.x - (childSpacing * 3.f), 0.5f, childMinWidth), childHeaderHeight };
                    ImGui::SetCursorPos(ImVec2{ childWindowHeaderSize.x + (childSpacing * 2.f), 20.f });
                }
                else {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, childHeaderHeight + childHeight + (childSpacing * 2.f) });
                }
#endif

                ImGui::BeginGroup();
                {
                    // First window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(.31f, .67f, .98f, 1.00f));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Title 2", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("DEVICE");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Info 2", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::Text("Plop");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();
                    }
                    ImGui::PopFont();
                }
                ImGui::EndGroup();

#if defined(LUG_SYSTEM_ANDROID)
                {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, ((childHeaderHeight + childHeight) * 2.f) + (childSpacing * 3.f) });
                }
#else
                if (uiSize.x >= minSizeDoubleElements) {
                    childWindowHeaderSize = ImVec2{ GUI::Utilities::getPercentage(uiSize.x - (childSpacing * 3.f), 0.5f, childMinWidth), childHeaderHeight };
                    ImGui::SetCursorPos(ImVec2{ childSpacing, childHeaderHeight + childHeight + (childSpacing * 2.f) });
                }
                else {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, ((childHeaderHeight + childHeight) * 2.f) + (childSpacing * 3.f) });
                }
#endif

                ImGui::BeginGroup();
                {
                    // First window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(.31f, .67f, .98f, 1.00f));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Title 3", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("DEVICE");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Info 3", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::Text("Plop");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();
                    }
                    ImGui::PopFont();
                }
                ImGui::EndGroup();

#if defined(LUG_SYSTEM_ANDROID)
                {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, ((childHeaderHeight + childHeight) * 3.f) + (childSpacing * 4.f) });
                }
#else
                if (uiSize.x >= minSizeDoubleElements) {
                    childWindowHeaderSize = ImVec2{ GUI::Utilities::getPercentage(uiSize.x - (childSpacing * 3.f), 0.5f, childMinWidth), childHeaderHeight };
                    ImGui::SetCursorPos(ImVec2{ childWindowHeaderSize.x + (childSpacing * 2.f), childHeaderHeight + childHeight + (childSpacing * 2.f) });
                }
                else {
                    childWindowHeaderSize = ImVec2{ childMinWidth, childHeaderHeight };
                    float cursorPos = (uiSize.x - childWindowHeaderSize.x) / 2.f;
                    ImGui::SetCursorPos(ImVec2{ cursorPos, ((childHeaderHeight + childHeight) * 3.f) + (childSpacing * 4.f) });
                }
#endif

                ImGui::BeginGroup();
                {
                    // First window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(.31f, .67f, .98f, 1.00f));
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.f, 1.f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Title 4", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("DEVICE");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.95f, 0.98f, 1.f, 1.00f));
                        ImGui::BeginChild("Device Info 4", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::Text("Plop");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor();
                    }
                    ImGui::PopFont();
                }
                ImGui::EndGroup();

            }
            ImGui::PopStyleVar(2);
        }
        ImGui::PopStyleColor(2);
    }
    ImGui::End();

    GUI::displayFooter(_application);

    return true;
}
