#pragma once

#include <wrl/client.h>
#include <d3d11_1.h>

#include"..\..\Tools\ErrorLogger.h"

template <class T>
class ConstantBuffer 
{
public:
	//��������Ӧ�Ľṹ������
	T bufferData;

	ConstantBuffer() {}

	ID3D11Buffer* Get()const
	{
		return m_constantBuffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf()const
	{
		return m_constantBuffer.GetAddressOf();
	}

	/// <summary>
	/// ������������ʼ��
	/// </summary>
	/// <param name="dxDevice">DirectX�豸</param>
	/// <returns>��ʼ���Ƿ�ɹ�</returns>
	bool Initialize(ID3D11Device* dxDevice)
	{
		//������������
		D3D11_BUFFER_DESC cbd = {};
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.MiscFlags = 0u;
		cbd.StructureByteStride = 0u;
		cbd.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));

		HRESULT hr = dxDevice->CreateBuffer(&cbd, nullptr, this->m_constantBuffer.GetAddressOf());

		//������
		COM_ERROR_IF_FAILED(hr, "Failed to create DirectX Constant Buffer.");


		return true;
	}

	/// <summary>
	/// ���³�������������
	/// </summary>
	/// <param name="dxDeviceContext">DirectX������</param>
	/// <returns>�����Ƿ�ɹ�</returns>
	bool UpdateConstantBuffer(ID3D11DeviceContext* dxDeviceContext)
	{
		// ���³���������
		D3D11_MAPPED_SUBRESOURCE mapped_data;
		HRESULT hr = dxDeviceContext->Map(this->m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_data);

		//������
		COM_ERROR_IF_FAILED(hr, "Failed to map DirectX Constant Buffer.");

		memcpy_s(mapped_data.pData, sizeof(T), &bufferData, sizeof(T));
		dxDeviceContext->Unmap(this->m_constantBuffer.Get(), 0);

		return true;
	}

private:
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

};
