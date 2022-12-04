#include "stdafx.h"
#include "Device.h"

Shader* shader = nullptr;

ID3D11Buffer* vertexBuffer = NULL;
ID3D11Buffer* IndexBuffer = NULL;

struct Vertex
{
	Vector3 Position;
	Color Color;
};

D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
	{
		"POSITION", //hlsl VS변수에 있는 SemanticName
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,	
		0,
		D3D11_INPUT_PER_VERTEX_DATA,
		0,

	},
	{
		"COLOR", //SemanticName
		0,
		DXGI_FORMAT_R32G32B32_FLOAT,
		0,
		12, // 끊어줄 단위 
		D3D11_INPUT_PER_VERTEX_DATA,
		0,
	}
};

Vertex vertices[4];

//WVP 위한 Matrix
Matrix World1, World2, View, Projection;

void InitScene()
{
	//shader = new Shader(L"01_Effect.fx");
	shader = new Shader(L"02_WVP.fx");

	//VertexBuffer Setting & Create VertexBuffer
	{
		vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f); //왼 아래
		vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f); //왼 위
		vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f); //우 아래
		vertices[3].Position = Vector3(+0.5f, +0.5f, 0.0f); //우 위

		vertices[0].Color = D3DXCOLOR(1, 0, 0, 1);
		vertices[1].Color = D3DXCOLOR(0, 1, 0, 1);
		vertices[2].Color = D3DXCOLOR(0, 0, 1, 1);
		vertices[3].Color = D3DXCOLOR(0, 1, 1, 1);

		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 4;			
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		
		desc.Usage = D3D11_USAGE_DYNAMIC;	
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices; 

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	// Index Buffer & IndexBuffer Setting
	{
		// - 값을 안넣기위한 unsigned int
		UINT indices[6] = { 0, 1 ,2 ,2 ,1 ,3 }; 

		//Create indexBuffer
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_IMMUTABLE;  // GPU, CPU 에서 수정 못하게 막기
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = indices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &IndexBuffer);
		assert(SUCCEEDED(hr));
	}

	//항등행렬 초기화
	D3DXMatrixIdentity(&View);
	D3DXMatrixIdentity(&Projection);

	//카메라 셋팅
	{
		Vector3 eye = Vector3(0, 0, 0); //카메라 위치
		Vector3 at  = Vector3(0, 0, 1); //카메라 바라보는 방향
		Vector3 up  = Vector3(0, 1, 0);  
		D3DXMatrixLookAtLH(&View, &eye, &(eye + at), &up);
		D3DXMatrixOrthoOffCenterLH(&Projection, 0, (FLOAT)Width, 0, (FLOAT)Height, -1.0f, +1.0f);
	}
}

void DestroyScene()
{
	SafeRelease(vertexBuffer);
	SafeRelease(IndexBuffer);
	SafeDelete(shader);
}

Vector3 postiion = Vector3(400, 300, 0);
void Update() 
{	
	//Key
	{
		if (Key->Press('A'))
			postiion.x -= 0.1f;
		else if (Key->Press('D'))
			postiion.x += 0.1f;

		if (Key->Press('W'))
			postiion.y += 0.1f;
		else if (Key->Press('S'))
			postiion.y -= 0.1f;

		D3DXMATRIX S, T;
		D3DXMatrixScaling(&S, 100, 100, 1.0f);
		D3DXMatrixTranslation(&T, postiion.x, postiion.y, 0.0f);
		World1 = S * T;
	}
	
	//Set WVP
	shader->AsMatrix("World")->SetMatrix(World1);
	shader->AsMatrix("View")->SetMatrix(View);
	shader->AsMatrix("Projection")->SetMatrix(Projection);
}

void Render()
{
	D3DXCOLOR bgcolor = D3DXCOLOR(0.22f, 0.27f, 0.44f, 1.0f);   // 배경색
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgcolor); //앞
	{
		UINT stride = sizeof(Vertex); //간격(보폭, 인덱스 나눠줄 크기)
		UINT offset = 0;		 	  //시작점

		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//출력
		shader->DrawIndexed(0 , 0 , 6); //변경
	}
	ImGui::Render(); //ImGui 출력
	SwapChain->Present(0, 0); //BackBuffer 출력
}
