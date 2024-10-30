#include "UI.h"
#include "Drawing.h"
#include "byte.h"
#include <D3DX11tex.h>
#include <dwmapi.h>
#include "texture.h"
#include "imgui_edited.h"
#include "colors3.h"
#include "inject.h"
#include "cipa.h"
#include "elements.h"
#pragma comment(lib, "D3DX11.lib")
ID3D11Device* UI::pd3dDevice = nullptr;
ID3D11DeviceContext* UI::pd3dDeviceContext = nullptr;
IDXGISwapChain* UI::pSwapChain = nullptr;
ID3D11RenderTargetView* UI::pMainRenderTargetView = nullptr;
using namespace std;
void injectowanieLOL()
{
    LPCSTR DllPath = "C:\\Backup\\Microsoft\\unnsense_load.dll"; // The Path to our DLL

    HWND hwnd = FindWindowA("LWJGL", nullptr); // HWND (Windows window) by Window Name
    DWORD procID; // A 32-bit unsigned integer, DWORDS are mostly used to store Hexadecimal Addresses
    GetWindowThreadProcessId(hwnd, &procID); // Getting our Process ID, as an ex. like 000027AC
    HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID); // Opening the Process with All Access

    // Allocate memory for the dllpath in the target process, length of the path string + null terminator
    LPVOID pDllPath = VirtualAllocEx(handle, 0, strlen(DllPath) + 1, MEM_COMMIT, PAGE_READWRITE);

    // Write the path to the address of the memory we just allocated in the target process
    WriteProcessMemory(handle, pDllPath, (LPVOID)DllPath, strlen(DllPath) + 1, 0);

    // Create a Remote Thread in the target process which calls LoadLibraryA as our dllpath as an argument -> program loads our dll
    HANDLE hLoadThread = CreateRemoteThread(handle, 0, 0,
        (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA"), pDllPath, 0, 0);

    WaitForSingleObject(hLoadThread, INFINITE); // Wait for the execution of our loader thread to finish



    VirtualFreeEx(handle, pDllPath, strlen(DllPath) + 1, MEM_RELEASE); // Free the memory allocated for our dll path



}
namespace texture
{
    ID3D11ShaderResourceView* logo = nullptr;
    ID3D11ShaderResourceView* swordicon = nullptr;
    ID3D11ShaderResourceView* logo_two = nullptr;
    ID3D11ShaderResourceView* foto_user = nullptr;
    ID3D11ShaderResourceView* sword_icon = nullptr;
    ID3D11ShaderResourceView* oko = nullptr;
    ID3D11ShaderResourceView* settings_icon = nullptr;
    ID3D11ShaderResourceView* close = nullptr;
    ID3D11ShaderResourceView* minimaze = nullptr;
    ID3D11ShaderResourceView* kamshi_logo = nullptr;
}

D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
HMODULE UI::hCurrentModule = nullptr;
namespace fonts {
    ImFont* medium;
    ImFont* semibold;

    ImFont* logo;
}
bool UI::CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    const UINT createDeviceFlags = 0;
    
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &pSwapChain, &pd3dDevice, &featureLevel, &pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void UI::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer != nullptr)
    {
        pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pMainRenderTargetView);
        pBackBuffer->Release();
    }
}

void UI::CleanupRenderTarget()
{
    if (pMainRenderTargetView)
    {
        pMainRenderTargetView->Release();
        pMainRenderTargetView = nullptr;
    }
}

void UI::CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (pSwapChain)
    {
        pSwapChain->Release();
        pSwapChain = nullptr;
    }

    if (pd3dDeviceContext)
    {
        pd3dDeviceContext->Release();
        pd3dDeviceContext = nullptr;
    }

    if (pd3dDevice)
    {
        pd3dDevice->Release();
        pd3dDevice = nullptr;
    }
}

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

