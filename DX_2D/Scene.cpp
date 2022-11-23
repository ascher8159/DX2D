#include "stdafx.h"
#include "Device.h"

Shader* shader = nullptr;

ID3D11Buffer* vertexBuffer = NULL;
ID3D11Buffer* IndexBuffer = NULL;
ID3D11RasterizerState* rs_FrameMode = NULL;


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

void InitScene()
{
	shader = new Shader(L"01_Effect.fx");

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

	//RasterizerState
	{
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		desc.FillMode = D3D11_FILL_WIREFRAME;	// 그려주는 방식 (선, 채우기)
		desc.CullMode = D3D11_CULL_BACK;		// 그림을 자르는 방식 (앞, 뒤)
		HRESULT hr = Device->CreateRasterizerState(&desc, &rs_FrameMode);
		assert(SUCCEEDED(hr));
	}
}

void DestroyScene()
{
	SafeRelease(vertexBuffer);
	SafeRelease(IndexBuffer);
	SafeRelease(rs_FrameMode);
	SafeDelete(shader);
}

bool Wirte = false;
void Update() 
{
	//if (GetAsyncKeyState('1') & 0x8001)
	if(Key->Toggle('1')) Wirte = !Wirte;

	if (Key->Press('A'))
	{
		for(int i =0 ; i < ARRAYSIZE(vertices) ; i++)
			vertices[i].Position.x -= 1e-4f; 
	}
	else if (Key->Press('D'))
	{
		for (int i = 0; i < ARRAYSIZE(vertices); i++)
			vertices[i].Position.x += 1e-4f; 
	}

	if (Key->Press('W'))
	{
		for (int i = 0; i < ARRAYSIZE(vertices); i++)
			vertices[i].Position.y += 1e-4f;
	}
	else if (Key->Press('S'))
	{
		for (int i = 0; i < ARRAYSIZE(vertices); i++)
			vertices[i].Position.y -= 1e-4f;
	}
	
	
	// Subresouce (CPU방식)
	D3D11_MAPPED_SUBRESOURCE subResouce;
	DeviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResouce);
	{
		//메모리 카피(저장 받을 Subresource, 복사할 시작위치, 크기)
		memcpy(subResouce.pData, vertices, sizeof(Vertex) * 4);
	}
	DeviceContext->Unmap(vertexBuffer, 0);
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

		DeviceContext->RSSetState(Wirte ? rs_FrameMode : NULL);

		//DeviceContext->DrawIndexed(6 , 0 , 0);

		//T
		shader->DrawIndexed(0 , 0 , 6); //변경
	}
	SwapChain->Present(0, 0); //백퍼에 위에 내용 보냄
}
