#include "SamplerState.h"

SamplerState::SamplerState(UINT slot):
	m_slot(slot)
{
}

void SamplerState::Instantiate(ID3D11Device* dxDevice)
{
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));

	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr =  dxDevice->CreateSamplerState(&sampDesc, m_samplerState.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create DirectX sampler state.");
}

void SamplerState::Bind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType)
{

	switch (shaderType)
	{
	case Shader::ShaderType::UnknownShaderType:
		break;
	case Shader::ShaderType::VertexShader:
		deviceContext->VSSetSamplers(m_slot, 1, m_samplerState.GetAddressOf());
		break;
	case Shader::ShaderType::PixelShader:
		deviceContext->PSSetSamplers(m_slot, 1, m_samplerState.GetAddressOf());
		break;
	case Shader::ShaderType::TessellationControlShader:
		deviceContext->HSSetSamplers(m_slot, 1, m_samplerState.GetAddressOf());
		break;
	case Shader::ShaderType::TessellationEvaluationShader:
		deviceContext->DSSetSamplers(m_slot, 1, m_samplerState.GetAddressOf());
		break;
	case Shader::ShaderType::GeometryShader:
		deviceContext->GSSetSamplers(m_slot, 1, m_samplerState.GetAddressOf());
		break;
	case Shader::ShaderType::ComputeShader:
		deviceContext->CSSetSamplers(m_slot, 1, m_samplerState.GetAddressOf());
		break;
	default:
		break;
	}
}

void SamplerState::UnBind(ID3D11DeviceContext* deviceContext, Shader::ShaderType shaderType)
{
	ID3D11SamplerState* pSamplers[] = { nullptr };

	switch (shaderType)
	{
	case Shader::ShaderType::UnknownShaderType:
		break;
	case Shader::ShaderType::VertexShader:
		deviceContext->VSSetSamplers(m_slot, 1, pSamplers);
		break;
	case Shader::ShaderType::PixelShader:
		deviceContext->PSSetSamplers(m_slot, 1, pSamplers);
		break;
	case Shader::ShaderType::TessellationControlShader:
		deviceContext->HSSetSamplers(m_slot, 1, pSamplers);
		break;
	case Shader::ShaderType::TessellationEvaluationShader:
		deviceContext->DSSetSamplers(m_slot, 1, pSamplers);
		break;
	case Shader::ShaderType::GeometryShader:
		deviceContext->GSSetSamplers(m_slot, 1, pSamplers);
		break;
	case Shader::ShaderType::ComputeShader:
		deviceContext->CSSetSamplers(m_slot, 1, pSamplers);
		break;
	default:
		break;
	}

}
