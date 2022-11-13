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
			2, // 정의된 갯수 (Semantic 갯수)
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
		desc.FillMode = D3D11_FILL_WIREFRAME;	// 그려주는 방식 (선, 채우기)
		desc.CullMode = D3D11_CULL_BACK;		// 그림을 자르는 방식 (앞, 뒤)

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
	D3DXCOLOR bgcolor = D3DXCOLOR(0.22f, 0.27f, 0.44f, 1.0f);   // 배경색
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgcolor); //앞
	{
		UINT stride = sizeof(Vertex); //간격(보폭, 인덱스 나눠줄 크기)
		UINT offset = 0;		 	  //시작점

		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->IASetInputLayout(inputLatout);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		DeviceContext->RSSetState(Wirte ? rs_FrameMode : NULL);
		DeviceContext->Draw(6, 0);
	}
	SwapChain->Present(0, 0); //백퍼에 위에 내용 보냄
}
