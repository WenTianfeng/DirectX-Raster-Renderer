#pragma once

#pragma comment(lib, "D3DCompiler.lib")

#include<string>
#include<d3dcompiler.h>//HLSL shader相关编译用 （https://docs.microsoft.com/en-us/windows/win32/api/d3dcompiler/）
#include <wrl/client.h>//ComPtr智能指针相关引用 （https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160）
#include <d3d11_1.h>//DirectX 11.1相关引用（https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/）

#include"..\..\Tools\ErrorLogger.h"

/// <summary>
/// 顶点着色器类封装
/// </summary>
class VertexShader 
{
public:
	/// <summary>
	/// 顶点着色器初始化
	/// </summary>
	/// <param name="dxDevice">DirectX 设备</param>
	/// <param name="shaderFilePath">着色器文件路径</param>
	/// <param name="inputLayoutDescs">输入布局列表头指针</param>
	/// <param name="numElements">输入布局元素个数</param>
	/// <returns></returns>
	bool Initialize(ID3D11Device* dxDevice , std::wstring shaderFilePath);

	ID3D11VertexShader* GetShader();
	ID3DBlob* GetBlob();

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;//DirectX顶点着色器
	Microsoft::WRL::ComPtr<ID3DBlob> m_blobBuffer;

};

/// <summary>
/// 像素着色器类封装
/// </summary>
class PixelShader
{
public:
	/// <summary>
	/// 像素着色器初始化
	/// </summary>
	/// <param name="dxDevice">DirectX 设备</param>
	/// <param name="shaderFilePath">着色器文件路径</param>
	/// <returns></returns>
	bool Initialize(ID3D11Device* dxDevice, std::wstring shaderFilePath);

	ID3D11PixelShader* GetShader();
	ID3DBlob* GetBlob();

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;//DirectX像素着色器
	Microsoft::WRL::ComPtr<ID3DBlob> m_blobBuffer;

};



