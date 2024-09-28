#include "Drawing.h"
#include "elements.h"
#include "UI.h"
#include "cipa.h"
#include "imgui_edited.h"
int mincps = 7;
bool niga_gui = false;
int maxcps = 14;
int mincps2 = 20;
int maxcps2 = 20;
int posy = 280;
int posy2 = 280;
int cwel = 0;
int cwel2 = 0;
int toogle_bind = { 0 };
int hold_bind = { 0 };
int toogle_bind2 = { 0 };
int hold_bind2 = { 0 };
bool boxes_gui = false;
bool health_gui = false;
int leftclicker_bind = { 0 };
int rightclicker_bind = { 0 };
bool Drawing::bDraw = true;
bool clicker_on = false;
bool rclicker_on = false;
void Drawing::Active()
{
	bDraw = true;
}

bool Drawing::isActive()
{
	return bDraw == true;
}
ImFont* mediumxxddd;
enum heads {
    HEAD_1,
    HEAD_2,
    HEAD_3
};
void binds() {
    if (GetAsyncKeyState(leftclicker_bind) & 1) {
        niga_gui ^= 1;
    }
    if (GetAsyncKeyState(rightclicker_bind) & 1) {
        rclicker_on ^= 1;
    }
}
void clicker_left() {

    if (cwel == 0) {
        static CTimer timer(std::chrono::milliseconds(1000 / mincps));
        if (!clicker_on || !(GetKeyState(VK_LBUTTON) & 0x8000))
            return;
        if (!timer.isElapsed())
            return;
        POINT cursorPos{};
        GetCursorPos(&cursorPos);
        PostMessageA(FindWindowA("LWJGL", nullptr), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(cursorPos.x, cursorPos.y));
        PostMessageA(FindWindowA("LWJGL", nullptr), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(cursorPos.x, cursorPos.y));
        std::uniform_int_distribution<> distr(mincps, maxcps);
        timer.setEvery(std::chrono::milliseconds(1000 / distr(gen)));
    } 
    if (cwel == 1) {
        static CTimer timer(std::chrono::milliseconds(1000 / mincps));
        if (!clicker_on || !(GetKeyState(toogle_bind) & 1))
            return;
        if (!timer.isElapsed())
            return;
        POINT cursorPos{};
        GetCursorPos(&cursorPos);
        PostMessageA(FindWindowA("LWJGL", nullptr), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(cursorPos.x, cursorPos.y));
        PostMessageA(FindWindowA("LWJGL", nullptr), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(cursorPos.x, cursorPos.y));
        std::uniform_int_distribution<> distr(mincps, maxcps);
        timer.setEvery(std::chrono::milliseconds(1000 / distr(gen)));
    }
    if (cwel == 2) {
        static CTimer timer(std::chrono::milliseconds(1000 / mincps));
        if (!clicker_on || !(GetKeyState(hold_bind) & 0x8000))
            return;
        if (!timer.isElapsed())
            return;
        POINT cursorPos{};
        GetCursorPos(&cursorPos);
        PostMessageA(FindWindowA("LWJGL", nullptr), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(cursorPos.x, cursorPos.y));
        PostMessageA(FindWindowA("LWJGL", nullptr), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(cursorPos.x, cursorPos.y));
        std::uniform_int_distribution<> distr(mincps, maxcps);
        timer.setEvery(std::chrono::milliseconds(1000 / distr(gen)));
    }

}
void clicker_right() {

    if (cwel2 == 0) {
        static CTimer timer(std::chrono::milliseconds(1000 / mincps2));
        if (!rclicker_on || !(GetKeyState(VK_RBUTTON) & 0x8000))
            return;
        if (!timer.isElapsed())
            return;
        POINT cursorPos{};
        GetCursorPos(&cursorPos);
        PostMessageA(FindWindowA("LWJGL", nullptr), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(cursorPos.x, cursorPos.y));
        PostMessageA(FindWindowA("LWJGL", nullptr), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(cursorPos.x, cursorPos.y));
        std::uniform_int_distribution<> distr(mincps2, maxcps2);
        timer.setEvery(std::chrono::milliseconds(1000 / distr(gen)));
    }
    if (cwel2 == 1) {
        static CTimer timer(std::chrono::milliseconds(1000 / mincps2));
        if (!rclicker_on || !(GetKeyState(toogle_bind2) & 1))
            return;
        if (!timer.isElapsed())
            return;
        POINT cursorPos{};
        GetCursorPos(&cursorPos);
        PostMessageA(FindWindowA("LWJGL", nullptr), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(cursorPos.x, cursorPos.y));
        PostMessageA(FindWindowA("LWJGL", nullptr), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(cursorPos.x, cursorPos.y));
        std::uniform_int_distribution<> distr(mincps2, maxcps2);
        timer.setEvery(std::chrono::milliseconds(1000 / distr(gen)));
    }
    if (cwel2 == 2) {
        static CTimer timer(std::chrono::milliseconds(1000 / mincps2));
        if (!rclicker_on || !(GetKeyState(hold_bind2) & 0x8000))
            return;
        if (!timer.isElapsed())
            return;
        POINT cursorPos{};
        GetCursorPos(&cursorPos);
        PostMessageA(FindWindowA("LWJGL", nullptr), WM_RBUTTONDOWN, MK_RBUTTON, MAKELPARAM(cursorPos.x, cursorPos.y));
        PostMessageA(FindWindowA("LWJGL", nullptr), WM_RBUTTONUP, MK_RBUTTON, MAKELPARAM(cursorPos.x, cursorPos.y));
        std::uniform_int_distribution<> distr(mincps2, maxcps2);
        timer.setEvery(std::chrono::milliseconds(1000 / distr(gen)));
    }
}
static heads head_selected = HEAD_1;
static bool checkbox[1]{};
ImGuiStyle* dst;
bool niga = false;

