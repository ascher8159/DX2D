#include "stdafx.h"
#include "Rect_Control.h"

Rect_Control::Rect_Control(Shader* shader) 
	: Rect(shader){}

Rect_Control::Rect_Control(Shader* shader, Vector2 position, Vector2 scale, D3DXCOLOR color)
	: Rect(shader, position, scale, color){}

void Rect_Control::Update(Matrix& V, Matrix P)
{
	//�θ� �ѹ� ȣ��
	//Rect::Update(V, P);
	__super::Update(V, P);

	Vector2 pos = Position();
	
	if (Ground == false)
	{
		velocity -= gravity * Time::Delta(); // �����¿� �߷¸�ŭ ��
		pos.y += velocity;
	}

	//�ٴ� ������ �ʱ�ȭ��
	if (pos.y < Scale().y * 0.5f) // �ڱ� ũ���� ���� ��ŭ ���� �ɸ��� ����
	{
		Ground = true;
		velocity = 0.0f;
		pos.y = Scale().y * 0.5f;
	}

	Position(pos);

	//��¿�
	ImGui::LabelText("Delta Time", "%.5f", Time::Delta());
	ImGui::LabelText("Velocity", "%.1f", velocity);
	ImGui::LabelText("Rect Center ", "%.1f", pos.y);
	ImGui::LabelText("Ground ", "%d", Ground);
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

void Rect_Control::Jump()
{
	if (Ground == true)
	{
		velocity = 0.35f;
		Ground = false; 
	}
}

void Rect_Control::StopJump()
{
	if (velocity > 0)
		velocity = 0.0f;
}

void Rect_Control::Movespeed(float val){	Speed = val;}
