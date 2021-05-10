#pragma once

#include <d3d11_1.h>//DirectX 11.1������ã�https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/��
#include <wrl/client.h>//ComPtr����ָ��������� ��https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160��
#include<DirectXTK\WICTextureLoader.h>

#include"Shader.h"

class Texture
{
public:
	/// <summary>
	/// ���캯��
	/// </summary>
	/// <param name="dxDevice">DirectX�豸</param>
	/// <param name="slot">������Դ���ڼĴ�����λ</param>
	Texture(ID3D11Device* dxDevice, UINT slot);
	
	/// <summary>
	/// ����ʵ����
	/// </summary>
	/// <param name="textureFileName"></param>
	void Instantiate(std::string textureFilePath);

	/// <summary>
	/// ��������DirectX������ָ����ɫ��
	/// </summary>
	/// <param name="deviceContext">DirectX������</param>
	/// <param name="shaderType">��ɫ������</param>
	void Bind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType);

	/// <summary>
	/// ���DirectX������ָ����ɫ�����͵�����
	/// </summary>
	/// <param name="deviceContext">DirectX������</param>
	/// <param name="shaderType">��ɫ������</param>
	void UnBind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType);

	/// <summary>
	/// ��ȡ�����ļ�·��
	/// </summary>
	/// <returns>����·���ַ���</returns>
	std::string GetTextureFilePath();

	/// <summary>
	/// ��ȡDirectX��ɫ����Դ��ͼ
	/// </summary>
	/// <returns>��ɫ����Դ��ͼ</returns>
	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	ID3D11Device* m_dxDevice;//DirectX�豸
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texture2D;//DirectX�������
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;//DirectX��ɫ����Դ��ͼ

	UINT m_slot;//�����ڼĴ����Ĳ�λ
	std::string m_textureFilePath;//��ɫ�������ļ�·��

};