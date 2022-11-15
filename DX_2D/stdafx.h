#pragma once

//Window
#include <Windows.h>
#include <assert.h>    //검사용

//STL
#include <bitset>      //키보드 관련 stl
#include <vector>
#include <string>
using namespace std;

//DX Lib
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#include <D3DX10math.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

//System
#include "System/Keyboard.h"


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

//https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nn-d3d11-id3d11pixelshader
extern ID3D11VertexShader* VertexShader;    // vs 단계에서 정점을 제어
extern ID3D11PixelShader* PixelShader;      // ps 단계에서 픽셸을 제어 
extern ID3D10Blob* VsBlob;					// 쉐이더 처리값 받는용 Vs용
extern ID3D10Blob* PsBlob;					// 쉐이더 처리값 받는용 Ps용

//TypeDef
typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR2 Vector2;
typedef D3DXMATRIX Matrix;
typedef D3DXCOLOR Color;
typedef D3DXQUATERNION Quaternion;

//Macro
#define SAFE_DELETE(p) { if(p) {delete (p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p) { if(p) {delete[] (p); (p) = NULL;}}
#define SAFE_RELEASE(p) { if(p) {(p)->Release(); (p) = NULL;}}