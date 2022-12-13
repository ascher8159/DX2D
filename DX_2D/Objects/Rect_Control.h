#pragma once
#include "Rect.h"

class Rect_Control : public Rect
{
public:
	Rect_Control(Shader* shader);
	Rect_Control(Shader* shader, Vector2 position, Vector2 scale, D3DXCOLOR color = D3DXCOLOR(1, 1, 1, 1));

	void MoveLeft();
	void MoveRight();

	void Movespeed(float val); //¼ÂÆÃ¿ë
	FORCEINLINE float Movespeed() { return Speed; }

private:
	float Speed = 0.1f;

};