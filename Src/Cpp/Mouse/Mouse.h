#pragma once

#include<queue>
#include <wrl/client.h>

#include"MouseEvent.h"

class Mouse {

private:
	static Mouse* mouseInstance;
	Mouse();

	std::queue<MouseEvent> eventBuffer;
	bool leftIsDown = false;
	bool rightIsDown = false;
	bool mbuttonDown = false;

	int x = 0;
	int y = 0;

public:
	static Mouse* GetInstance();

	std::queue<MouseEvent> GetEventBuffer();

	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseMoveRaw(int x, int y);

	bool IsLeftDown();
	bool IsMiddleDown();
	bool IsRightDown();

	int GetPosX();
	int GetPosY();
	MousePoint GetPos();

	//消息队列是否为空
	bool EventBufferIsEmpty();
	//清理消息队列
	void CleanEvent();

};