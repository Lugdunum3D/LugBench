#include "GUI.hpp"


void GUI::displayConfigInfoString(const char* title, const char* content, const ImVec4 color) {
    ImGui::TextColored(color, "%s", title);
    ImGui::Indent();
    {
        ImGui::Text("%s", content);
    }
    ImGui::Unindent();
}

void GUI::displayConfigInfoVersion(const char* title, const lug::Core::Version& version, const ImVec4 color) {
    ImGui::TextColored(color, "%s", title);
    ImGui::Indent();
    {
        ImGui::Text("%d.%d.%d", version.major, version.minor, version.patch);
    }
    ImGui::Unindent();
}

void GUI::displayConfigInfoValue(const char* title, const int value, const ImVec4 color) {
    ImGui::TextColored(color, "%s", title);
    ImGui::Indent();
    {
        ImGui::Text("%d", value);
    }
    ImGui::Unindent();
}

void GUI::displayConfigInfoFloatValue(const char* title, const float value, const ImVec4 color) {
    ImGui::TextColored(color, "%s", title);
    ImGui::Indent();
    {
        ImGui::Text("%.3f", value);
    }
    ImGui::Unindent();
}

void GUI::displayConfigInfoUnsignedLongValue(const char* title, const uint64_t value, const ImVec4 color) {
    ImGui::TextColored(color, "%s", title);
    ImGui::Indent();
    {
        ImGui::Text("%lu", value);
    }
    ImGui::Unindent();
}

void GUI::displayConfigInfoBool(const char* title, const bool isTrue, const ImVec4 color)
{
    ImGui::TextColored(color, "%s", title);
    ImGui::Indent();
    {
        if (isTrue == true) { ImGui::Text("True"); }
        else { ImGui::Text("False"); }
    }
    ImGui::Unindent();
}

void GUI::displayConfigInfoArrayUint8(const char* title, const std::vector<uint8_t>& dataArray) {
    if (ImGui::CollapsingHeader(title)) {
        ImGui::Indent();
        {
            for (auto element : dataArray) {
                ImGui::Text("%d", element);
            }
        }
        ImGui::Unindent();
    }
}

void GUI::displayConfigInfoArrayUint32(const char* title, const std::vector<uint32_t>& dataArray) {
    if (ImGui::CollapsingHeader(title)) {
        ImGui::Indent();
        {
            for (auto element : dataArray) {
                ImGui::Text("%d", element);
            }
        }
        ImGui::Unindent();
    }
}

void GUI::displayConfigInfoArrayFloat(const char* title, const std::vector<float>& dataArray) {
    if (ImGui::CollapsingHeader(title)) {
        ImGui::Indent();
        {
            for (auto element : dataArray) {
                ImGui::Text("%f.3", element);
            }
        }
        ImGui::Unindent();
    }
}

void GUI::displayConfigInfoArrayStr(const char* title, const std::vector<const char *>& dataArray) {
    if (ImGui::CollapsingHeader(title)) {
        ImGui::Indent();
        {
            for (auto element : dataArray) {
                ImGui::Text("%s", element);
            }
        }
        ImGui::Unindent();
    }
}

void GUI::displayScoreInCell(const char * deviceName, const float score, const float progressbarValue) {

	ImGui::BeginGroup();
	ImGui::Text("Your GPU: %s",deviceName);
	ImGui::Value("Your score", score);


	ImGui::ProgressBar(progressbarValue, ImVec2(-1, 50.0F));
	ImGui::Separator();
	ImGui::EndGroup();
}