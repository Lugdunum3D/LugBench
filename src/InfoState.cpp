#include "InfoState.hpp"
#include "GUI.hpp"

#include <lug/Graphics/Builder/Light.hpp>
#include <lug/Graphics/Builder/Camera.hpp>
#include <lug/Graphics/Vulkan/API/RTTI/Enum.hpp>
#include <lug/Graphics/Vulkan/Render/Texture.hpp>
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
    float windowHeaderOffset = GUI::displayMenu(_application);
    float windowFooterOffset = GUI::displayFooter(_application);

    ImVec2 infoWindowSize{ GUI::Utilities::getPercentage(windowWidth, 0.35f, 390.f), windowHeight - (windowHeaderOffset + windowFooterOffset) };

    ImGui::Begin("Info Window", 0, _application._window_flags);
    {
        ImGui::SetWindowSize(infoWindowSize);
        ImGui::SetWindowPos(ImVec2{ 0.f, windowHeaderOffset });

#if defined(LUG_SYSTEM_ANDROID)
        ImVec2 deviceWindowSize{ GUI::Utilities::getPercentage(ImGui::GetWindowWidth(), 0.9f), 150.f * 2.f };
#else
        ImVec2 deviceWindowSize{ GUI::Utilities::getPercentage(ImGui::GetWindowWidth(), 0.9f), 150.f };
#endif
        float padding = (ImGui::GetWindowWidth() - deviceWindowSize.x) / 2.f;
        ImGui::SetCursorPos(ImVec2{ padding , 20.f });
        ImGui::PushStyleColor(ImGuiCol_Text, GUI::V4_DARKGRAY);
        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, GUI::V4_LIGHTBLUE);
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.f, 0.f });
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 5.f, 2.f });
            {
                ImGui::BeginChild("Device Window", deviceWindowSize);
                {
                    ImGui::SetCursorPos(ImVec2{ 15.f, GUI::Utilities::getPercentage(deviceWindowSize.y, 0.15f) });
                    auto vkTexture = lug::Graphics::Resource::SharedPtr<lug::Graphics::Vulkan::Render::Texture>::cast(_application._infoDeviceLogo);
#if defined(LUG_SYSTEM_ANDROID)
                    ImGui::Image(vkTexture.get(), ImVec2(100.f * 2.f, 100.f * 2.f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(130.f * 2.f);
#else
                    ImGui::Image(vkTexture.get(), ImVec2(100.f, 100.f), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1));
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(130.f);
#endif

                    ImGui::BeginChild("Device Info");
                    {
                        ImGui::Text("DEVICE");
                        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
                        {
                            lug::Core::Version apiVersion = lug::Core::Version::fromInt(_physicalDeviceInfo->properties.apiVersion);
                            lug::Core::Version driverVersion = lug::Core::Version::fromInt(_physicalDeviceInfo->properties.driverVersion);

                            ImGui::Text("Name: %s", _physicalDeviceInfo->properties.deviceName);
                            ImGui::Text("Driver Version: %d.%d.%d", driverVersion.major, driverVersion.minor, driverVersion.patch);
                            ImGui::Text("API Version: %d.%d.%d", apiVersion.major, apiVersion.minor, apiVersion.patch);
                        }
                        ImGui::PopFont();
                    }
                    ImGui::EndChild();
                }
                ImGui::EndChild();
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
        ImGui::SetWindowPos(ImVec2{ infoWindowSize.x, windowHeaderOffset });

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

                ImVec2 childWindowSize{ childWindowHeaderSize.x, childHeight - 20.f };

                ImGui::BeginGroup();
                {
                    // First window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, GUI::V4_SKYBLUE);
                        ImGui::PushStyleColor(ImGuiCol_Text, GUI::V4_WHITE);
                        ImGui::BeginChild("Device Title 1", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("FEATURES");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, GUI::V4_LIGHTBLUE);
                        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, GUI::V4_LIGHTBLUE);
                        ImGui::BeginChild("Device Info 1", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::Indent();
                                {
                                    GUI::displayDeviceFeatures(_physicalDeviceInfo, childWindowSize.x * .8f);
                                }
                                ImGui::Unindent();
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
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
                    // Second window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, GUI::V4_SKYBLUE);
                        ImGui::PushStyleColor(ImGuiCol_Text, GUI::V4_WHITE);
                        ImGui::BeginChild("Device Title 2", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("EXTENSIONS");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, GUI::V4_LIGHTBLUE);
                        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, GUI::V4_LIGHTBLUE);
                        ImGui::BeginChild("Device Info 2", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::Indent();
                                {
                                    for (auto extension : _physicalDeviceInfo->extensions) {
                                        GUI::displayConfigInfoVersion(extension.extensionName, lug::Core::Version::fromInt(extension.specVersion), childWindowSize.x * .8f);
                                    }
                                }
                                ImGui::Unindent();
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
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
                    // Third window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, GUI::V4_SKYBLUE);
                        ImGui::PushStyleColor(ImGuiCol_Text, GUI::V4_WHITE);
                        ImGui::BeginChild("Device Title 3", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("QUEUE FAMILIES");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, GUI::V4_LIGHTBLUE);
                        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, GUI::V4_LIGHTBLUE);
                        ImGui::BeginChild("Device Info 3", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::Indent();
                                {
                                    for (uint32_t i = 0; i < _physicalDeviceInfo->queueFamilies.size(); ++i) {
                                        ImGui::PushID(i);
                                        {
                                            ImGui::Text("Queue %d", i);
                                            ImGui::Indent();
                                            {
                                                VkQueueFamilyProperties queueFamilyProperties = _physicalDeviceInfo->queueFamilies[i];

                                                GUI::displayConfigInfoUnsignedLongValue("Queue Count", queueFamilyProperties.queueCount);
                                                GUI::displayConfigInfoArrayStr("Queue Flags", lug::Graphics::Vulkan::API::RTTI::VkQueueFlagsToStrVec(queueFamilyProperties.queueFlags));
                                            }
                                            ImGui::Unindent();
                                        }
                                        ImGui::PopID();
                                    }
                                }
                                ImGui::Unindent();
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
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
                    // Fourth window
                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, GUI::V4_SKYBLUE);
                        ImGui::PushStyleColor(ImGuiCol_Text, GUI::V4_WHITE);
                        ImGui::BeginChild("Device Title 4", childWindowHeaderSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::SetWindowFontScale(0.87f);
                                ImGui::Text("MEMORY HEAPS");
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
                    }
                    ImGui::PopFont();

                    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
                    {
                        ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, GUI::V4_LIGHTBLUE);
                        ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, GUI::V4_LIGHTBLUE);
                        ImGui::BeginChild("Device Info 4", childWindowSize, false, ImGuiWindowFlags_AlwaysUseWindowPadding);
                        {
                            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 3.f, 1.f });
                            {
                                ImGui::Indent();
                                {
                                    for (size_t i = 0; i < _physicalDeviceInfo->memoryProperties.memoryHeapCount; ++i) {
                                        ImGui::Text("Heap %lu", i);
                                        ImGui::Indent();
                                        {
                                            GUI::displayConfigInfoUnsignedLongValue("Size", _physicalDeviceInfo->memoryProperties.memoryHeaps[i].size);
                                            std::vector<const char*> flags = lug::Graphics::Vulkan::API::RTTI::VkMemoryHeapFlagsToStrVec(_physicalDeviceInfo->memoryProperties.memoryHeaps[i].flags);
                                            if (flags.size() > 0) {
                                                ImGui::PushID(static_cast<int>(i));
                                                {
                                                    GUI::displayConfigInfoArrayStr("Flags", flags);
                                                }
                                                ImGui::PopID();
                                            }
                                        }
                                        ImGui::Unindent();
                                    }
                                }
                                ImGui::Unindent();                            
                            }
                            ImGui::PopStyleVar();
                        }
                        ImGui::EndChild();
                        ImGui::PopStyleColor(2);
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

    return true;
}
