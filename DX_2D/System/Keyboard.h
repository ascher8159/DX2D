#pragma once

//�ƽ�Ű �ڵ� 128 2�� (Ȥ�� �� ���)
#define KEYMAX 256

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	bool Down(int key);		// Ű �Է� ���� ����
	bool Up(int key);		// Ű �Է¿��� ���� ����
	bool Press(int key);	// Ű �Է��� �ϰ� ���� ����
	bool Toggle(int key);	// �Է��� �ϸ� On / Off 

private:
	bitset<KEYMAX> up;     
	bitset<KEYMAX> down;
};