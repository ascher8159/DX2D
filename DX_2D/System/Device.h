#include "stdafx.h"

//Global Scene Function
void InitScene();
void DestroyScene();
void Update();
void Render();

//Global Function
void InitWindow(HINSTANCE hInstance, int nCmdShow);							 // ������ Ŭ���� ���� ����
void InitDirct3D(HINSTANCE hInstance);		 								 // dx���� ���� ����
void Destroy();																 // �Ҵ� ���� ������ ��ȯ
WPARAM Running();															 // Win32API �۾�
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); // Win32API �̺�Ʈ