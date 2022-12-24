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
	
	void Movespeed(float val); //셋팅용
	FORCEINLINE float Movespeed() { return Speed; }

private:
	float Speed = 0.1f;

	float velocity = 0.0f;	// 점프에 필요한 기본 힘의 값
	float gravity = 2.0f;	// 중력

	bool Ground = true;		// true 지상 판정 false 공중 판정
};