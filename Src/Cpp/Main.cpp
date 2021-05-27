#include"RenderingEngine.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);
	// 允许在Debug版本进行运行时内存分配和泄漏检测
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	try
	{
		//渲染引擎初始化
		RenderingEngine::GetInstance()->Initialize(hInstance, "StemCell Engine", "WndClassName", 1200, 800);
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return -1;
	}
	//渲染引擎运行
	return RenderingEngine::GetInstance()->Run();
	
}