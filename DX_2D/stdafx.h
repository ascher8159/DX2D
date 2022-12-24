#pragma once
//디버그 모드
//#ifdef _DEBUG
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif 
#include <iostream>
using namespace std;

//Window
#include <Windows.h>
#include <assert.h>		// 검사용
#include <mutex>		// thread 임계구역 구별용 (Time)

//STL
#include <bitset>      // 키보드 관련 stl
#include <vector>
#include <string>
#include <algorithm>   // transfrom 
#include <functional>  // 추후 void* 변수 관련
using namespace std;

//DX Lib
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#include <D3DX10math.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

//D3D11 fx & Imgui
#include <d3dx11effect.h>
#include <d3dcompiler.h>
#pragma comment(lib, "Effects11d.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "ImGui.lib")

//System
#include "System/Keyboard.h"
#include "System/Time.h"
#include "Utilities/Path.h"
#include "Utilities/String.h"
#include "Renders/Shader.h"

//Macro
#define SafeRelease(p) { if(p) { (p)->Release(); (p) = nullptr; } }
#define SafeDelete(p) { if(p) { delete (p); (p) = nullptr; } }
#define SafeDeleteArray(p) { if(p) { delete[] (p); (p) = nullptr; } }
#define Check(hr) { assert(SUCCEEDED(hr)); }
#define SAFE_DELETE(p) { if(p) { delete (p); (p) = nullptr; } }

//TypeDef
typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR2 Vector2;
typedef D3DXMATRIX Matrix;
typedef D3DXCOLOR Color;
typedef D3DXQUATERNION Quaternion;

//Global Variable
//--윈도우
const UINT Width = 800;
const UINT Height = 600;
extern HWND Hwnd;
extern wstring Title;

//Frame work (System)
extern Keyboard* Key;

//--DX ComInterfase
extern IDXGISwapChain* SwapChain;			// 백버퍼 관리용
extern ID3D11Device* Device;				// dx자원을 셋팅용
extern ID3D11DeviceContext* DeviceContext;  // dx에서 사용하는 HDC(렌더링)
extern ID3D11RenderTargetView* RTV;         // 실제 화면을 보여주는 용


