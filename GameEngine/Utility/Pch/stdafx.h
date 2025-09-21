#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"

#include "../Debug/DebugHelper.h"

#include <Windows.h>
#include <iostream>
#include <wrl.h>
#include <array>
#include <map>
#include <vector>
#include <unordered_map>
#include <chrono>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

using Microsoft::WRL::ComPtr;
using namespace DirectX;