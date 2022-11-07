#include "Device.h"

ID3D11Buffer* vertexBuffer = NULL;
ID3D11InputLayout* inputLatout = NULL;

struct Vertex
{
	Vector3 Position;
	Color color;
};

//hlsl 연결
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
	Vertex vertices[4];
	vertices[0].Position = Vector3(0.0f, 0.0f, 0.0f);
	vertices[1].Position = Vector3(-0.5f, 0.5f, 0.0f);
	vertices[2].Position = Vector3(0.5f, 0.5f, 0.0f);
	vertices[3].Position = Vector3(0.0f, 0.0f, 0.0f);

	vertices[0].color = D3DXCOLOR(1, 0, 0, 1);
	vertices[1].color = D3DXCOLOR(0, 1, 0, 1);
	vertices[2].color = D3DXCOLOR(0, 0, 1, 1);
	vertices[3].color = D3DXCOLOR(1, 1, 0, 1);
	
	//Create VertexBuffer (desc -> SUBRESOURCE -> create buffer)
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;				// CPU는 쓸수 없지만 GPU는 쓸수 있음
		desc.ByteWidth = sizeof(Vertex) * 4;			// 버퍼의 크기를 잡아줌
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// 사용 용도

		//버퍼에 값을 넣고 넘겨줄 정의
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices;  // 처음 시작 주소

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
}

void DestroyScene()
{
	inputLatout->Release();
	vertexBuffer->Release();
}

void Update() {}

void Render()
{
	D3DXCOLOR bgcolor = D3DXCOLOR(0.22f, 0.27f, 0.44f, 1.0f);   // 배경색
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgcolor); //앞
	{
		//그려주는 코드
		UINT stride = sizeof(Vertex); //간격(보폭, 인덱스 나눠줄 크기)
		UINT offset = 0;		 	  //시작점

		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->IASetInputLayout(inputLatout);
		//그려주는 방식
		//DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//최종적으로 그려주는 코드 (파이프 라인을 호출)
		DeviceContext->Draw(4, 0);
	}
	SwapChain->Present(0, 0); //백퍼에 위에 내용 보냄
}
