#pragma once

#include <d3d11_1.h>//DirectX 11.1������ã�https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/��
#include <wrl/client.h>//ComPtr����ָ��������� ��https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160��

#include"Shader.h"

class SamplerState
{
public:
	/// <summary>
	/// ���캯��
	/// </summary>
	/// <param name="slot">���������ڼĴ����Ĳ�λ</param>
	SamplerState(UINT slot);

	/// <summary>
	/// ʵ����������
	/// </summary>
	/// <param name="dxDevice">DirectX�豸</param>
	void Instantiate(ID3D11Device* dxDevice);

	/// <summary>
	/// �󶨲�������DirectX������ָ����ɫ��
	/// </summary>
	/// <param name="deviceContext">DirectX������</param>
	/// <param name="shaderType">��ɫ������</param>
	void Bind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType);

	/// <summary>
	/// ���DirectX������ָ����ɫ���Ĳ�����
	/// </summary>
	/// <param name="deviceContext">DirectX������</param>
	/// <param name="shaderType">��ɫ������</param>
	void UnBind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType);

private:

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samplerState;//DirectX������
	UINT m_slot;//�������ڼĴ����Ĳ�λ


};