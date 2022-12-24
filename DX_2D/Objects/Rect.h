#pragma once

class Rect
{
public:
	Rect(Shader* shader);
	Rect(Shader* shader, Vector2 position, Vector2 scale, D3DXCOLOR color = D3DXCOLOR(1,1,1,1));
	virtual ~Rect(); // 가상함수 (상속)
	
	virtual void Update(Matrix& V, Matrix P);
	void Render();
	
private:
	void Initialize();
	void CreateBuffer();
	void Apply();

public:
	void Position(float x, float y);
	void Position(D3DXVECTOR2& vec);

	void Scale(float x, float y);
	void Scale(D3DXVECTOR2& vec);

	void Color(float r, float g, float b, float a = 1.0f);
	void Color(D3DXCOLOR& vec);

	FORCEINLINE	D3DXVECTOR2 Position() { return position; }
	FORCEINLINE D3DXVECTOR2 Scale() { return scale; }
	FORCEINLINE D3DXCOLOR Color() { return color; }
	FORCEINLINE	UINT &PASS() { return pass; }

private:
	struct Vertex
	{
		Vector3 Position;
		D3DXCOLOR color;
	};

private:
	Shader* shader;

	ID3D11Buffer* VertexBuffer = nullptr;
	ID3D11Buffer* IndexBuffer = nullptr;

	Vector2 scale;
	D3DXCOLOR color;
	
	Matrix W, V, P;

	UINT pass = 0;

	// shader->AsMatrix()를 계속 shader를 통해서 호출 하는건 자원 낭비 심함
	ID3DX11EffectMatrixVariable* sWorld;
	ID3DX11EffectMatrixVariable* sView;
	ID3DX11EffectMatrixVariable* sProjection;
	ID3DX11EffectVectorVariable* sColor;

	Vector2 position;
};