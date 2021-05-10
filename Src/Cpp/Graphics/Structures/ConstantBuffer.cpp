#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer(UINT slot, UINT byteWidth):
	m_slot(slot),
	m_byteWidth(byteWidth),
	m_dirty(false),
	m_data(new BYTE[byteWidth]{})
{

}

void ConstantBuffer::Instantiate(ID3D11Device* dxDevice)
{

	//创建缓冲描述
	D3D11_BUFFER_DESC cbd = {};
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.MiscFlags = 0u;
	cbd.StructureByteStride = 0u;
	cbd.ByteWidth = static_cast<UINT>(m_byteWidth + (16 - (m_byteWidth % 16)));

	HRESULT hr = dxDevice->CreateBuffer(&cbd, nullptr, this->m_constantBuffer.GetAddressOf());

	//错误检查
	COM_ERROR_IF_FAILED(hr, "Failed to create DirectX Constant Buffer.");
}

void ConstantBuffer::Bind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType)
{
	switch (shaderType)
	{
	case Shader::ShaderType::UnknownShaderType:
		break;
	case Shader::ShaderType::VertexShader:
		deviceContext->VSSetConstantBuffers(m_slot, 1, m_constantBuffer.GetAddressOf());
		break;
	case Shader::ShaderType::PixelShader:
		deviceContext->PSSetConstantBuffers(m_slot, 1, m_constantBuffer.GetAddressOf());
		break;
	case Shader::ShaderType::TessellationControlShader:
		deviceContext->HSSetConstantBuffers(m_slot, 1, m_constantBuffer.GetAddressOf());
		break;
	case Shader::ShaderType::TessellationEvaluationShader:
		deviceContext->DSSetConstantBuffers(m_slot, 1, m_constantBuffer.GetAddressOf());
		break;
	case Shader::ShaderType::GeometryShader:
		deviceContext->GSSetConstantBuffers(m_slot, 1, m_constantBuffer.GetAddressOf());
		break;
	case Shader::ShaderType::ComputeShader:
		deviceContext->CSSetConstantBuffers(m_slot, 1, m_constantBuffer.GetAddressOf());
		break;
	default:
		break;
	}
}

void ConstantBuffer::UnBind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType)
{
	ID3D11Buffer* pBuffers[] = { nullptr };

	switch (shaderType)
	{
	case Shader::ShaderType::UnknownShaderType:
		break;
	case Shader::ShaderType::VertexShader:
		deviceContext->VSSetConstantBuffers(m_slot, 1, pBuffers);
		break;
	case Shader::ShaderType::PixelShader:
		deviceContext->PSSetConstantBuffers(m_slot, 1, pBuffers);
		break;
	case Shader::ShaderType::TessellationControlShader:
		deviceContext->HSSetConstantBuffers(m_slot, 1, pBuffers);
		break;
	case Shader::ShaderType::TessellationEvaluationShader:
		deviceContext->DSSetConstantBuffers(m_slot, 1, pBuffers);
		break;
	case Shader::ShaderType::GeometryShader:
		deviceContext->GSSetConstantBuffers(m_slot, 1, pBuffers);
		break;
	case Shader::ShaderType::ComputeShader:
		deviceContext->CSSetConstantBuffers(m_slot, 1, pBuffers);
		break;
	default:
		break;
	}

}

void ConstantBuffer::UpdateConstantBuffer(ID3D11DeviceContext* dxDeviceContext)
{
	if (m_dirty)
	{
		m_dirty = false;

		// 更新常量缓冲区
		D3D11_MAPPED_SUBRESOURCE mapped_data;
		HRESULT hr = dxDeviceContext->Map(this->m_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_data);

		//错误检查
		COM_ERROR_IF_FAILED(hr, "Failed to map DirectX Constant Buffer.");

		memcpy_s(mapped_data.pData, m_byteWidth, m_data.get(), m_byteWidth);
		dxDeviceContext->Unmap(this->m_constantBuffer.Get(), 0);

	}

}

ConstantBufferVariable ConstantBuffer::GetConstantBufferVariableByName(std::string variableName)
{
	std::map<std::string, ConstantBufferVariable>::const_iterator iter = constantVariablesMap.find(variableName);
	return (iter->second);
}

ID3D11Buffer* ConstantBuffer::Get() const
{
	return m_constantBuffer.Get();
}

ID3D11Buffer* const* ConstantBuffer::GetAddressOf() const
{
	return m_constantBuffer.GetAddressOf();
}

void ConstantBuffer::SetData(const void* data, UINT byteOffset, UINT byteCount)
{
	if (byteOffset > m_byteWidth)
		return;
	if (byteOffset + byteCount > m_byteWidth)
		byteCount = m_byteWidth - byteOffset;

	memcpy_s(m_data.get() + byteOffset, byteCount, data, byteCount);
	m_dirty = true;
}

HRESULT ConstantBuffer::GetData(void* pOutput, UINT byteOffset, UINT byteCount)
{
	if (byteOffset > m_byteWidth || byteCount > m_byteWidth - byteOffset)
		return E_BOUNDS;
	if (!pOutput)
		return E_INVALIDARG;
	memcpy_s(pOutput, byteCount, m_data.get() + byteOffset, byteCount);
	return S_OK;
}

void ConstantBuffer::SetFloatVec(const FLOAT data[4], UINT numComponents, UINT byteOffset, UINT byteWidth)
{
	if (numComponents > 4)
		numComponents = 4;

	UINT byteCount = numComponents * sizeof(FLOAT);
	if (byteCount > byteWidth)
		byteCount = byteWidth;

	SetData(data, byteOffset, byteCount);
}

void ConstantBuffer::SetBool(bool data, UINT byteOffset)
{
	SetData(&data, byteOffset, sizeof(bool));
}

void ConstantBuffer::SetStructure(void* data, UINT byteWidth)
{
	if (byteWidth > m_byteWidth)
		return;
	memcpy_s(m_data.get(), byteWidth, data, byteWidth);
	m_dirty = true;
}

void ConstantBuffer::SetIntVec(const int data[4], UINT numComponents, UINT byteOffset, UINT byteWidth)
{
	if (numComponents > 4)
		numComponents = 4;

	UINT byteCount = numComponents * sizeof(int);
	if (byteCount > byteWidth)
		byteCount = byteWidth;

	SetData(data, byteOffset, byteCount);
}


