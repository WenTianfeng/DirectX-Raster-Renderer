#pragma once

#include <wrl/client.h>//ComPtr智能指针相关引用 （https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160）
#include<string>

class RenderWindow 
{
public:
	RenderWindow();
	~RenderWindow();

	/// <summary>
	/// 窗口初始化——创建Windows窗口
	/// </summary>
	/// <param name="hInstance">程序句柄</param>
	/// <param name="windowCaption">窗口标题</param>
	/// <param name="windowClassName">窗口类注册名</param>
	/// <param name="width">窗口宽度</param>
	/// <param name="height">窗口高度</param>
	/// <returns>窗口是否创建成功</returns>
	bool Initialize(HINSTANCE hInstance,std::string windowCaption,std::string windowClassName,int width,int height);

	void OnWindowResize(int clientWidth, int clientHeight);

	/// <summary>
	/// 在窗口标题栏显示FPS和单帧时间
	/// </summary>
	/// <param name="totalTime"></param>
	void DisplayFrameStats(float totalTime);

	//设置鼠标的显隐和锁定
	void SetCursorShowLock(bool show, bool lock);

	//Get/Set方法
	#pragma region Get/Set Functions

	/// <summary>
	/// 获取窗口句柄
	/// </summary>
	/// <returns>HWND窗口句柄</returns>
	HWND GetWindowHandle();

	int GetWindowWidth();
	int GetWindowHeight();

	bool IsWindowMinimized();
	bool IsWindowMaximized();
	bool isWindowResizing();

	void SetWindowMinimized(bool minimized);
	void SetWindowMaximized(bool maximized);
	void SetWindowResizing(bool resizing);

	#pragma endregion

private:
	HINSTANCE m_hAppInstance;//应用程序句柄
	HWND m_hMainWindow;//窗口句柄

	std::wstring m_mainWndCaption = L"";//窗口标题
	std::wstring m_mainWndClassName = L"";//WNDCLASS 名

	int m_clientWidth;//窗口宽度
	int m_clientHeight;//窗口高度

	bool m_minimized;
	bool m_maximized;
	bool m_resizing;
};
