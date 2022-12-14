#include "stdafx.h"
#include "Device.h"

#include "Objects/Rect.h"
#include "Objects/Rect_Control.h"

Rect* BackGround;
Rect* rect;
Rect_Control* R_c;

Shader* shader = nullptr;

//WVP 위한 Matrix
Matrix View, Projection;

void InitScene()
{
	shader = new Shader(L"02_WVP.fx");

	BackGround = new Rect(shader);
	BackGround->Position((float)Width * 0.5f, (float)Height * 0.5f);
	BackGround->Scale((float)Width, (float)Height);
	BackGround->Color(0, 1, 1, 1);

	rect = new Rect(shader);
	rect->Position(200, 500);
	rect->Scale(50, 100);
	rect->Color(1, 0, 1, 1);
	rect->PASS() = 1;

	R_c = new Rect_Control(shader);
	R_c->Position((float)Width * 0.5f, (float)Height * 0.5f);
	R_c->Scale(100, 100);
	R_c->Color(1, 1, 1, 1);
}

void DestroyScene()
{
	SafeDelete(shader);

	SafeDelete(BackGround);
	SafeDelete(rect);
	SafeDelete(R_c);
}

void Update() 
{	
	//Camera
	Vector3 eye = Vector3(0, 0, 0); 
	Vector3 at = Vector3(0, 0, 1);
	Vector3 up = Vector3(0, 1, 0);
	D3DXMatrixLookAtLH(&View, &eye, &(eye + at), &up);
	D3DXMatrixOrthoOffCenterLH(&Projection, 0, (FLOAT)Width, 0, (FLOAT)Height, -1.0f, +1.0f);
	
	//Test Pass
	static bool check = false;
	ImGui::Checkbox("Rect Pass", &check);
	check ? rect->PASS() = 0 : rect->PASS() = 1;

	//Test speed
	float mvSpeed = R_c->Movespeed();
	ImGui::SliderFloat("MoveSpeed", &mvSpeed, 0.001f, 1.0f);
	R_c->Movespeed(mvSpeed);

	if (Key->Press('A'))
		R_c->MoveLeft();

	else if (Key->Press('D'))
		R_c->MoveRight();

	//FPS
	ImGui::Text("%.1f", ImGui::GetIO().Framerate);
	ImGui::Text("FPS : %.1f", Time::Get()->FPS());
	ImGui::Text("Running Time : %.1f", Time::Get()->Running());

	BackGround->Update(View, Projection);
	rect->Update(View, Projection);
	R_c->Update(View, Projection);
}

void Render()
{
	D3DXCOLOR bgcolor = D3DXCOLOR(0.22f, 0.27f, 0.44f, 1.0f);   // 배경색
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgcolor); //앞
	{
		BackGround->Render();
		rect->Render();

		R_c->PASS() = 1;
		R_c->Render();
	}
	ImGui::Render(); //ImGui 출력
	SwapChain->Present(0, 0); //BackBuffer 출력
}
