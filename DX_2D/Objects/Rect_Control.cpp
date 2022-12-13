#include "stdafx.h"
#include "Rect_Control.h"

Rect_Control::Rect_Control(Shader* shader) 
	: Rect(shader)
{
}

Rect_Control::Rect_Control(Shader* shader, Vector2 position, Vector2 scale, D3DXCOLOR color)
	: Rect(shader, position, scale, color)
{
}

void Rect_Control::MoveLeft()
{
	Vector2 pos = Position();
	pos.x -= Speed;

	Position(pos);
}

void Rect_Control::MoveRight()
{
	Vector2 pos = Position();
	pos.x += Speed;

	Position(pos);
}

void Rect_Control::Movespeed(float val)
{
	Speed = val;
}
