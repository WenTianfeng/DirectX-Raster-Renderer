/********************************************************************************
  * @file    Graphics.h
  * @author  WenTianfeng
  * @version v1.0
  * @date    25-2-2021
  * @brief   Class Graphics`s definition and members` declaration
  ******************************************************************************
  * @attention 
  ******************************************************************************
  */

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
#include<DirectXTK\WICTextureLoader.h> //纹理加载库


#include"Structures\Shader.h"
#include"Structures\VertexBuffer.h"
#include"Structures\IndexBuffer.h"
#include"Structures\VertexTypes.h"
#include"Structures\ConstantBuffer.h"
#include"Structures\ConstantBufferTypes.h"
#include"..\Tools\ErrorLogger.h"

#include"UI\ImGui\imgui.h"
#include"UI\ImGui\imgui_impl_dx11.h"
#include"UI\ImGui\imgui_impl_win32.h"


class Graphics 
{

public:

	/// <summary>
	/// 图形处理初始化——当前：DirectX初始化
	/// </summary>
	/// <param name="hwnd">窗口句柄</param>
	/// <param name="width">窗口宽度</param>
	/// <param name="height">窗口高度</param>
	/// <returns>所有相关内容是否初始化成功</returns>
	bool Initialize(HWND hwnd, int width, int height);

	/// <summary>
	/// 渲染
	/// </summary>
	void RenderFrame();

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
	/// 着色器和特效相关的初始化
	/// </summary>
	/// <returns>相关内容是否初始化成功</returns>
	bool InitializeEffect();

	/// <summary>
	/// 场景内容初始化
	/// </summary>
	/// <returns>相关内容是否初始化成功</returns>
	bool InitializeScene();

	/// <summary>
	/// ImGui初始化
	/// </summary>
	/// <param name="hwnd">窗口句柄</param>
	/// <returns>相关内容是否初始化成功</returns>
	bool InitializeUI(HWND hwnd);


	Microsoft::WRL::ComPtr<ID3D11Device> m_dxDevice;//Dx设备
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_dxSwapChain;//Dx交换链
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_dxDeviceContext;//Dx上下文
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_dxRenderTargetView;//Dx渲染目标视图
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dxDepthStencilView;//深度模板视图
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_dxDepthStencilBuffer;//深度模板缓冲
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_dxDepthStencilState;//深度模板状态
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_dxRasterizerState;//光栅化状态
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_dxBlendState;//混合状态

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_mainTexResourceView;//着色器纹理资源
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_dxSamplerState;//纹理采样器
	

	VertexShader m_vertexShader; //顶点着色器	
	PixelShader m_pixelShader; //像素着色器
	VertexBuffer<Vertex_PosTex> m_vertexBuffer; //顶点缓冲
	IndexBuffer<DWORD> m_indexBuffer; //索引缓冲
	ConstantBuffer<CB_VS_Offset> m_constantBuffer; //常量缓冲


};