static int combobox, sliderscalar = 0;
const char* combobox_items[3] = { "MHold", "Toogle", "Hold"};
static int combobox2, sliderscalar2 = 0;
const char* combobox_items2[3] = { "MHold", "Toogle", "Hold"};
void Drawing::Draw()
{
	if (isActive())
	{
        
        binds();
          
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 232, 0));
		
            auto draw = ImGui::GetWindowDrawList();
            auto draw2 = ImGui::GetForegroundDrawList();

            auto pos = ImGui::GetWindowPos();
            auto size = ImGui::GetWindowSize();
            draw->AddRectFilled(ImVec2(pos.x + 0, pos.y + 50), ImVec2(pos.x + size.x - 0, pos.y + 462), ImColor(38, 38, 38));
            draw->AddRectFilled(ImVec2(pos.x + 0, pos.y + 0), ImVec2(pos.x + size.x - 0, pos.y + 61), ImColor(35, 35, 35), 8.0f, ImDrawFlags_RoundCornersBottom);
          //  draw->AddRectFilledMultiColorRounded(pos, ImVec2(pos.x + 55, pos.y + 51), ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.05f), ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.05f), 9.0f, ImDrawFlags_RoundCornersTopLeft);
           
            draw->AddText(mediumxxddd, 17.0f, ImVec2(pos.x + 400, pos.y + 21), ImColor(0, 150, 214), "hex");
            draw->AddText(mediumxxddd, 17.0f, ImVec2(pos.x + 425, pos.y + 21), ImColor(255, 255, 255), "agon");
            ImGui::SetCursorPos({ 0, 65 }); 
           // ImGui::SetCursorPos({ 20, 32 });
            ImGui::BeginGroup(); {
                ImGui::BeginChild("##containertab", ImVec2(100, 120), false, ImGuiWindowFlags_NoScrollbar); {
                   // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                if (elements::tab("Combat", head_selected == HEAD_1)) head_selected = HEAD_1;
                //ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
               // ImGui::SameLine();
                if (elements::tab("Visuals", head_selected == HEAD_2)) head_selected = HEAD_2;
              //  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
               // ImGui::SameLine();
                if (elements::tab("Other", head_selected == HEAD_3)) head_selected = HEAD_3;
                }
                ImGui::EndChild();
        }
            ImGui::EndGroup();

            switch (head_selected) {
            case HEAD_1:
                

                ImGui::SetCursorPos({ 105, 65 });
                ImGui::BeginChild("##container", ImVec2(420, 350), false, ImGuiWindowFlags_NoScrollbar); {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                    ImGui::Checkbox("LeftClicker", &niga_gui); ImGui::SameLine();
                    ImGui::Checkbox("RightClicker", &rclicker_on);


                }
                ImGui::EndChild();
                if (niga_gui) {
                    clicker_on = true;
                    clicker_left();
                    ImGui::SetCursorPos({ 120, 110 });
                    ImGui::BeginChild("##container1", ImVec2(147, 100), false, ImGuiWindowFlags_NoScrollbar); {
                        
                      // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                        ImGui::PushItemWidth(20);
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                        ImGui::SliderInt("Min CPS", &mincps, 1, 15, "%d", ImGuiSliderFlags_None);
                      //  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                        ImGui::PushItemWidth(20);
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                        ImGui::SliderInt("Max CPS", &maxcps, 1, 15, "%d", ImGuiSliderFlags_None);
                        if (mincps > maxcps) {
                            maxcps = mincps;
                        }
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                        ImGui::Checkbox2("break blocks", &checkbox[0]);
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos({ 120, 220 });
                    ImGui::BeginChild("##container1123123", ImVec2(147, 150), false, ImGuiWindowFlags_NoScrollbar); {
                        
                       ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
                        edited::Keybind("Bind", "", &leftclicker_bind);
                        if (leftclicker_bind > 0) {
                            if (ImGui::IsItemHovered()) {
                                ImGui::BeginTooltip();
                                ImGui::Text("If you want clear the bind click Delete key");
                                ImGui::EndTooltip();
                            }
                            else {

                                ImGui::CloseCurrentPopup();
                            }
                        }
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
                        ImGui::Combo("", &combobox, combobox_items, IM_ARRAYSIZE(combobox_items));
                        if (combobox == 0) {
                            cwel = 0;
                        } if (combobox == 1) {
                            cwel = 1;
                            edited::Keybind("Tg Bind", "", &toogle_bind);
                            posy = 320;
                            if (toogle_bind > 0) {
                                if (ImGui::IsItemHovered()) {
                                    ImGui::BeginTooltip();
                                    ImGui::Text("If you want clear the bind click Delete key");
                                    ImGui::EndTooltip();
                                }
                                else {

                                    ImGui::CloseCurrentPopup();
                                }
                            }
                        }
                        if (combobox == 2) {
                            cwel = 2;
                            edited::Keybind("Hold Bind", "", &hold_bind);
                            posy = 320;
                            if (hold_bind > 0) {
                                if (ImGui::IsItemHovered()) {
                                    ImGui::BeginTooltip();
                                    ImGui::Text("If you want clear the bind click Delete key");
                                    ImGui::EndTooltip();
                                }
                                else {

                                    ImGui::CloseCurrentPopup();
                                }
                            }
                        }
                        if (cwel == 0) {
                            posy = 280;
                        }
                       // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                        //ImGui::Checkbox2("break blocks", &checkbox[1]);
                    }
                    ImGui::EndChild();
                   
                   // draw->AddText(mediumxxddd, 14.0f, ImVec2(pos.x + 285, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Settings");
                    
                    ImGui::SetCursorPos({ 155, 15 });
                   
                      draw->AddRectFilled(ImVec2(pos.x + 120, pos.y + 115), ImVec2(pos.x + size.x - 599, pos.y + 210), ImColor(45, 45, 45), 4.0f, ImDrawFlags_RoundCornersAll);
                      draw->AddRectFilled(ImVec2(pos.x + 120, pos.y + 215), ImVec2(pos.x + size.x - 599, pos.y + posy), ImColor(45, 45, 45), 4.0f, ImDrawFlags_RoundCornersAll);
                       ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 51);
                       ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                        
                        

                } 
                else
                {
                    clicker_on = false;
                }
                if (rclicker_on) {
                    clicker_right();
                    ImGui::SetCursorPos({ 285, 110 });
                    ImGui::BeginChild("##container12232323", ImVec2(147, 100), false, ImGuiWindowFlags_NoScrollbar); {
                        
                      // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                        ImGui::PushItemWidth(20);
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                        ImGui::SliderInt("Min CPS", &mincps2, 1, 60, "%d", ImGuiSliderFlags_None);
                      //  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                        ImGui::PushItemWidth(20);
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                        ImGui::SliderInt("Max CPS", &maxcps2, 1, 60, "%d", ImGuiSliderFlags_None);
                        if (mincps2 > maxcps2) {
                            maxcps2 = mincps2;
                        }
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                        ImGui::Checkbox2("break blocks", &checkbox[0]);
                    }
                    ImGui::EndChild();
                    ImGui::SetCursorPos({ 285, 220 });
                    ImGui::BeginChild("##container1123123234", ImVec2(147, 150), false, ImGuiWindowFlags_NoScrollbar); {

                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
                        edited::Keybind("Bind", "", &rightclicker_bind);
                        if (rightclicker_bind > 0) {
                            if (ImGui::IsItemHovered()) {
                                ImGui::BeginTooltip();
                                ImGui::Text("If you want clear the bind click Delete key");
                                ImGui::EndTooltip();
                            }
                            else {

                                ImGui::CloseCurrentPopup();
                            }
                        }
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
                        ImGui::Combo("", &combobox2, combobox_items2, IM_ARRAYSIZE(combobox_items2));
                        if (combobox2 == 0) {
                            cwel2 = 0;
                        } if (combobox2 == 1) {
                            cwel2 = 1;
                            edited::Keybind("Tg Bind", "", &toogle_bind2);
                            posy2 = 320;
                            if (toogle_bind2 > 0) {
                                if (ImGui::IsItemHovered()) {
                                    ImGui::BeginTooltip();
                                    ImGui::Text("If you want clear the bind click Delete key");
                                    ImGui::EndTooltip();
                                }
                                else {

                                    ImGui::CloseCurrentPopup();
                                }
                            }
                        }
                        if (combobox2 == 2) {
                            cwel2 = 2;
                            edited::Keybind("Hold Bind", "", &hold_bind2);
                            posy2 = 320;
                            if (hold_bind2 > 0) {
                                if (ImGui::IsItemHovered()) {
                                    ImGui::BeginTooltip();
                                    ImGui::Text("If you want clear the bind click Delete key");
                                    ImGui::EndTooltip();
                                }
                                else {

                                    ImGui::CloseCurrentPopup();
                                }
                            }
                        }
                        
                        if (cwel2 == 0) {
                            posy2 = 280;
                        }
                        // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                         //ImGui::Checkbox2("break blocks", &checkbox[1]);
                    }
                    ImGui::EndChild();
                   // draw->AddText(mediumxxddd, 14.0f, ImVec2(pos.x + 285, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Settings");
                    
                    ImGui::SetCursorPos({ 155, 135 });
                   
                      draw->AddRectFilled(ImVec2(pos.x + 285, pos.y + 115), ImVec2(pos.x + size.x - 433, pos.y + 210), ImColor(45, 45, 45), 4.0f, ImDrawFlags_RoundCornersAll);
                      draw->AddRectFilled(ImVec2(pos.x + 285, pos.y + 215), ImVec2(pos.x + size.x - 433, pos.y + posy2), ImColor(45, 45, 45), 4.0f, ImDrawFlags_RoundCornersAll);
                       ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 51);
                       ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                        
                        

                }
                
                break;
            case HEAD_2:
                

                ImGui::SetCursorPos({ 105, 65 });
                ImGui::BeginChild("##container234", ImVec2(420, 350), false, ImGuiWindowFlags_NoScrollbar); {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                    ImGui::Checkbox("PlayerESP", &niga);


                }
                ImGui::EndChild();
                if (niga) {
                    clicker_left();
                    ImGui::SetCursorPos({ 120, 115 });
                    ImGui::BeginChild("##container1", ImVec2(147, 80), false, ImGuiWindowFlags_NoScrollbar); {
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 232, 0));
                         
                        // ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 15);
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                        ImGui::Checkbox2("Box", &boxes_gui);
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                        ImGui::Checkbox2("Health", &health_gui);
                        // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                       // ImGui::PushItemWidth(20);
                      //  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                       // ImGui::SliderInt("Min CPS", &mincps, 1, 15, "%d", ImGuiSliderFlags_None);
                        //  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                       // ImGui::PushItemWidth(20);
                      //  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                        //ImGui::SliderInt("Max CPS", &maxcps, 1, 15, "%d", ImGuiSliderFlags_None);
                      //  if (mincps > maxcps) {
                       //     maxcps = mincps;
                       // }
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos({ 320, 130 });
                    ImGui::BeginChild("##container22423423434345", ImVec2(120, 80), false, ImGuiWindowFlags_NoScrollbar); {
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 232, 0));
                        
                        // ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                       // ImGui::PushItemWidth(20);
                      //  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                       // ImGui::SliderInt("Min CPS", &mincps, 1, 15, "%d", ImGuiSliderFlags_None);
                        //  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                       // ImGui::PushItemWidth(20);
                      //  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                        //ImGui::SliderInt("Max CPS", &maxcps, 1, 15, "%d", ImGuiSliderFlags_None);
                      //  if (mincps > maxcps) {
                       //     maxcps = mincps;
                       // }
                    }
                    ImGui::EndChild();
                    // draw->AddText(mediumxxddd, 14.0f, ImVec2(pos.x + 285, pos.y + 60), ImColor(1.0f, 1.0f, 1.0f, 0.6f), "Settings");

                    ImGui::SetCursorPos({ 155, 135 });

                    draw->AddRectFilled(ImVec2(pos.x + 120, pos.y + 115), ImVec2(pos.x + size.x - 599, pos.y + 230), ImColor(45, 45, 45), 4.0f, ImDrawFlags_RoundCornersAll);
                    draw->AddRectFilled(ImVec2(pos.x + 283, pos.y + 75), ImVec2(pos.x + size.x - 459, pos.y + 230), ImColor(45, 45, 45), 4.0f, ImDrawFlags_RoundCornersAll);
                    if (boxes_gui) {
                        draw->AddRectFilled(ImVec2(pos.x + 308, pos.y + 85), ImVec2(pos.x + size.x - 484, pos.y + 220), ImColor(55, 55, 55), 2.0f, ImDrawFlags_RoundCornersAll);
                        draw->AddRect(ImVec2(pos.x + 308, pos.y + 85), ImVec2(pos.x + size.x - 484, pos.y + 220), ImColor(245, 245, 222), 2.0f, ImDrawFlags_RoundCornersAll);
                        
                    }
                    if (health_gui) {
                        draw->AddLine(ImVec2(pos.x + 303, pos.y + 87), ImVec2(pos.x + size.x - 560.8, pos.y + 218), ImColor(0, 255, 0), 0.0f);
                    }
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 51);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);



                }
                
                break;
            }
    
	}

	#ifdef _WINDLL
	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDraw = !bDraw;
	#endif
}
