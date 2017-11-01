#pragma once

#include <stdint.h>
#include <lug/Core/Version.hpp>
#include <lug/Graphics/Vulkan/Renderer.hpp>
#include <json/json.hpp>
#include <imgui.h>
#include "Application.hpp"

#include "BenchmarksState.hpp"
#include "ModelsState.hpp"
#include "ResultsState.hpp"
#include "InfoState.hpp"
#include "ContactState.hpp"

#include <vector>

namespace GUI {

static const ImVec4 V4_WHITE{ 1.f, 1.f, 1.f, 1.f };
static const ImVec4 V4_SKYBLUE{ .31f, .67f, .98f, 1.f };
static const ImVec4 V4_LIGHTGRAY{ 0.80f, 0.80f, 0.83f, 1.f };
static const ImVec4 V4_GRAY{ 0.5f, 0.5f, 0.5f, 1.f };
static const ImVec4 V4_DARKGRAY{ 0.40f, 0.40f, 0.40f, 1.f };
static const ImVec4 V4_PINK{ 1.f, 0.08f, 0.58f, 1.f };

void displayConfigInfoString(const char* title, const char* content, const ImVec4 color = {.64f, .87f, .29f, 1.f});
void displayConfigInfoVersion(const char* title, const lug::Core::Version& version, const ImVec4 color = {.64f, .87f, .29f, 1.f});

void displayConfigInfoValue(const char* title, const int value, const ImVec4 color = {.64f, .87f, .29f, 1.f});
void displayConfigInfoFloatValue(const char* title, const float value, const ImVec4 color = {.64f, .87f, .29f, 1.f});
void displayConfigInfoUnsignedLongValue(const char* title, const uint64_t value, const ImVec4 color = {.64f, .87f, .29f, 1.f});

void displayConfigInfoBool(const char* title, const bool isTrue, const ImVec4 color = {.64f, .87f, .29f, 1.f});

void displayConfigInfoArrayUint8(const char* title, const std::vector<uint8_t>& dataArray);
void displayConfigInfoArrayUint32(const char* title, const std::vector<uint32_t>& dataArray);
void displayConfigInfoArrayFloat(const char* title, const std::vector<float>& dataArray);
void displayConfigInfoArrayStr(const char* title, const std::vector<const char *>& dataArray);
void displayScoreInCell(const char * deviceName, const float score, const float progressbarValue);

void displayInfoScreen(bool* isOpen, ImGuiWindowFlags windowFlags, lug::Graphics::Render::Window* window, lug::Graphics::Vulkan::PhysicalDeviceInfo* physicalDeviceInfo);
void displayResultScreen(bool* isOpen, ImGuiWindowFlags windowFlags, lug::Graphics::Render::Window* window, lug::Graphics::Vulkan::PhysicalDeviceInfo* physicalDeviceInfo, nlohmann::json* devices);

void displayDeviceLimits(lug::Graphics::Vulkan::PhysicalDeviceInfo* physicalDeviceInfo);
void displayDeviceFeatures(lug::Graphics::Vulkan::PhysicalDeviceInfo* physicalDeviceInfo);

bool displayReturnButton();

void setDefaultStyle();

float displayMenu(LugBench::Application &application);
float displayFooter(LugBench::Application &application);

namespace Utilities {

float getPercentage(float fullSize, float percentage, float minSize = 0);
float getMainMenuHeight(float windowHeight);
float getFooterHeight(float windowHeight);

} // Utilities
} // GUI
