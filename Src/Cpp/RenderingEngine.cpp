#include "RenderingEngine.h"

RenderWindow* RenderingEngine::renderWindow = new RenderWindow();
SceneManager* RenderingEngine::sceneManager = new SceneManager();
Graphics* RenderingEngine::graphics = new Graphics();
Timer* RenderingEngine::timer = new Timer();

RenderingEngine::RenderingEngine():
	m_paused(false)
{


}

bool RenderingEngine::Initialize(HINSTANCE hInstance,std::string windowCaption,std::string windowClassName,int width,int height)
{
	//初始化窗口
	if (!renderWindow->Initialize(hInstance, windowCaption, windowClassName, width, height))
	{
		return false;
	}

	//初始化图形
	if (!graphics->Initialize(renderWindow->GetWindowHandle(), width, height)) 
	{
		return false;
	}

	//初始化场景管理器
	if (!sceneManager->Initialize(graphics->GetDirectXDevice(), graphics->GetDirectXDeviceContext()))
	{
		return false;
	}

	return true;
}

int RenderingEngine::Run()
{
	//消息处理
	MSG msg = { 0 };

	timer->Reset();

	while (msg.message != WM_QUIT) {

		//如果消息队列中有消息则进行处理
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {//否则执行应用逻辑
			timer->Tick();

			if (!m_paused) {

				//显示FPS和单帧时间
				renderWindow->DisplayFrameStats(timer->TotalTime());

				//更新
				Update(timer->DeltaTime());

				//渲染
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
	this->sceneManager->Update(dt);

}

void RenderingEngine::Render()
{
	this->graphics->RenderFrame(SceneManager::objects,SceneManager::lights);
}

