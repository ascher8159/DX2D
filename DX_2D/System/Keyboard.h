#pragma once

//아스키 코드 128 2배 (혹시 모를 대비)
#define KEYMAX 256

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	bool Down(int key);		// 키 입력 했을 때만
	bool Up(int key);		// 키 입력에서 땟을 때만
	bool Press(int key);	// 키 입력을 하고 있을 때만
	bool Toggle(int key);	// 입력을 하면 On / Off 

private:
	bitset<KEYMAX> up;     
	bitset<KEYMAX> down;
};