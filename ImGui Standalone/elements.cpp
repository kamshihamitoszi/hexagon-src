#include "elements.h"

struct tab_element {
    float element_opacity;
};

bool elements::tab(const char* name, bool boolean)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(name);
    const ImVec2 label_size = ImGui::CalcTextSize(name, NULL, true);
    ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, ImVec2(pos.x + label_size.x + 40, pos.y + label_size.y + 15));
    ImGui::ItemSize(ImVec4(rect.Min.x, rect.Min.y, rect.Max.x + 40, rect.Max.y), style.FramePadding.y);
    if (!ImGui::ItemAdd(rect, id))
        return false;

    bool hovered, held;
    bool pressed = ImGui::ButtonBehavior(rect, id, &hovered, &held, NULL);

    static std::map <ImGuiID, tab_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end()) {
        anim.insert({ id, { 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.element_opacity = ImLerp(it_anim->second.element_opacity, (boolean ? 0.8f : hovered ? 0.6f : 0.4f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
    window->DrawList->AddRectFilled(ImVec2(pos.x, pos.y + 0), ImVec2(pos.x + 80, rect.Max.y), ImColor(0 / 255.0f, 150 / 255.0f, 214 / 255.0f, it_anim->second.element_opacity), 4, ImDrawFlags_RoundCornersRight);
    window->DrawList->AddRectFilled(ImVec2(pos.x + 0.1, pos.y + 1), ImVec2(pos.x + 79, rect.Max.y -1), ImColor(55 / 255.0f, 55 / 255.0f, 55 / 255.0f, it_anim->second.element_opacity), 3, ImDrawFlags_RoundCornersRight);
   // window->DrawList->AddRectFilled(ImVec2(pos.x, pos.y + 1.1), ImVec2(pos.x + 3, rect.Max.y - 1.1), ImColor(0 / 255.0f, 150 / 255.0f, 214 / 255.0f, it_anim->second.element_opacity), 4, ImDrawFlags_RoundCornersRight);
    window->DrawList->AddText(rect.Min + ImVec2(12,7), ImColor(55 / 255.0f, 55 / 255.0f, 55 / 255.0f, it_anim->second.element_opacity), name);
    window->DrawList->AddText(rect.Min + ImVec2(12,7), ImColor(1.0f, 1.0f, 1.0f, it_anim->second.element_opacity), name);
    

    return pressed;
}
