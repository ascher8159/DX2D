#include "stdafx.h"
#include "Rect.h"

Rect::Rect(Shader* shader) 
	: shader(shader), position(0, 0), scale(1, 1), color(1, 1, 1, 1), pass (0)
{
	Initialize();
}

Rect::Rect(Shader* shader, Vector2 position, Vector2 scale, D3DXCOLOR color)
	: shader(shader), position(position), scale(scale), color(color), pass(0)
{
	Initialize();
}

Rect::~Rect()
{
	SafeRelease(VertexBuffer);
	SafeRelease(IndexBuffer);
}

void Rect::Initialize()
{
	CreateBuffer();

	D3DXMatrixIdentity(&W);
	D3DXMatrixIdentity(&V);
	D3DXMatrixIdentity(&P);

	sWorld = shader->AsMatrix("World");
	sView = shader->AsMatrix("View");
	sProjection = shader->AsMatrix("Projection");
	sColor = shader->AsVector("Color");
}

void Rect::CreateBuffer()
{
	Vertex vertices[4];
	{
		vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
		vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
		vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);
		vertices[3].Position = Vector3(+0.5f, +0.5f, 0.0f);

		vertices[0].color = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1);
		vertices[1].color = D3DXCOLOR(0.0f, 0.5f, 0.5f, 1);
		vertices[2].color = D3DXCOLOR(0.0f, 0.0f, 0.5f, 1);
		vertices[3].color = D3DXCOLOR(0.0f, 0.5f, 0.5f, 1);
	}

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.ByteWidth = sizeof(Vertex) * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC; //CPU 영역에서 수정 가능
		desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &VertexBuffer);
		assert(SUCCEEDED(hr));
	}

	// Index Buffer Setting
	UINT indices[6] = { 0, 1 ,2 , 2, 1 ,3 };
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = sizeof(UINT) * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = indices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &IndexBuffer);
		assert(SUCCEEDED(hr));
	}
}

void Rect::Apply()
{
	D3DXMATRIX S, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, 1.0f);
	D3DXMatrixTranslation(&T, position.x, position.y, 0.0f);
	
	W = S * T;

	sWorld->SetMatrix(W);
	sView->SetMatrix(V);
	sProjection->SetMatrix(P);

	sColor->SetFloatVector(color);
}

void Rect::Update(Matrix& V, Matrix P)
{
	this->V = V;
	this->P = P;
}

void Rect::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	Apply();

	DeviceContext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	DeviceContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->DrawIndexed(0, pass, 6);
}

void Rect::Position(float x, float y){	
	Position(D3DXVECTOR2(x, y));
}
void Rect::Position(D3DXVECTOR2& vec){	position = vec; }

void Rect::Scale(float x, float y){ 
	Scale(D3DXVECTOR2(x, y));
}
void Rect::Scale(D3DXVECTOR2& vec){	scale = vec; }

void Rect::Color(float r, float g, float b, float a){ 
	Color(D3DXCOLOR(r, g, b, a));
}
void Rect::Color(D3DXCOLOR& vec){color = vec;}

