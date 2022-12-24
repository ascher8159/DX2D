#pragma once
#include "Rect.h"

class Rect_Control : public Rect
{
public:
	Rect_Control(Shader* shader);
	Rect_Control(Shader* shader, Vector2 position, Vector2 scale, D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1));

	void Update(Matrix& V, Matrix P) override;

	void MoveLeft();
	void MoveRight();

	void Jump();
	void StopJump();
	
	void Movespeed(float val); //���ÿ�
	FORCEINLINE float Movespeed() { return Speed; }

private:
	float Speed = 0.1f;

	float velocity = 0.0f;	// ������ �ʿ��� �⺻ ���� ��
	float gravity = 2.0f;	// �߷�

	bool Ground = true;		// true ���� ���� false ���� ����
};