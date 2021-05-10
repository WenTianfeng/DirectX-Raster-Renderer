#pragma once

#include<memory>
#include<vector>
#include<map>
#include <wrl/client.h>
#include <d3d11_1.h>

#include"Shader.h"
#include"ConstantBufferVariable.h"

#include"..\..\Tools\ErrorLogger.h"


class ConstantBuffer
{
public:
	/// <summary>
	/// ���캯��
	/// </summary>
	/// <param name="slot">�����������ڵļĴ�����λ</param>
	/// <param name="byteWidth">���������ֽڴ�С</param>
	ConstantBuffer(UINT slot, UINT byteWidth);

	/// <summary>
	/// ʵ������������
	/// </summary>
	/// <param name="dxDevice">DirectX�豸</param>
	void Instantiate(ID3D11Device* dxDevice);

	/// <summary>
	/// �������������DirectX������
	/// </summary>
	/// <param name="deviceContext">DirectX������</param>
	/// <param name="bufferSlot">�Ĵ�����λ</param>
	/// <param name="shaderType">��ɫ������</param>
	void Bind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType);

	void UnBind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType);


	/// <summary>
	/// ���³�������
	/// </summary>
	/// <param name="dxDeviceContext"></param>
	void UpdateConstantBuffer(ID3D11DeviceContext* dxDeviceContext);
	
	ID3D11Buffer* Get() const;
	ID3D11Buffer* const* GetAddressOf() const;

	/// <summary>
	/// ������ɫ�����������ض�Ӧ����
	/// </summary>
	/// <param name="variableName"></param>
	/// <returns></returns>
	ConstantBufferVariable GetConstantBufferVariableByName(std::string variableName);

public:
	/// <summary>
	/// ����ָ��λ�á�ָ����С�Ļ�������ֵ
	/// </summary>
	/// <param name="data">Դ����</param>
	/// <param name="byteOffset">��λ��ƫ��ֵ</param>
	/// <param name="byteCount">�����ֽڴ�С</param>
	void SetData(const void* data, UINT byteOffset = 0, UINT byteCount = 0xFFFFFFFF);

	/// <summary>
	/// ��ȡָ��λ�á�ָ����С�Ļ�������ֵ
	/// </summary>
	/// <param name="pOutput">��ȡ������</param>
	/// <param name="byteOffset">��λ��ƫ��ֵ</param>
	/// <param name="byteCount">�����ֽڴ�С</param>
	/// <returns></returns>
	HRESULT GetData(void* pOutput, UINT byteOffset = 0, UINT byteCount = 0xFFFFFFFF);

	/// <summary>
	/// ����int��������
	/// </summary>
	/// <param name="numComponents">���ݷ�����</param>
	/// <param name="byteOffset">��λ��ƫ��ֵ</param>
	/// <param name="byteWidth">�����ֽڴ�С</param>
	/// <param name="data">int�����б�</param>
	void SetIntVec(const int data[4], UINT numComponents, UINT byteOffset, UINT byteWidth);

	/// <summary>
	/// ����float��������
	/// </summary>
	/// <param name="numComponents">���ݷ�����</param>
	/// <param name="byteOffset">��λ��ƫ��ֵ</param>
	/// <param name="byteWidth">�����ֽڴ�С</param>
	/// <param name="data">float�����б�</param>
	void SetFloatVec(const FLOAT data[4], UINT numComponents, UINT byteOffset, UINT byteWidth);

	/// <summary>
	/// ����bool��������
	/// </summary>
	/// <param name="data">boolֵ</param>
	/// <param name="byteOffset">��λ��ƫ��ֵ</param>
	void SetBool(bool data, UINT byteOffset);

	/// <summary>
	/// ���ýṹ������
	/// </summary>
	/// <param name="data">�ṹ������ָ��</param>
	/// <param name="byteWidth">�ṹ�����ݴ�С</param>
	void SetStructure(void* data,UINT byteWidth);

public:
	std::map<std::string, ConstantBufferVariable> constantVariablesMap;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;//DirectX�����������
	std::unique_ptr<BYTE[]> m_data;//������������
	bool m_dirty;//�������������Ƿ��޸���δ�ύ����
	UINT m_slot;//���������ڼĴ����Ĳ�λ
	UINT m_byteWidth;//���������ֽڴ�С


};