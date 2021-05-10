#pragma once

#include <d3d11_1.h>//DirectX 11.1相关引用（https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/）
#include <wrl/client.h>//ComPtr智能指针相关引用 （https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160）

#include"Shader.h"

class SamplerState
{
public:
	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="slot">采样器所在寄存器的槽位</param>
	SamplerState(UINT slot);

	/// <summary>
	/// 实例化采样器
	/// </summary>
	/// <param name="dxDevice">DirectX设备</param>
	void Instantiate(ID3D11Device* dxDevice);

	/// <summary>
	/// 绑定采样器至DirectX上下文指定着色器
	/// </summary>
	/// <param name="deviceContext">DirectX上下文</param>
	/// <param name="shaderType">着色器类型</param>
	void Bind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType);

	/// <summary>
	/// 解绑DirectX上下文指定着色器的采样器
	/// </summary>
	/// <param name="deviceContext">DirectX上下文</param>
	/// <param name="shaderType">着色器类型</param>
	void UnBind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType);

private:

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;//DirectX采样器
	UINT m_slot;//采样器在寄存器的槽位


};