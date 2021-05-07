#pragma once

#include <d3d11_1.h>//DirectX 11.1������ã�https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/��
#include <wrl/client.h>//ComPtr����ָ��������� ��https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160��

#include"Shader.h"

class Texture
{
public:
	Texture();

	/// <summary>
	/// ʵ��������
	/// </summary>
	/// <param name="dxDevice">DirectX�豸</param>
	/// <returns></returns>
	bool Instantiate(ID3D11Device* dxDevice);

	void Bind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType);



private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

};