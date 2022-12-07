#include "stdafx.h"
#include "Device.h"

#include "Objects/Rect.h"

Rect* rect;
Shader* shader = nullptr;

//WVP ���� Matrix
Matrix View, Projection;

void InitScene()
{
	shader = new Shader(L"02_WVP.fx");

	rect = new Rect(shader);
	rect->Position(400, 300);
	rect->Scale(100, 100);
	rect->Color(1, 0, 0, 1);
}

void DestroyScene()
{
	SafeDelete(shader);
	SafeDelete(rect);
}

void Update() 
{	
	Vector3 eye = Vector3(0, 0, 0); //ī�޶� ��ġ
	Vector3 at = Vector3(0, 0, 1); //ī�޶� �ٶ󺸴� ����
	Vector3 up = Vector3(0, 1, 0);
	D3DXMatrixLookAtLH(&View, &eye, &(eye + at), &up);
	D3DXMatrixOrthoOffCenterLH(&Projection, 0, (FLOAT)Width, 0, (FLOAT)Height, -1.0f, +1.0f);

	rect->Update(View, Projection);
}

void Render()
{
	D3DXCOLOR bgcolor = D3DXCOLOR(0.22f, 0.27f, 0.44f, 1.0f);   // ����
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgcolor); //��
	{
		rect->Render();
	}
	ImGui::Render(); //ImGui ���
	SwapChain->Present(0, 0); //BackBuffer ���
}
