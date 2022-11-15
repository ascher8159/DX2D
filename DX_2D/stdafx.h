#pragma once

//Window
#include <Windows.h>
#include <assert.h>    //�˻��

//STL
#include <bitset>      //Ű���� ���� stl
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
//--������
const UINT Width = 800;
const UINT Height = 600;
extern HWND Hwnd;
extern wstring Title;

//Frame work (System)
extern Keyboard* Key;

//--DX ComInterfase
extern IDXGISwapChain* SwapChain;			// ����� ������
extern ID3D11Device* Device;				// dx�ڿ��� ���ÿ�
extern ID3D11DeviceContext* DeviceContext;  // dx���� ����ϴ� HDC(������)
extern ID3D11RenderTargetView* RTV;         // ���� ȭ���� �����ִ� ��

//https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nn-d3d11-id3d11pixelshader
extern ID3D11VertexShader* VertexShader;    // vs �ܰ迡�� ������ ����
extern ID3D11PixelShader* PixelShader;      // ps �ܰ迡�� �ȼ��� ���� 
extern ID3D10Blob* VsBlob;					// ���̴� ó���� �޴¿� Vs��
extern ID3D10Blob* PsBlob;					// ���̴� ó���� �޴¿� Ps��

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