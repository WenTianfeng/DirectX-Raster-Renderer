#pragma once

#include <wrl/client.h>
#include <d3d11_1.h>

#include"..\..\Tools\ErrorLogger.h"


template<class T>
class VertexBuffer 
{
public:
	VertexBuffer() {}

	/// <summary>
	/// 顶点缓冲实例化
	/// </summary>
	/// <param name="device">DirectX设备</param>
	/// <param name="data">顶点列表</param>
	/// <param name="numVertices">顶点列表顶点数</param>
	/// <returns>顶点缓冲创建是否成功</returns>
	bool Instantiate(ID3D11Device* dxDevice, T* data, UINT numVertices)
	{
		if (this->m_vertexBuffer != nullptr)
		{
			this->m_vertexBuffer.Reset();
		}

		this->m_vertexCount = numVertices;
		this->m_vertexStride = sizeof(T);

		//创建顶点缓冲
		D3D11_BUFFER_DESC vbd = {};
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = (m_vertexStride * m_vertexCount);
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA srd = {};
		srd.pSysMem = data;
		srd.SysMemPitch = 0;
		srd.SysMemSlicePitch = 0;

		HRESULT hr = dxDevice->CreateBuffer(&vbd, &srd, m_vertexBuffer.GetAddressOf());

		//错误检查
		COM_ERROR_IF_FAILED(hr, "Failed to create Vertex Buffer.");

		return true;

	}

	/// <summary>
	/// 返回顶点缓冲对象的指针
	/// </summary>
	/// <returns></returns>
	ID3D11Buffer* Get() const 
	{
		return m_vertexBuffer.Get();
	}

	/// <summary>
	/// 返回顶点缓冲对象的指针的指针
	/// </summary>
	/// <returns></returns>
	ID3D11Buffer*const* GetAddressOf() const
	{
		return m_vertexBuffer.GetAddressOf();
	}

	/// <summary>
	/// 获取顶点缓冲数据量
	/// </summary>
	/// <returns>数据量值</returns>
	UINT VertexCount()const 
	{
		return this->m_vertexCount;
	}

	/// <summary>
	/// 获取顶点类型宽度（步长）
	/// </summary>
	/// <returns>步长值</returns>
	const UINT Stride()const
	{
		return this->m_vertexStride;
	}

	/// <summary>
	/// 获取顶点类型宽度（步长）对象指针
	/// </summary>
	/// <returns>步长值对象指针</returns>
	const UINT* StridePtr()const
	{
		return &this->m_vertexStride;
	}



private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	UINT m_vertexStride = 0;
	UINT m_vertexCount = 0;
};
