#pragma once

#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <map>
#include <D3DX11tex.h>

#pragma comment(lib, "D3DX11.lib")
#include "imgui/imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"

namespace elements {
    bool tab(ID3D11ShaderResourceView* icon,const char* name, bool boolean);
}
