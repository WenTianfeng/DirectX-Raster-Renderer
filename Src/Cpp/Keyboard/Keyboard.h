#pragma once

#include<queue>
#include"KeyboardEvent.h"

class Keyboard {
private:
	static Keyboard* keyboardInstance;
	Keyboard();

	bool autoRepeatKeys = false;
	bool autoRepeatChars = false;
	bool keyStates[256];

	std::queue<KeyboardEvent> keyBuffer;
	std::queue<unsigned char> charBuffer;

public:
	static Keyboard* GetInstance();

	bool KeyIsPressed(const unsigned char keycode);
	bool KeyBufferEmpty();
	bool CharBufferEmpty();
	void CleanEvent();


	unsigned char ReadChar();
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);
	void EnableAutoRepeatKeys();
	void DisableAutoRepeatKeys();
	void EnableAutoRepeatChars();
	void DisableAutoRepeatChars();
	bool IsKeysAutoRepeat();
	bool IsCharsAutoRepeat();



	

};