#pragma once

#include <stdint.h>
#include <lug/Core/Version.hpp>
#include <imgui.h>

#include <vector>

namespace GUI {

void displayConfigInfoString(const char* title, const char* content, const ImVec4 color = { 0, 255, 0, 255 });
void displayConfigInfoVersion(const char* title, const lug::Core::Version& version, const ImVec4 color = { 0, 255, 0, 255 });

void displayConfigInfoValue(const char* title, const int value, const ImVec4 color = { 0, 255, 0, 255 });
void displayConfigInfoFloatValue(const char* title, const float value, const ImVec4 color = { 0, 255, 0, 255 });
void displayConfigInfoUnsignedLongValue(const char* title, const uint64_t value, const ImVec4 color = { 0, 255, 0, 255 });

void displayConfigInfoBool(const char* title, const bool isTrue, const ImVec4 color = { 0, 255, 0, 255 });

void displayConfigInfoArrayUint8(const char* title, const std::vector<uint8_t>& dataArray);
void displayConfigInfoArrayUint32(const char* title, const std::vector<uint32_t>& dataArray);
void displayConfigInfoArrayFloat(const char* title, const std::vector<float>& dataArray);
void displayConfigInfoArrayStr(const char* title, const std::vector<const char *>& dataArray);

} // GUI
