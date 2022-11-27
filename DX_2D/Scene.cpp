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
		"POSITION", //hlsl VS������ �ִ� SemanticName
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
		12, // ������ ���� 
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
		vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f); //�� �Ʒ�
		vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f); //�� ��
		vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f); //�� �Ʒ�
		vertices[3].Position = Vector3(+0.5f, +0.5f, 0.0f); //�� ��

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
		// - ���� �ȳֱ����� unsigned int
		UINT indices[6] = { 0, 1 ,2 ,2 ,1 ,3 }; 

		//Create indexBuffer
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_IMMUTABLE;  // GPU, CPU ���� ���� ���ϰ� ����
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
		desc.FillMode = D3D11_FILL_WIREFRAME;	// �׷��ִ� ��� (��, ä���)
		desc.CullMode = D3D11_CULL_BACK;		// �׸��� �ڸ��� ��� (��, ��)
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
Vector3 color = {0.0f, 0.0f, 0.0f}; //D3DXCOLOR ����ü ����ص� �������
void Update() 
{	
	//ImGui
	{
		ImGui::Text("%s", "ImGui start");						// �ؽ�Ʈ
		ImGui::Checkbox("Frame", &Wirte);						// üũ �ڽ�
		ImGui::ColorEdit3("Color", (float*)&color, 0);			// Color Edit
		ImGui::Text("%f, %f, %f", color.x, color.y, color.z);
	}

	// cpu���� Semantic�̸��� ���ؼ� �ش簪 ����
	// 1. AsVector�� ���� 2. SetFloatVector ���ؼ� ����
	shader->AsVector("Color")->SetFloatVector(color);

	//RS
	if(Key->Toggle('1')) Wirte = !Wirte;

	//Key
	{
		if (Key->Press('A'))
		{
			for (int i = 0; i < ARRAYSIZE(vertices); i++)
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
	}

	// Subresouce (CPU���)
	D3D11_MAPPED_SUBRESOURCE subResouce;
	DeviceContext->Map(vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResouce);
	{
		//�޸� ī��(���� ���� Subresource, ������ ������ġ, ũ��)
		memcpy(subResouce.pData, vertices, sizeof(Vertex) * 4);
	}
	DeviceContext->Unmap(vertexBuffer, 0);
}

void Render()
{
	D3DXCOLOR bgcolor = D3DXCOLOR(0.22f, 0.27f, 0.44f, 1.0f);   // ����
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgcolor); //��
	{
		UINT stride = sizeof(Vertex); //����(����, �ε��� ������ ũ��)
		UINT offset = 0;		 	  //������

		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DeviceContext->RSSetState(Wirte ? rs_FrameMode : NULL);

		//���
		shader->DrawIndexed(0 , 0 , 6); //����
	}
	ImGui::Render(); //ImGui ���
	SwapChain->Present(0, 0); //BackBuffer ���
}
