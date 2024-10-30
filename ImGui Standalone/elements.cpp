#include "elements.h"
#include "colors.h"
struct tab_element {
    float element_opacity;
    float element_opacity2;
    float element_opacity3;
    float element_opacity4;
};

bool elements::tab(ID3D11ShaderResourceView* icon, const char* name, bool boolean)
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
    it_anim->second.element_opacity2 = ImLerp(it_anim->second.element_opacity2, (boolean ? 0.8f : hovered ? 0.6f : 0.0f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.element_opacity3 = ImLerp(it_anim->second.element_opacity3, (boolean ? 0.8f : hovered ? 0.6f : 0.1f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.element_opacity4 = ImLerp(it_anim->second.element_opacity4, (boolean ? 0.3f : hovered ? 0.2f : 0.0f), 0.07f * (1.0f - ImGui::GetIO().DeltaTime));
    window->DrawList->AddRectFilled(ImVec2(pos.x, pos.y + 0), ImVec2(pos.x + 80, rect.Max.y), ImColor(one, two, three, it_anim->second.element_opacity4), 4, ImDrawFlags_RoundCornersAll);
    window->DrawList->AddRectFilled(ImVec2(pos.x, pos.y + 0), ImVec2(pos.x + 80, rect.Max.y), ImColor(48 / 255.0f, 48 / 255.0f, 48 / 255.0f, it_anim->second.element_opacity), 4, ImDrawFlags_RoundCornersAll);
    window->DrawList->AddRectFilled(ImVec2(pos.x, pos.y + 5), ImVec2(pos.x + 2, rect.Max.y - 5), ImColor(one, two, three, it_anim->second.element_opacity2), 100, ImDrawFlags_RoundCornersRight);
    //window->DrawList->AddRectFilled(ImVec2(pos.x + 0.1, pos.y + 1), ImVec2(pos.x + 79, rect.Max.y -1), ImColor(55 / 255.0f, 55 / 255.0f, 55 / 255.0f, it_anim->second.element_opacity), 3, ImDrawFlags_RoundCornersAll);
   // window->DrawList->AddRectFilled(ImVec2(pos.x, pos.y + 1.1), ImVec2(pos.x + 3, rect.Max.y - 1.1), ImColor(0 / 255.0f, 150 / 255.0f, 214 / 255.0f, it_anim->second.element_opacity), 4, ImDrawFlags_RoundCornersRight);
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 27);
    ImGui::Image(icon, ImVec2(15, 15), ImVec2(0, 0),ImVec2(1,1),ImVec4(1,1,1,1), ImColor(95 / 0.1f, 20 / 0.1f, 255 / 0.1f, it_anim->second.element_opacity3));
    
    window->DrawList->AddText(rect.Min + ImVec2(23,7), ImColor(55 / 255.0f, 55 / 255.0f, 55 / 255.0f, it_anim->second.element_opacity), name);
    window->DrawList->AddText(rect.Min + ImVec2(23,7), ImColor(1.0f, 1.0f, 1.0f, it_anim->second.element_opacity), name);
    
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    return pressed;
}
