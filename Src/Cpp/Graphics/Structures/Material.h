#pragma once

#include<vector>
#include<map>

#include"Shader.h"
#include"ShaderParameter.h"
#include"ConstantBufferTypes.h"


class Material 
{
public:
	/// <summary>
	/// ���캯��
	/// </summary>
	/// <param name="device">DirectX�豸</param>
	Material(ID3D11Device* device);

	/// <summary>
	/// ����ʵ����
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderFilePath">��ɫ���ļ�·����������ɫ�������ļ���</param>
	/// <returns></returns>
	bool Instantiate(std::string shaderFilePath);

	/// <summary>
	/// ��ȡ������ɫ���ļ�·��
	/// </summary>
	/// <returns></returns>
	std::string GetShaderFilePath();

	/// <summary>
	/// ��ȡָ��������ɫ��
	/// </summary>
	/// <param name="shaderType">��ɫ������</param>
	/// <returns>��ɫ��ָ��</returns>
	Shader* GetShader(Shader::ShaderType shaderType) const;

	/// <summary>
	/// ��ȡ���벼��
	/// </summary>
	/// <returns>���벼��ָ��</returns>
	ID3D11InputLayout* GetInputLayout() const;

	/// <summary>
	/// ��ȡ ���-ģ�� ״̬
	/// </summary>
	/// <returns>���-ģ�� ״ָ̬��</returns>
	ID3D11DepthStencilState* GetDepthStencilState() const;

	/// <summary>
	/// ��ȡ���״̬
	/// </summary>
	/// <returns>���״ָ̬��</returns>
	ID3D11BlendState* GetBlendState() const;

	/// <summary>
	/// ��������ɫ����DirectX������
	/// </summary>
	/// <param name="deviceContext">DirectX������</param>
	void BindShaders(ID3D11DeviceContext* deviceContext);


public:
	std::map<std::string, ShaderParameter*> shaderParametersMap;	//<��ɫ��������-����>map

private:
	ID3D11Device* m_dxDevice;//DirectX �豸
	std::string m_shaderFilePath;//��ɫ���ļ�·��
	std::map<Shader::ShaderType, Shader*> m_shadersMap;//<��ɫ������-��ɫ��> map

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_dxInputLayout;//DirectX���벼�ֶ���
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_dxDepthStencilState;//���ģ��״̬
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_dxBlendState;//���״̬
};