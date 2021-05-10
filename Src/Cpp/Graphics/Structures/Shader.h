#pragma once

#include<string>//wstring
#include <d3d11_1.h>//DirectX 11.1������ã�https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/��
#include <wrl/client.h>//ComPtr����ָ��������� ��https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160��

#include"..\Helpers\ShaderProcessor.h"	//CreateShaderFromFile ��ɫ��������ط���

class Shader
{
/// <summary>
/// ��ɫ������ö��
/// </summary>
public:
	enum class ShaderType
	{
		UnknownShaderType = 0,
		VertexShader = 1,
		PixelShader  = 2,
		TessellationControlShader = 3,      // Hull Shader in DirectX
		TessellationEvaluationShader = 4,   // Domain Shader in DirectX
		GeometryShader = 5,
		ComputeShader = 6
	};

public:
	/// <summary>
	/// ���캯��
	/// </summary>
	/// <param name="shaderType">��ɫ������</param>
	/// <param name="shaderFilePath">��ɫ���ļ�·��</param>
	/// <param name="entryPoint">��ɫ��������ڵ�</param>
	/// <param name="shaderModel">��ɫģ��</param>
	Shader(ShaderType shaderType, std::string shaderFilePath, std::string entryPoint, std::string shaderModel);

	/// <summary>
	/// ��ɫ��ʵ����
	/// </summary>
	/// <param name="dxDevice">DirectX�豸</param>
	/// <returns>�Ƿ�ʵ�����ɹ�</returns>
	bool Instantiate(ID3D11Device* dxDevice);

	/// <summary>
	/// ����ɫ����DirectX������
	/// </summary>
	/// <param name="deviceContext">DirectX������</param>
	void Bind(ID3D11DeviceContext* deviceContext);

	/// <summary>
	/// ��ȡ��ɫ�������Ķ���������
	/// </summary>
	/// <returns>����������ָ��</returns>
	ID3DBlob* GetBlob();

private:
	/// <summary>
	/// Ԥ������������ɫ��ָ�룬��������ѡ����ʵ����
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11HullShader> m_hullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader> m_domainShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_computeShader;

	std::string m_shaderFilePath;//��ɫ���ļ�·��
	ShaderType m_shaderType = ShaderType::UnknownShaderType;//��ɫ������
	std::string m_entryPoint;//��ɫ��������ں���
	std::string m_shaderModel;//��ɫģ�Ͱ汾��
	Microsoft::WRL::ComPtr<ID3DBlob> m_blob;//��ɫ���������������ݶ���

};