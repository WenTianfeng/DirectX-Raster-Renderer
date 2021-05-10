#pragma once

#include <d3d11_1.h>//DirectX 11.1相关引用（https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/）
#include <wrl/client.h>//ComPtr智能指针相关引用 （https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160）
#include<DirectXTK\WICTextureLoader.h>

#include"Shader.h"

class Texture
{
public:
	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="dxDevice">DirectX设备</param>
	/// <param name="slot">纹理资源所在寄存器槽位</param>
	Texture(ID3D11Device* dxDevice, UINT slot);
	
	/// <summary>
	/// 纹理实例化
	/// </summary>
	/// <param name="textureFileName"></param>
	void Instantiate(std::string textureFilePath);

	/// <summary>
	/// 绑定纹理至DirectX上下文指定着色器
	/// </summary>
	/// <param name="deviceContext">DirectX上下文</param>
	/// <param name="shaderType">着色器类型</param>
	void Bind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType);

	/// <summary>
	/// 解绑DirectX上下文指定着色器类型的纹理
	/// </summary>
	/// <param name="deviceContext">DirectX上下文</param>
	/// <param name="shaderType">着色器类型</param>
	void UnBind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType);

	/// <summary>
	/// 获取纹理文件路径
	/// </summary>
	/// <returns>纹理路径字符串</returns>
	std::string GetTextureFilePath();

	/// <summary>
	/// 获取DirectX着色器资源视图
	/// </summary>
	/// <returns>着色器资源视图</returns>
	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	ID3D11Device* m_dxDevice;//DirectX设备
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;//DirectX纹理对象
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;//DirectX着色器资源视图

	UINT m_slot;//纹理在寄存器的槽位
	std::string m_textureFilePath;//着色器纹理文件路径

};