#include "stdafx.h"
#include "System/Device.h"

#include "Objects/Rect.h"
#include "Objects/Rect_Control.h"

Shader* shader = nullptr;

//WVP 위한 Matrix
Matrix V, P;

struct Vertex
{
	Vector3 Position;
	Vector2 UV;			//TexCoord 이미지 좌표 값이라고도 부름
}vertices[6];

ID3D11Buffer* vertexBuffer = nullptr;
ID3D11ShaderResourceView* srv;

void InitScene()
{
	shader = new Shader(L"03_Texture.fx");
	
	//Vertices Setting
	{
		vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
		vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
		vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);
		vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);
		vertices[4].Position = Vector3(-0.5f, +0.5f, 0.0f);
		vertices[5].Position = Vector3(+0.5f, +0.5f, 0.0f);

		vertices[0].UV = Vector2(0, 1);
		vertices[1].UV = Vector2(0, 0);
		vertices[2].UV = Vector2(1, 1);
		vertices[3].UV = Vector2(1, 1);
		vertices[4].UV = Vector2(0, 0);
		vertices[5].UV = Vector2(1, 0);
	}
		
	//Vertex Setting
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices;
		
		Check(Device->CreateBuffer(&desc, &data, &vertexBuffer));
	}

	//Create SRV (Image)
	{
		Check(D3DX11CreateShaderResourceViewFromFile
		(
			Device,
			L"../../_Textures/Yukari.png",
			nullptr,
			nullptr,
			&srv,
			nullptr
		));
	}
}

void DestroyScene()
{
	SafeDelete(shader);
	
	SafeRelease(vertexBuffer);
	SafeRelease(srv);
}

void Update()
{
	//FPS
	ImGui::Text("%.1f", ImGui::GetIO().Framerate);
	ImGui::Text("FPS : %.1f", Time::Get()->FPS());
	ImGui::Text("Running Time : %.1f", Time::Get()->Running());
}

void Render()
{
	D3DXCOLOR bgcolor = D3DXCOLOR(0.22f, 0.27f, 0.44f, 1.0f);   // 배경색
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgcolor); //앞
	{
		Matrix W, S, T;
		Vector3 eye = Vector3(0, 0, 0);
		Vector3 at = Vector3(0, 0, 1);
		Vector3 up = Vector3(0, 1, 0);

		// NDC & Camera Setting
		{
			D3DXMatrixScaling(&S, 300, 300, 1);
			D3DXMatrixTranslation(&T, Width * 0.5f, Height * 0.5f, 0);
			W = S * T;

			D3DXMatrixLookAtLH(&V, &eye, &(eye + at), &up);
			D3DXMatrixOrthoOffCenterLH(&P, 0, (FLOAT)Width, 0, (FLOAT)Height, -1.0f, +1.0f);
		}

		//Shader Setting
		{
			shader->AsMatrix("World")->SetMatrix(W);
			shader->AsMatrix("View")->SetMatrix(V);
			shader->AsMatrix("Projection")->SetMatrix(P);
			shader->AsSRV("TextureMap")->SetResource(srv);
		}

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		static UINT pass = 0;
		static UINT select = 0;
		ImGui::SliderInt("Pass Setting", (int*)&pass, 0, shader->PassCount()-1);
		ImGui::SliderInt("Sampler Setting", (int*)&select, 0, 1);
		shader->AsScalar("select")->SetInt(select);

		shader->Draw(0, pass, 6);
	}
	ImGui::Render(); //ImGui 출력
	SwapChain->Present(0, 0); //BackBuffer 출력
}



