#include "GUI.hpp"


void GUI::displayConfigInfoUI(char * title, char * content, ImVec4 color) {
    ImGui::TextColored(color, title);
    ImGui::Indent();
    {
        ImGui::Text(content);
    }
    ImGui::Unindent();
}
