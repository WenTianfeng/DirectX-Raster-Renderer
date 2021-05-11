#include <memory>
#include <sstream>

#include "RenderWindow.h"
#include"..\Mouse\Mouse.h"
#include"..\Keyboard\Keyboard.h"
#include"..\Tools\DataTypeConverter.h"
#include"..\Tools\ErrorLogger.h"

#include"..\..\..\resource.h"//图标资源加载

RenderWindow::RenderWindow():
	m_hAppInstance(nullptr),
	m_hMainWindow(nullptr),
	m_mainWndCaption(L""),
	m_mainWndClassName(L""),
	m_clientWidth(800),
	m_clientHeight(600),
	m_minimized(false),
	m_maximized(false),
	m_resizing(false)
{
}

RenderWindow::~RenderWindow()
{
	if (this->m_hMainWindow != NULL) {
		UnregisterClass(this->m_mainWndClassName.c_str(), this->m_hAppInstance);
		DestroyWindow(this->m_hMainWindow);
	}
}

extern LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
	//wc.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));//加载自定义图标
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

void RenderWindow::OnWindowResize(int clientWidth, int clientHeight)
{
	this->m_clientWidth = clientWidth;
	this->m_clientHeight = clientHeight;
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

//设置鼠标的显隐和锁定
void RenderWindow::SetCursorShowLock(bool show, bool lock)
{
	ShowCursor(show);
	if (lock) {
		//将鼠标限制在窗口范围内
		RECT window_rect;
		GetClientRect(m_hMainWindow, &window_rect);

		POINT left_up_point = {};
		left_up_point.x = window_rect.left;
		left_up_point.y = window_rect.top;

		POINT right_bottom_point = {};
		right_bottom_point.x = window_rect.right;
		right_bottom_point.y = window_rect.bottom;

		//将窗口坐标系的对角线端点坐标映射到屏幕坐标系
		MapWindowPoints(m_hMainWindow, nullptr, &left_up_point, 1);
		MapWindowPoints(m_hMainWindow, nullptr, &right_bottom_point, 1);

		window_rect.left = left_up_point.x;
		window_rect.top = left_up_point.y;
		window_rect.right = right_bottom_point.x;
		window_rect.bottom = right_bottom_point.y;

		ClipCursor(&window_rect);//限制鼠标范围在窗口矩形内
	}
	else {
		ClipCursor(NULL);
	}
}


#pragma region Get/Set Functions

HWND RenderWindow::GetWindowHandle()
{
	return this->m_hMainWindow;
}

int RenderWindow::GetWindowWidth()
{
	return this->m_clientWidth;
}

int RenderWindow::GetWindowHeight()
{
	return this->m_clientHeight;
}

bool RenderWindow::IsWindowMinimized()
{
	return this->m_minimized;
}

bool RenderWindow::IsWindowMaximized()
{
	return this->m_maximized;
}

bool RenderWindow::isWindowResizing()
{
	return this->m_resizing;
}

void RenderWindow::SetWindowMinimized(bool minimized)
{
	this->m_minimized = minimized;
}

void RenderWindow::SetWindowMaximized(bool maximized)
{
	this->m_maximized = maximized;
}

void RenderWindow::SetWindowResizing(bool resizing)
{
	this->m_resizing = resizing;
}

#pragma endregion



