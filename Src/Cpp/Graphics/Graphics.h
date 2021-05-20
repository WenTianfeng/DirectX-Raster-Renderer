#pragma once

// 添加要使用的动态链接库（也可以在项目属性的连接器附加依赖项中手动添加）
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"DirectXTK.lib")

#include <wrl/client.h>//ComPtr智能指针相关引用 （https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160）
#include <d3d11_1.h>//DirectX 11.1相关引用（https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/）
#include<DirectXMath.h>

#include<vector>

#include"..\Tools\ErrorLogger.h"
#include"..\Logic\Object.h"

#include"Structures\StructuredBuffer.h"
#include"Structures\StructuredBufferTypes.h"

#include"..\UI\Frames\UserInterface.h"


class Graphics 
{
public:

	Graphics();
	~Graphics();


	/// <summary>
	/// 图形处理初始化——当前：DirectX初始化、ImGui 初始化
	/// </summary>
	/// <param name="hwnd">窗口句柄</param>
	/// <param name="width">窗口宽度</param>
	/// <param name="height">窗口高度</param>
	/// <returns>所有相关内容是否初始化成功</returns>
	bool Initialize(HWND hwnd, int width, int height, SceneManager* sceneManager);

	/// <summary>
	/// 帧渲染
	/// </summary>
	/// <param name="objects">对象列表（从SceneManager获取）</param>
	void Render();

	/// <summary>
	/// 窗口缩放后调用，根据新窗口大小修改DirectX资源属性
	/// </summary>
	/// <param name="clientWidth">新窗口宽度</param>
	/// <param name="clientHeight">新窗口高度</param>
	void OnWindowResize(int clientWidth, int clientHeight);

	//获取DirectX设备
	ID3D11Device* GetDirectXDevice();

	//获取DirectX上下文
	ID3D11DeviceContext* GetDirectXDeviceContext();

private:

	/// <summary>
	/// 初始化DirecX——DirectX Device、SwapChain、m_dxDeviceContext、RenderTargetView等
	/// </summary>
	/// <param name="hwnd">窗口句柄</param>
	/// <param name="width">窗口宽度</param>
	/// <param name="height">窗口高度</param>
	/// <returns>DirectX各项是否初始化成功</returns>
	bool InitializeDirectX(HWND hwnd,int width,int height);

	/// <summary>
	/// 初始化渲染所需的相关成员
	/// </summary>
	/// <returns></returns>
	bool InitializeEffect();

	/// <summary>
	/// 更新渲染通用成员状态
	/// </summary>
	/// <param name="lights">光源列表</param>
	void UpdateEffect(std::vector<Object*> lights);

	/// <summary>
	/// ImGui初始化
	/// </summary>
	/// <param name="hwnd">窗口句柄</param>
	/// <returns>相关内容是否初始化成功</returns>
	bool InitializeUI(HWND hwnd, SceneManager* sceneManager);



private:
	//DirectX 相关成员
	Microsoft::WRL::ComPtr<ID3D11Device> m_dxDevice;//Dx设备
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_dxSwapChain;//Dx交换链
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_dxDeviceContext;//Dx上下文
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_dxRenderTargetView;//Dx渲染目标视图
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dxDepthStencilView;//深度模板视图
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_dxDepthStencilBuffer;//深度模板缓冲
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_dxRasterizerState;//光栅化状态
	float m_renderTargetBackgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1 };//帧缓冲默认刷新颜色

	//渲染相关通用成员
	StructuredBuffer<SB_PS_Light> m_lightSB;
	
private:
	SceneManager* m_sceneManager;
	UserInterface* m_userInterface = new UserInterface();//UI对象

};