LRESULT WINAPI UI::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_CREATE:
        // ...
        // Perform app resource initialization after window creation
        // ...

        RECT wRect;
        if (::GetWindowRect(hWnd, &wRect)) {
            HRGN hRgn = ::CreateRoundRectRgn(wRect.left, wRect.top, wRect.right, wRect.bottom, 100, 100);
            ::SetWindowRgn(hWnd, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }
        
        break;
    case WM_SIZE:
        if (pd3dDevice != nullptr && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;

    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;

    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;

    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;

    default:
        break;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
int mincps = 7;
bool niga_gui = false;
int maxcps = 14;
int mincps2 = 20;
int maxcps2 = 20;
int posy = 280;
int posy2 = 280;
int cwel = 0;
int cwel2 = 0;
bool injectok = false;
int toogle_bind = { 0 };
int hold_bind = { 0 };
int toogle_bind2 = { 0 };
int hold_bind2 = { 0 };
bool boxes_gui = false;
bool health_gui = false;
int leftclicker_bind = { 0 };
int rightclicker_bind = { 0 };
bool bDraw = true;
bool clicker_on = false;
bool rclicker_on = false;
void Active()
{
    bDraw = true;
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
bool isActive()
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

static heads head_selected = HEAD_1;
static bool checkbox[1]{};
ImGuiStyle* dst;
bool niga = false;

static int combobox, sliderscalar = 0;
const char* combobox_items[3] = { "MHold", "Toogle", "Hold" };
static int combobox2, sliderscalar2 = 0;
const char* combobox_items2[3] = { "MHold", "Toogle", "Hold" };
LPCSTR lpWindowName = "ImGui Standalone";
ImVec2 vWindowSize = { 864, 462 };
ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoDecoration;
void UI::Render()
{
    ImGui_ImplWin32_EnableDpiAwareness();
    
    const WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, _T("ImGui Standalone"), nullptr };
    ::RegisterClassEx(&wc);
    const HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("ImGui Standalone"),  WS_OVERLAPPEDWINDOW, 100, 100, 50, 50, NULL, NULL, wc.hInstance, NULL);
    
    if (!CreateDeviceD3D(hwnd))
    {
       
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return;
    }

    ::ShowWindow(hwnd, SW_HIDE);
    ::UpdateWindow(hwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

   

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 4;
        style.FrameRounding = 4.0f;
        style.PopupRounding = 4;
        style.WindowBorderSize = 0;
        style.Colors[ImGuiCol_FrameBg].w = 0.0f;
        style.Colors[ImGuiCol_ModalWindowDimBg].w = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 0.0f;
        style.Colors[ImGuiCol_Border].w = 0.0f;
        style.FramePadding = ImVec2(0,0);
        style.WindowPadding = ImVec2(0, 0);
        style.ChildRounding = 8;
    }
    ImFontConfig font_config;
    font_config.PixelSnapH = false;
    font_config.OversampleH = 5;
    font_config.OversampleV = 5;
    font_config.RasterizerMultiply = 1.2f;

    static const ImWchar ranges[] =
    {
        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0xE000, 0xE226, // icons
        0,
    };

    font_config.GlyphRanges = ranges;

    fonts::medium = io.Fonts->AddFontFromMemoryTTF(InterMedium, sizeof(InterMedium), 15.0f, &font_config, ranges);
    fonts::semibold = io.Fonts->AddFontFromMemoryTTF(InterSemiBold, sizeof(InterSemiBold), 17.0f, &font_config, ranges);
    D3DX11CreateShaderResourceViewFromMemory(UI::pd3dDevice, close_p, sizeof(close_p), &info, pump, &texture::close/*shader*/, 0);
  //  D3DX11CreateShaderResourceViewFromMemory(UI::pd3dDevice, sword_icon, sizeof(sword_icon), &info, pump, &texture::swordicon/*shader*/, 0);
    D3DX11CreateShaderResourceViewFromMemory(UI::pd3dDevice, kurwylogo, sizeof(kurwylogo), &info, pump, &texture::swordicon/*shader*/, 0);
    D3DX11CreateShaderResourceViewFromMemory(UI::pd3dDevice, stiv, sizeof(stiv), &info, pump, &texture::foto_user/*shader*/, 0);
    D3DX11CreateShaderResourceViewFromMemory(UI::pd3dDevice, mieczyk, sizeof(mieczyk), &info, pump, &texture::sword_icon/*shader*/, 0);
    D3DX11CreateShaderResourceViewFromMemory(UI::pd3dDevice, oczko, sizeof(oczko), &info, pump, &texture::oko/*shader*/, 0);
    D3DX11CreateShaderResourceViewFromMemory(UI::pd3dDevice, settings, sizeof(settings), &info, pump, &texture::settings_icon/*shader*/, 0);
    D3DX11CreateShaderResourceViewFromMemory(UI::pd3dDevice, kamshilogo, sizeof(kamshilogo), &info, pump, &texture::kamshi_logo/*shader*/, 0);
    const HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info = {};
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    const int monitor_height = info.rcMonitor.bottom - info.rcMonitor.top;

    if (monitor_height > 1080)
    {
        const float fScale = 2.0f;
        ImFontConfig cfg;
        cfg.SizePixels = 13 * fScale;
        ImGui::GetIO().Fonts->AddFontDefault(&cfg);
    }

    ImGui::GetIO().IniFilename = nullptr;

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(pd3dDevice, pd3dDeviceContext);

    const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    bool bDone = false;

    while (!bDone)
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                bDone = true;
        }

        if (GetAsyncKeyState(VK_END) & 1)
            bDone = true;

        if (bDone)
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::SetNextWindowSize(vWindowSize, ImGuiCond_Once);
            ImGui::SetNextWindowBgAlpha(1.0f);
            ImGui::Begin("essa", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
            {
                if (isActive())
                {

                    binds();

                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 232, 0));

                    auto draw = ImGui::GetWindowDrawList();
                    auto draw2 = ImGui::GetForegroundDrawList();

                    auto pos = ImGui::GetWindowPos();
                    auto size = ImGui::GetWindowSize();
                    draw->AddRectFilled(ImVec2(pos.x + 0, pos.y + 0), ImVec2(pos.x + size.x - 0, pos.y + 462), ImColor(bg_color1, bg_color2, bg_color3));
                    draw->AddRectFilled(ImVec2(pos.x + 0, pos.y + 0), ImVec2(pos.x + size.x - 760, pos.y + 462), ImColor(35, 35, 35), 8.0f, ImDrawFlags_RoundCornersRight);
                    //  draw->AddRectFilledMultiColorRounded(pos, ImVec2(pos.x + 55, pos.y + 51), ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.05f), ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.00f), ImColor(1.0f, 1.0f, 1.0f, 0.05f), 9.0f, ImDrawFlags_RoundCornersTopLeft);
                    draw->AddRectFilled(ImVec2(pos.x + 10, pos.y + 425), ImVec2(pos.x + size.x - 828, pos.y + 452), ImColor(38, 38, 38),4, ImDrawFlags_RoundCornersAll);
                    draw->AddRectFilled(ImVec2(pos.x + 40, pos.y + 425), ImVec2(pos.x + size.x - 770, pos.y + 452), ImColor(38, 38, 38),4, ImDrawFlags_RoundCornersAll);
                   //   draw->AddText(mediumxxddd, 17.0f, ImVec2(pos.x + 400, pos.y + 21), ImColor(0, 150, 214), "hex");
                    //  draw->AddText(mediumxxddd, 17.0f, ImVec2(pos.x + 425, pos.y + 21), ImColor(255, 255, 255), "agon");
                    draw->AddText(mediumxxddd, 13.0f, ImVec2(pos.x + 45, pos.y + 425), ImColor(255, 255, 255), "made by");
                    draw->AddText(mediumxxddd, 13.0f, ImVec2(pos.x + 49, pos.y + 435), ImColor(one2,two2,three2), "kamshi");
                    ImGui::SetCursorPos({ 0, 65 });
                    // ImGui::SetCursorPos({ 20, 32 });
                    ImGui::BeginGroup(); {
                        ImGui::BeginChild("##containertab", ImVec2(100, 120), false, ImGuiWindowFlags_NoScrollbar); {
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 11);
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                            if (elements::tab(texture::sword_icon,"Combat", head_selected == HEAD_1)) head_selected = HEAD_1;
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 11);
                            // ImGui::SameLine();
                            if (elements::tab(texture::oko,"Visuals", head_selected == HEAD_2)) head_selected = HEAD_2;
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 11);
                            // ImGui::SameLine();
                            if (elements::tab(texture::settings_icon,"Other", head_selected == HEAD_3)) head_selected = HEAD_3;
                        }
                        ImGui::EndChild();
                    }
                    ImGui::EndGroup();

                    switch (head_selected) {
                    case HEAD_1:
                        draw->AddText(mediumxxddd, 17.0f, ImVec2(pos.x + 440, pos.y + 21), ImColor(95 / 255.0f, 20 / 255.0f, 255 / 255.0f), "Combat");

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
                        draw->AddText(mediumxxddd, 17.0f, ImVec2(pos.x + 440, pos.y + 21), ImColor(95 / 255.0f, 20 / 255.0f, 255 / 255.0f), "Visuals");

                        ImGui::SetCursorPos({ 105, 65 });
                        ImGui::BeginChild("##container234", ImVec2(420, 350), false, ImGuiWindowFlags_NoScrollbar); {
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                            ImGui::Checkbox("PlayerESP", &niga);
                            

                        }
                        ImGui::EndChild();
                        if (niga) {

                            clicker_left();
                            clicker_right();
                            ImGui::SetCursorPos({ 120, 115 });
                            ImGui::BeginChild("##container1", ImVec2(147, 90), false, ImGuiWindowFlags_NoScrollbar); {
                                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 232, 0));
                                
                                // ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 15);
                                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                                ImGui::Checkbox2("Box", &boxes_gui);
                                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
                                ImGui::Checkbox2("Health", &health_gui);
                                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                                ImGui::Text("this shit dont work xd");
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
                            ImGui::SetCursorPos(ImVec2(278, 73));
                            ImGui::Image(texture::foto_user, ImVec2(130, 160), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImColor(95 / 0.1f, 20 / 0.1f, 255 / 0.1f, 1.0f));


                        }

                        break;
                    case HEAD_3:
                        draw->AddText(mediumxxddd, 17.0f, ImVec2(pos.x + 443, pos.y + 21), ImColor(95 / 255.0f, 20 / 255.0f, 255 / 255.0f), "Other");
                        clicker_left();
                        clicker_right();
                        ImGui::SetCursorPos({ 105, 65 });
                        ImGui::BeginChild("##container234", ImVec2(420, 350), false, ImGuiWindowFlags_NoScrollbar); {
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 45);
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
                            ImGui::Text("Inject cheetos");
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
                            
                            ImGui::Checkbox("    Inject", &injectok);
                            if (injectok) {
                                downloaddll();
                                injectowanieLOL();
                            }
                                if (ImGui::IsItemHovered()) {
                                    ImGui::BeginTooltip();
                                    ImGui::Text("You use it at your own risk");
                                    ImGui::EndTooltip();
                                }
                                else {

                                    ImGui::CloseCurrentPopup();
                                }
                            
                        }
                        ImGui::EndChild();
                        

                        break;
                    }

                }

