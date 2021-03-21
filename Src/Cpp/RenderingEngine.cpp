#include "RenderingEngine.h"

RenderingEngine::RenderingEngine():
	m_paused(false)
{


}

bool RenderingEngine::Initialize(HINSTANCE hInstance,std::string windowCaption,std::string windowClassName,int width,int height)
{
	//初始化窗口
	if (!m_renderWindow.Initialize(hInstance, windowCaption, windowClassName, width, height))
	{
		return false;
	}

	//初始化图形
	if (!m_graphics.Initialize(m_renderWindow.GetWindowHandle(), width, height)) 
	{
		return false;
	}



	return true;
}

int RenderingEngine::Run()
{
	//消息处理
	MSG msg = { 0 };

	m_timer.Reset();

	while (msg.message != WM_QUIT) {

		//如果消息队列中有消息则进行处理
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {//否则执行应用逻辑
			m_timer.Tick();

			if (!m_paused) {

				//显示FPS和单帧时间
				m_renderWindow.DisplayFrameStats(m_timer.TotalTime());

				Update(m_timer.DeltaTime());

				Render();

			}
			else {
				Sleep(100);
			}
		}
	}

	return 0;
}

void RenderingEngine::Update(float dt)
{


}

void RenderingEngine::Render()
{
	m_graphics.RenderFrame();

}
