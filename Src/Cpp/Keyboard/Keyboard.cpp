#include "Keyboard.h"

Keyboard* Keyboard::keyboardInstance = nullptr;

Keyboard::Keyboard()
{
	for (int i = 0; i < 256; i++)
	{
		this->keyStates[i] = false;
	}
}

Keyboard* Keyboard::GetInstance()
{
	if (keyboardInstance == nullptr)  //判断是否第一次调用
		keyboardInstance = new Keyboard();
	return keyboardInstance;
}


bool Keyboard::KeyIsPressed(const unsigned char keycode)
{
	return this->keyStates[keycode];
}

bool Keyboard::KeyBufferEmpty()
{
	return keyBuffer.empty();
}

bool Keyboard::CharBufferEmpty()
{
	return charBuffer.empty();
}

void Keyboard::CleanEvent()
{
	while (!keyBuffer.empty())
	{
		keyBuffer.pop();
	}
}

unsigned char Keyboard::ReadChar()
{
	if (this->charBuffer.empty()) {
		return 0u;
	}
	else {
		unsigned char c = this->charBuffer.front();
		this->charBuffer.pop();
		return c;
	}

}

void Keyboard::OnKeyPressed(const unsigned char key)
{
	this->keyStates[key] = true;
	this->keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));

}

void Keyboard::OnKeyReleased(const unsigned char key)
{
	this->keyStates[key] = false;
	this->keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void Keyboard::OnChar(const unsigned char key)
{
	this->charBuffer.push(key);

}

void Keyboard::EnableAutoRepeatKeys()
{
	this->autoRepeatKeys = true;
}

void Keyboard::DisableAutoRepeatKeys()
{
	this->autoRepeatKeys = false;
}

void Keyboard::EnableAutoRepeatChars()
{
	this->autoRepeatChars = true;
}

void Keyboard::DisableAutoRepeatChars()
{
	this->autoRepeatChars = false;
}

bool Keyboard::IsKeysAutoRepeat()
{
	return autoRepeatKeys;
}

bool Keyboard::IsCharsAutoRepeat()
{
	return autoRepeatChars;
}

