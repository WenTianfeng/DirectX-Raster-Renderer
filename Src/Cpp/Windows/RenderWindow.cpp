#include <memory>
#include <sstream>

#include "RenderWindow.h"
#include"..\Mouse\Mouse.h"
#include"..\Keyboard\Keyboard.h"
#include"..\Tools\DataTypeConverter.h"
#include"..\Tools\ErrorLogger.h"



RenderWindow::RenderWindow():
	m_hAppInstance(nullptr),
	m_hMainWindow(nullptr),
	m_mainWndCaption(L""),
	m_mainWndClassName(L""),
	m_clientWidth(800),
	m_clientHeight(600)
{
}

HWND RenderWindow::GetWindowHandle()
{
	return this->m_hMainWindow;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) {
		return true;
	}

	switch (msg)
	{

	case WM_CLOSE:
		PostQuitMessage(0);

		return 0;

	case WM_CHAR:
	{
		unsigned char c = static_cast<unsigned char>(wParam);
		if (Keyboard::GetInstance()->IsCharsAutoRepeat()) {
			Keyboard::GetInstance()->OnChar(c);
		}
		else {
			bool pressedBefore = lParam & 0x40000000;
			if (!pressedBefore) {
				Keyboard::GetInstance()->OnChar(c);
			}
		}

		return 0;
	}
	case WM_KEYDOWN:
	{
		unsigned char keycode_down = static_cast<unsigned char>(wParam);
		if (Keyboard::GetInstance()->IsKeysAutoRepeat()) {
			Keyboard::GetInstance()->OnKeyPressed(keycode_down);
		}
		else {
			bool pressedBefore = lParam & 0x40000000;
			if (!pressedBefore) {
				Keyboard::GetInstance()->OnKeyPressed(keycode_down);
			}
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char keycode_up = static_cast<unsigned char>(wParam);
		Keyboard::GetInstance()->OnKeyReleased(keycode_up);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance()->OnMouseMove(x, y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance()->OnLeftPressed(x, y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance()->OnRightPressed(x, y);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance()->OnMiddlePressed(x, y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance()->OnLeftReleased(x, y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance()->OnRightReleased(x, y);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		Mouse::GetInstance()->OnMiddleReleased(x, y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			Mouse::GetInstance()->OnWheelUp(x, y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			Mouse::GetInstance()->OnWheelDown(x, y);
		}
		return 0;
	}
	case WM_INPUT:
	{
		UINT dataSize = 0;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER)); //Need to populate data size first

		if (dataSize > 0)
		{
			std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
			{
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
				if (raw->header.dwType == RIM_TYPEMOUSE)
				{
					Mouse::GetInstance()->OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
				}
			}
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

bool RenderWindow::Initialize(HINSTANCE hInstance, std::string windowCaption, std::string windowClassName, int width, int height)
{
	this->m_hAppInstance = hInstance;
	this->m_mainWndCaption = DataTypeConverter::StringToWideString(windowCaption);
	this->m_mainWndClassName = DataTypeConverter::StringToWideString(windowClassName);
	this->m_clientWidth = width;
	this->m_clientHeight = height;

	//注册窗口类信息设定
	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = m_hAppInstance;
	wc.lpfnWndProc = MainWndProc;
	wc.lpszClassName = m_mainWndClassName.c_str();
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = NULL;

	//注册窗口类，如果失败，打印错误信息并返回
	if (!RegisterClass(&wc)) {
		ErrorLogger::Log(GetLastError(), "Failed to Register WNDCLASS.");
		return false;
	}

	//根据所需窗口渲染区域大小计算得到总的窗口大小（将窗口样式纳入计算）
	RECT R = { 0, 0, width, height };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int total_width = R.right - R.left;
	int total_height = R.bottom - R.top;


	//创建窗口并获取窗口句柄
	m_hMainWindow = CreateWindow(
		m_mainWndClassName.c_str(), m_mainWndCaption.c_str(), WS_OVERLAPPEDWINDOW,
		GetSystemMetrics(SM_CXSCREEN) / 2 - total_width / 2,	//将窗口初始化在屏幕中心
		GetSystemMetrics(SM_CYSCREEN) / 2 - total_height / 2,
		total_width, 
		total_height,
		NULL, NULL, m_hAppInstance, NULL);

	if (!m_hMainWindow)
	{
		ErrorLogger::Log(GetLastError(), "Failed to create window.");
		return false;
	}

	ShowWindow(m_hMainWindow, SW_SHOW);
	SetForegroundWindow(m_hMainWindow);
	UpdateWindow(m_hMainWindow);

	return true;
}

void RenderWindow::DisplayFrameStats(float totalTime)
{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if ((totalTime - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << m_mainWndCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(m_hMainWindow, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

RenderWindow::~RenderWindow()
{
	if (this->m_hMainWindow != NULL) {
		UnregisterClass(this->m_mainWndClassName.c_str(), this->m_hAppInstance);
		DestroyWindow(this->m_hMainWindow);
	}
}




