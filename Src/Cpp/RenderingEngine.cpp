#include "RenderingEngine.h"
#include"Mouse\Mouse.h"
#include"Keyboard\Keyboard.h"

RenderingEngine* RenderingEngine::m_instance = nullptr;

RenderingEngine::RenderingEngine():
	m_paused(false),
	renderWindow(nullptr),
	sceneManager(nullptr),
	graphics(nullptr),
	timer(nullptr)
{
	renderWindow = new RenderWindow();
	sceneManager = new SceneManager();
	graphics = new Graphics();
	timer = new Timer();

}

RenderingEngine::~RenderingEngine()
{
	
}

RenderingEngine* RenderingEngine::GetInstance()
{
	if (m_instance == nullptr)  //判断是否第一次调用
		m_instance = new RenderingEngine();
	return m_instance;
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

				//每一帧结束时清除键盘和鼠标接收到的事件
				Mouse::GetInstance()->CleanEvent();
				Keyboard::GetInstance()->CleanEvent();

			}
			else {
				Sleep(100);
			}
		}
	}

	return 0;
}


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return RenderingEngine::GetInstance()->EngineMsgProc(hwnd, msg, wParam, lParam);
}


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT RenderingEngine::EngineMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) {
		return true;
	}

	switch (msg)
	{

	case WM_CLOSE:
		PostQuitMessage(0);

		return 0;

	case WM_SIZE:
	{

		int newClientWidth = LOWORD(lParam);
		int newClientHeight = HIWORD(lParam);

		this->renderWindow->OnWindowResize(newClientWidth, newClientHeight);

		//当窗口最小化
		if (wParam == SIZE_MINIMIZED)
		{
			m_paused = true;
			renderWindow->SetWindowMaximized(false);
			renderWindow->SetWindowMinimized(true);
		}

		//当窗口最大化
		else if (wParam == SIZE_MAXIMIZED)
		{
			m_paused = false;
			renderWindow->SetWindowMaximized(true);
			renderWindow->SetWindowMinimized(false);
			this->graphics->OnWindowResize(newClientWidth, newClientHeight);
			this->sceneManager->OnWindowResize(newClientWidth, newClientHeight);

		}

		//当窗口大小改变
		else if (wParam == SIZE_RESTORED)
		{
			//当窗口由最小化状态缩放
			if (renderWindow->IsWindowMinimized())
			{
				m_paused = false;
				renderWindow->SetWindowMinimized(false);
				this->graphics->OnWindowResize(newClientWidth, newClientHeight);
				this->sceneManager->OnWindowResize(newClientWidth, newClientHeight);
			}

			//当窗口由最大化状态缩放
			else if (renderWindow->IsWindowMaximized())
			{
				m_paused = false;
				renderWindow->SetWindowMaximized(false);
				this->graphics->OnWindowResize(newClientWidth, newClientHeight);
				this->sceneManager->OnWindowResize(newClientWidth, newClientHeight);
			}

			//当正在进行窗口缩放时
			else if (renderWindow->isWindowResizing())
			{
				//不在此处执行Graphics的OnWindowResize方法，因为当用户拖拽窗口进行缩放时，
				//这里会持续调用Graphics的OnWindowResize方法，导致卡顿
			}
			else
			{
				this->graphics->OnWindowResize(newClientWidth, newClientHeight);
			}
		}
	}
		return 0;

	//当开始窗口缩放时
	case WM_ENTERSIZEMOVE:

		m_paused = true;
		renderWindow->SetWindowResizing(true);
		timer->Pause();
		return 0;

	//当停止窗口缩放时
	case WM_EXITSIZEMOVE:
	{
		m_paused = false;
		renderWindow->SetWindowResizing(false);
		timer->Start();
		this->graphics->OnWindowResize(renderWindow->GetWindowWidth(), renderWindow->GetWindowHeight());
		this->sceneManager->OnWindowResize(renderWindow->GetWindowWidth(), renderWindow->GetWindowHeight());
	}
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

void RenderingEngine::Update(float dt)
{
	this->sceneManager->Update(dt);

}

void RenderingEngine::Render()
{
	this->graphics->RenderFrame(SceneManager::objects,SceneManager::lights);
}
