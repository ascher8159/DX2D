#include "Device.h"

ID3D11Buffer* vertexBuffer = NULL;
ID3D11InputLayout* inputLatout = NULL;

struct Vertex
{
	Vector3 Position;
	Color color;
};

//hlsl ����
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
		desc.Usage = D3D11_USAGE_DEFAULT;				// CPU�� ���� ������ GPU�� ���� ����
		desc.ByteWidth = sizeof(Vertex) * 4;			// ������ ũ�⸦ �����
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// ��� �뵵

		//���ۿ� ���� �ְ� �Ѱ��� ����
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices;  // ó�� ���� �ּ�

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
}

void DestroyScene()
{
	inputLatout->Release();
	vertexBuffer->Release();
}

void Update() {}

void Render()
{
	D3DXCOLOR bgcolor = D3DXCOLOR(0.22f, 0.27f, 0.44f, 1.0f);   // ����
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgcolor); //��
	{
		//�׷��ִ� �ڵ�
		UINT stride = sizeof(Vertex); //����(����, �ε��� ������ ũ��)
		UINT offset = 0;		 	  //������

		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->IASetInputLayout(inputLatout);
		//�׷��ִ� ���
		//DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//���������� �׷��ִ� �ڵ� (������ ������ ȣ��)
		DeviceContext->Draw(4, 0);
	}
	SwapChain->Present(0, 0); //���ۿ� ���� ���� ����
}
