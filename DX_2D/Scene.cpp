#include "Device.h"

ID3D11Buffer* vertexBuffer = NULL;
ID3D11InputLayout* inputLatout = NULL;
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

void InitScene()
{
	Vertex vertices[6];
	vertices[0].Position = Vector3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = Vector3(+0.5f, -0.5f, 0.0f);
	vertices[4].Position = Vector3(-0.5f, +0.5f, 0.0f);
	vertices[5].Position = Vector3(+0.5f, +0.5f, 0.0f);

	vertices[0].Color = D3DXCOLOR(1, 0, 0, 1);
	vertices[1].Color = D3DXCOLOR(0, 1, 0, 1);
	vertices[2].Color = D3DXCOLOR(0, 0, 1, 1);
	vertices[3].Color = D3DXCOLOR(0, 0, 1, 1);
	vertices[4].Color = D3DXCOLOR(0, 1, 0, 1);
	vertices[5].Color = D3DXCOLOR(1, 0, 0, 1);

	//Create VertexBuffer 1
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;				
		desc.ByteWidth = sizeof(Vertex) * 6;			
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices; 

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//Create InputLayout
	{
		HRESULT hr = Device->CreateInputLayout(
			layoutDesc,
			2, // ���ǵ� ���� (Semantic ����)
			VsBlob->GetBufferPointer(),
			VsBlob->GetBufferSize(),
			&inputLatout
		);
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
	inputLatout->Release();
	vertexBuffer->Release();
	rs_FrameMode->Release();
}

bool Wirte = false;
void Update() 
{
	if (GetAsyncKeyState('1') & 0x8001)
		Wirte = !Wirte;
}

void Render()
{
	D3DXCOLOR bgcolor = D3DXCOLOR(0.22f, 0.27f, 0.44f, 1.0f);   // ����
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgcolor); //��
	{
		UINT stride = sizeof(Vertex); //����(����, �ε��� ������ ũ��)
		UINT offset = 0;		 	  //������

		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->IASetInputLayout(inputLatout);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		DeviceContext->RSSetState(Wirte ? rs_FrameMode : NULL);
		DeviceContext->Draw(6, 0);
	}
	SwapChain->Present(0, 0); //���ۿ� ���� ���� ����
}
