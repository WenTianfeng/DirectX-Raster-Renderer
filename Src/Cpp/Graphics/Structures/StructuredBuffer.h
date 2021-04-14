#pragma once

#include <wrl/client.h>
#include <d3d11_1.h>
#include<vector>
#include"..\..\Tools\ErrorLogger.h"

template<class T>
class StructuredBuffer
{
public:

	//缓冲区对应的结构体数据
	std::vector<T> bufferData;

	StructuredBuffer() :
		m_structuredBuffer(nullptr)
	{

	}

	ID3D11Buffer* Get() const
	{
		return m_structuredBuffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf() const
	{
		return m_structuredBuffer.GetAddressOf();
	}

	ID3D11ShaderResourceView* GetSRV()
	{
		return this->m_structuredBufferSRV.Get();
	}

	ID3D11ShaderResourceView* const* GetSRVAddressOf()
	{
		return this->m_structuredBufferSRV.GetAddressOf();
	}

	void SetData(T* data, UINT elementSize, UINT numElements)
	{
		T* first = data;
		T* last = first + elementSize * numElements;

		bufferData.assign(first, last);
	}

	/// <summary>
	/// 结构缓冲区初始化
	/// </summary>
	/// <param name="dxDevice">DirectX设备</param>
	/// <returns>初始化是否成功</returns>
	bool Initialize(ID3D11Device* dxDevice, UINT numElements)
	{
		//创建缓冲描述
		D3D11_BUFFER_DESC cbd = {};
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		cbd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		cbd.StructureByteStride = sizeof(T);
		cbd.ByteWidth = numElements * sizeof(T);

		HRESULT hr = dxDevice->CreateBuffer(&cbd, nullptr, this->m_structuredBuffer.GetAddressOf());

		//错误检查
		COM_ERROR_IF_FAILED(hr, "Failed to create DirectX Structured Buffer.");

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
		srvd.Format = DXGI_FORMAT_UNKNOWN;
		srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvd.Buffer.FirstElement = 0;			// 起始元素的索引
		srvd.Buffer.NumElements = numElements;	// 元素数目

		hr = dxDevice->CreateShaderResourceView(m_structuredBuffer.Get(), &srvd, m_structuredBufferSRV.GetAddressOf());

		//错误检查
		COM_ERROR_IF_FAILED(hr, "Failed to create DirectX Structured Buffer Shader Resource View.");

		return true;
	}

	/// <summary>
	/// 更新结构缓冲区数据
	/// </summary>
	/// <param name="dxDeviceContext">DirectX上下文</param>
	/// <returns>更新是否成功</returns>
	bool UpdateStructuredBuffer(ID3D11DeviceContext* dxDeviceContext)
	{
		// 更新常量缓冲区
		D3D11_MAPPED_SUBRESOURCE mapped_data;
		HRESULT hr = dxDeviceContext->Map(this->m_structuredBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_data);

		//错误检查
		COM_ERROR_IF_FAILED(hr, "Failed to map DirectX Constant Buffer.");

		size_t numElements = bufferData.size();
		memcpy_s(mapped_data.pData, numElements, bufferData.data(), numElements);
		dxDeviceContext->Unmap(this->m_structuredBuffer.Get(), 0);

		return true;
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_structuredBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_structuredBufferSRV;
};
