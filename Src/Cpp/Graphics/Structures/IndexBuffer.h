#pragma once

#include <wrl/client.h>
#include <d3d11_1.h>

#include"..\..\Tools\ErrorLogger.h"


template<class T>
class IndexBuffer 
{
public:
	IndexBuffer() {}

	/// <summary>
	/// 返回索引缓冲对象的指针
	/// </summary>
	/// <returns></returns>
	ID3D11Buffer* Get()const 
	{
		return m_indexBuffer.Get();
	}

	/// <summary>
	/// 返回索引缓冲对象的指针的指针
	/// </summary>
	/// <returns></returns>
	ID3D11Buffer* const* GetAddressOf() const
	{
		return m_indexBuffer.GetAddressOf();
	}

	/// <summary>
	/// 获取索引缓冲数据量
	/// </summary>
	/// <returns>数据量值</returns>
	UINT IndexCount()const 
	{
		return this->m_IndexCount;
	}

	/// <summary>
	/// 获取索引类型宽度（步长）
	/// </summary>
	/// <returns>步长值</returns>
	const UINT Stride()const
	{
		return this->m_indexStride;
	}

	/// <summary>
	/// 获取索引类型宽度（步长）对象指针
	/// </summary>
	/// <returns>步长值对象指针</returns>
	const UINT* StridePtr()const
	{
		return &this->m_indexStride;
	}

	/// <summary>
	/// 索引缓冲初始化
	/// </summary>
	/// <param name="device">DirectX设备</param>
	/// <param name="data">索引列表</param>
	/// <param name="numVertices">索引列表顶点数</param>
	/// <returns>索引缓冲创建是否成功</returns>
	bool Initialize(ID3D11Device* dxDevice,T* data,UINT numIndices)
	{
		if (this->m_indexBuffer != nullptr)
		{
			this->m_indexBuffer.Reset();
		}

		this->m_IndexCount = numIndices;
		this->m_indexStride = sizeof(T);

		//创建索引缓冲
		D3D11_BUFFER_DESC ibd = {};
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = (UINT)(sizeof(T) * this->m_IndexCount);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA srd = {};
		srd.pSysMem = data;

		HRESULT hr  = dxDevice->CreateBuffer(&ibd, &srd, m_indexBuffer.GetAddressOf());
		//错误检查
		COM_ERROR_IF_FAILED(hr, "Failed to create Index Buffer.");

		return true;
	
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
	UINT m_indexStride = 0;//索引步长
	UINT m_IndexCount = 0;//索引数量
};