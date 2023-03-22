#include "stdafx.h"

//Global Scene Function
void InitScene();
void DestroyScene();
void Update();
void Render();

//Global Function
void InitWindow(HINSTANCE hInstance, int nCmdShow);							 // 윈도우 클래스 관련 셋팅
void InitDirct3D(HINSTANCE hInstance);		 								 // dx변수 관련 셋팅
void Destroy();																 // 할당 받은 변수들 반환
WPARAM Running();															 // Win32API 작업
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); // Win32API 이벤트