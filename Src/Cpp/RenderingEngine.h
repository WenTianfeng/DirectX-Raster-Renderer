#pragma once

#include"Time\Timer.h"
#include"Windows\RenderWindow.h"
#include"Graphics\Graphics.h"

class RenderingEngine 
{
public:

	RenderingEngine();

	/// <summary>
	/// 引擎相关内容初始化
	/// </summary>
	/// <param name="hInstance">程序句柄</param>
	/// <param name="windowCaption">窗口标题</param>
	/// <param name="windowClassName">窗口类注册名</param>
	/// <param name="width">窗口宽度</param>
	/// <param name="height">窗口高度</param>
	/// <returns>程序运行所需内容是否创建成功</returns>
	bool Initialize(HINSTANCE hInstance, std::string windowCaption, std::string windowClassName, int width, int height);

	/// <summary>
	/// 引擎程序运行
	/// </summary>
	/// <returns>程序运行结束返回值</returns>
	int Run();

private:

	/// <summary>
	/// 每帧更新
	/// </summary>
	/// <param name="dt">单帧所用时间</param>
	void Update(float dt);

	/// <summary>
	/// 每帧渲染
	/// </summary>
	void Render();

private:

	RenderWindow m_renderWindow;//引擎程序使用的窗口实例
	Graphics m_graphics;//引擎程序使用的渲染实例
	Timer m_timer;//计时器

	bool m_paused;//程序是否暂停
};