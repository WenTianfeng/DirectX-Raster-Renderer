﻿#include"RenderingEngine.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance, _In_ LPSTR cmdLine, _In_ int showCmd) {
	// 这些参数不使用
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(showCmd);
	// 允许在Debug版本进行运行时内存分配和泄漏检测
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif


	RenderingEngine render_engine;

	try
	{
		//引擎初始化
		render_engine.Initialize(hInstance, "Wind Rendering Engine v1.0", "WndClassName", 1200, 800);
	}
	catch (COMException& exception)
	{
		ErrorLogger::Log(exception);
		return -1;
	}
	//引擎执行
	return render_engine.Run();
	

}