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
	/// 构造函数
	/// </summary>
	/// <param name="slot">常量缓冲所在的寄存器槽位</param>
	/// <param name="byteWidth">常量缓冲字节大小</param>
	ConstantBuffer(UINT slot, UINT byteWidth);

	/// <summary>
	/// 实例化常量缓冲
	/// </summary>
	/// <param name="dxDevice">DirectX设备</param>
	void Instantiate(ID3D11Device* dxDevice);

	/// <summary>
	/// 将常量缓冲绑定至DirectX上下文
	/// </summary>
	/// <param name="deviceContext">DirectX上下文</param>
	/// <param name="bufferSlot">寄存器槽位</param>
	/// <param name="shaderType">着色器类型</param>
	void Bind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType);

	void UnBind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType);


	/// <summary>
	/// 更新常量缓冲
	/// </summary>
	/// <param name="dxDeviceContext"></param>
	void UpdateConstantBuffer(ID3D11DeviceContext* dxDeviceContext);
	
	ID3D11Buffer* Get() const;
	ID3D11Buffer* const* GetAddressOf() const;

	/// <summary>
	/// 根据着色器变量名返回对应变量
	/// </summary>
	/// <param name="variableName"></param>
	/// <returns></returns>
	ConstantBufferVariable GetConstantBufferVariableByName(std::string variableName);

public:
	/// <summary>
	/// 设置指定位置、指定大小的缓冲数据值
	/// </summary>
	/// <param name="data">源数据</param>
	/// <param name="byteOffset">首位置偏移值</param>
	/// <param name="byteCount">数据字节大小</param>
	void SetData(const void* data, UINT byteOffset = 0, UINT byteCount = 0xFFFFFFFF);

	/// <summary>
	/// 获取指定位置、指定大小的缓冲数据值
	/// </summary>
	/// <param name="pOutput">获取的数据</param>
	/// <param name="byteOffset">首位置偏移值</param>
	/// <param name="byteCount">数据字节大小</param>
	/// <returns></returns>
	HRESULT GetData(void* pOutput, UINT byteOffset = 0, UINT byteCount = 0xFFFFFFFF);

	/// <summary>
	/// 设置int类型数据
	/// </summary>
	/// <param name="numComponents">数据分量数</param>
	/// <param name="byteOffset">首位置偏移值</param>
	/// <param name="byteWidth">数据字节大小</param>
	/// <param name="data">int数据列表</param>
	void SetIntVec(const int data[4], UINT numComponents, UINT byteOffset, UINT byteWidth);

	/// <summary>
	/// 设置float类型数据
	/// </summary>
	/// <param name="numComponents">数据分量数</param>
	/// <param name="byteOffset">首位置偏移值</param>
	/// <param name="byteWidth">数据字节大小</param>
	/// <param name="data">float数据列表</param>
	void SetFloatVec(const FLOAT data[4], UINT numComponents, UINT byteOffset, UINT byteWidth);

	/// <summary>
	/// 设置bool类型数据
	/// </summary>
	/// <param name="data">bool值</param>
	/// <param name="byteOffset">首位置偏移值</param>
	void SetBool(bool data, UINT byteOffset);

	/// <summary>
	/// 设置结构体数据
	/// </summary>
	/// <param name="data">结构体数据指针</param>
	/// <param name="byteWidth">结构体数据大小</param>
	void SetStructure(void* data,UINT byteWidth);

public:
	std::map<std::string, ConstantBufferVariable> constantVariablesMap;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;//DirectX常量缓冲对象
	std::unique_ptr<BYTE[]> m_data;//常量缓冲数据
	bool m_dirty;//常量缓冲数据是否被修改且未提交更新
	UINT m_slot;//常量缓冲在寄存器的槽位
	UINT m_byteWidth;//常量缓冲字节大小


};