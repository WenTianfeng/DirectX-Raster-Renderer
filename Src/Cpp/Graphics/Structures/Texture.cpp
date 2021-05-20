#include "Texture.h"

Texture::Texture(ID3D11Device* dxDevice, UINT slot):
	m_dxDevice(dxDevice),
	m_slot(slot),
	m_textureFilePath("None")
{
}

void Texture::Instantiate(std::string textureFilePath)
{
	if (textureFilePath.empty())
	{
		return;
	}
	else if (textureFilePath == "None")
	{
		//清空资源
		m_shaderResourceView.Reset();
		this->m_textureFilePath = textureFilePath;
	}
	else
	{
		this->m_textureFilePath = textureFilePath;

		HRESULT hr;
		hr = DirectX::CreateWICTextureFromFile(m_dxDevice, DataTypeConverter::StringToWideString(m_textureFilePath).c_str(), nullptr, m_shaderResourceView.GetAddressOf());
		//从文件路径加载文件，填充 ID3D11Texture2D
		//hr = DirectX::CreateWICTextureFromFileEx(m_dxDevice, DataTypeConverter::StringToWideString(m_textureFileName).c_str(),
		//	0, D3D11_USAGE_STAGING,
		//	0, D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
		//	0, DirectX::WIC_LOADER_FLAGS::WIC_LOADER_DEFAULT,
		//	reinterpret_cast<ID3D11Resource**>(m_texture2D.ReleaseAndGetAddressOf()), m_shaderResourceView.ReleaseAndGetAddressOf());

	}


	
}

void Texture::Bind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType)
{
	if (!m_shaderResourceView)
	{
		return;
	}

	switch (shaderType)
	{
	case Shader::ShaderType::UnknownShaderType:
		break;
	case Shader::ShaderType::VertexShader:
		deviceContext->VSSetShaderResources(m_slot, 1, m_shaderResourceView.GetAddressOf());
		break;
	case Shader::ShaderType::PixelShader:
		deviceContext->PSSetShaderResources(m_slot, 1, m_shaderResourceView.GetAddressOf());
		break;
	case Shader::ShaderType::TessellationControlShader:
		deviceContext->HSSetShaderResources(m_slot, 1, m_shaderResourceView.GetAddressOf());
		break;
	case Shader::ShaderType::TessellationEvaluationShader:
		deviceContext->DSSetShaderResources(m_slot, 1, m_shaderResourceView.GetAddressOf());
		break;
	case Shader::ShaderType::GeometryShader:
		deviceContext->GSSetShaderResources(m_slot, 1, m_shaderResourceView.GetAddressOf());
		break;
	case Shader::ShaderType::ComputeShader:
		deviceContext->CSSetShaderResources(m_slot, 1, m_shaderResourceView.GetAddressOf());
		break;
	default:
		break;
	}

}

void Texture::UnBind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType)
{
	ID3D11ShaderResourceView* emptySRV[] = { nullptr };

	switch (shaderType)
	{
	case Shader::ShaderType::UnknownShaderType:
		break;
	case Shader::ShaderType::VertexShader:
		deviceContext->VSSetShaderResources(m_slot, 1, emptySRV);
		break;
	case Shader::ShaderType::PixelShader:
		deviceContext->PSSetShaderResources(m_slot, 1, emptySRV);
		break;
	case Shader::ShaderType::TessellationControlShader:
		deviceContext->HSSetShaderResources(m_slot, 1, emptySRV);
		break;
	case Shader::ShaderType::TessellationEvaluationShader:
		deviceContext->DSSetShaderResources(m_slot, 1, emptySRV);
		break;
	case Shader::ShaderType::GeometryShader:
		deviceContext->GSSetShaderResources(m_slot, 1, emptySRV);
		break;
	case Shader::ShaderType::ComputeShader:
		deviceContext->CSSetShaderResources(m_slot, 1, emptySRV);
		break;
	default:
		break;
	}
}

std::string Texture::GetTextureFilePath()
{
	return this->m_textureFilePath;
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView()
{
	return this->m_shaderResourceView.Get();
}