#ifdef _WINDLL
                if (GetAsyncKeyState(VK_INSERT) & 1)
                    bDraw = !bDraw;
#endif

           ImGui::SetCursorPos(ImVec2(820, 21.5));
           ImGui::Image(texture::close, ImVec2(20, 20), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImColor(95 / 0.1f, 20 / 0.1f, 255 / 0.1f, 1.0f));
           if (ImGui::IsItemClicked(0))
               exit(0);
            }
            
            ImGui::SetCursorPos(ImVec2(10, 425));
            ImGui::Image(texture::kamshi_logo, ImVec2(25, 25), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImColor(95 / 0.1f, 20 / 0.1f, 255 / 0.1f, 1.0f));

            ImGui::SetCursorPos(ImVec2(27, 5));
            ImGui::Image(texture::swordicon, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, 1), ImColor(95 / 0.1f, 20 / 0.1f, 255 / 0.1f, 1.0f));
            
            ImGui::End();
        }
        ImGui::EndFrame();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        pd3dDeviceContext->OMSetRenderTargets(1, &pMainRenderTargetView, nullptr);
        pd3dDeviceContext->ClearRenderTargetView(pMainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        pSwapChain->Present(1, 0);

        #ifndef _WINDLL
            if (!isActive())
                break;
        #endif
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    #ifdef _WINDLL
    CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)FreeLibrary, hCurrentModule, NULL, nullptr);
    #endif
}