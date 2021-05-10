#include "ShaderParameter.h"

ShaderParameter::ShaderParameter(ID3D11Device* device, std::string parameterName, ShaderParameterType parameterType, UINT slotID):
	m_shaderParameterName(parameterName),
	m_shaderParameterType(parameterType),
	m_slot(slotID),
	constantBuffer(nullptr),
	texture(nullptr),
	samplerState(nullptr)
{
	switch (parameterType)
	{
	case ShaderParameter::ShaderParameterType::Invalid:

		break;
	case ShaderParameter::ShaderParameterType::ConstantBuffer:
		break;
	case ShaderParameter::ShaderParameterType::Texture:

		texture = new Texture(device, slotID);

		break;
	case ShaderParameter::ShaderParameterType::Sampler:

		samplerState = new SamplerState(slotID);
		samplerState->Instantiate(device);

		break;
	case ShaderParameter::ShaderParameterType::StructuredBuffer:
		break;
	default:
		break;
	}

}

ShaderParameter::ShaderParameter(ID3D11Device* device, std::string parameterName, ShaderParameterType parameterType, UINT slotID, UINT byteWidth):
	m_shaderParameterName(parameterName),
	m_shaderParameterType(parameterType),
	m_slot(slotID),
	constantBuffer(nullptr),
	texture(nullptr),
	samplerState(nullptr)
{
	switch (parameterType)
	{
	case ShaderParameter::ShaderParameterType::Invalid:

		break;
	case ShaderParameter::ShaderParameterType::ConstantBuffer:

		constantBuffer = new ConstantBuffer(slotID, byteWidth);
		constantBuffer->Instantiate(device);

		break;
	case ShaderParameter::ShaderParameterType::Texture:
		break;
	case ShaderParameter::ShaderParameterType::Sampler:
		break;
	case ShaderParameter::ShaderParameterType::StructuredBuffer:
		break;
	default:
		break;
	}
}

void ShaderParameter::Bind(ID3D11DeviceContext* deviceContext)
{
	//遍历ShaderParameter属于的每一种着色器类型
	for (std::map<Shader::ShaderType, bool>::value_type pair_shaderType_used : ownerShaderTypesMap)
	{
		//如果ShaderParameter不属于此类型着色器，则跳过绑定
		if (pair_shaderType_used.second == false)
		{
			continue;
		}

		switch (m_shaderParameterType)
		{
			case ShaderParameterType::ConstantBuffer:

				this->constantBuffer->Bind(deviceContext, pair_shaderType_used.first);

				break;

			case ShaderParameterType::Texture:

				this->texture->Bind(deviceContext, pair_shaderType_used.first);

				break;

			case ShaderParameterType::Sampler:

				this->samplerState->Bind(deviceContext, pair_shaderType_used.first);

				break;

			case ShaderParameterType::StructuredBuffer:

				break;

			case ShaderParameterType::Invalid:

				break;
		}
	}
}

void ShaderParameter::UnBind(ID3D11DeviceContext* deviceContext)
{
	//遍历ShaderParameter属于的每一种着色器类型
	for (std::map<Shader::ShaderType, bool>::value_type pair_shaderType_used : ownerShaderTypesMap)
	{
		//如果ShaderParameter不属于此类型着色器，则跳过绑定
		if (pair_shaderType_used.second == false)
		{
			continue;
		}

		switch (m_shaderParameterType)
		{
		case ShaderParameterType::ConstantBuffer:

			this->constantBuffer->UnBind(deviceContext, pair_shaderType_used.first);

			break;

		case ShaderParameterType::Texture:

			this->texture->UnBind(deviceContext, pair_shaderType_used.first);

			break;

		case ShaderParameterType::Sampler:

			this->samplerState->UnBind(deviceContext, pair_shaderType_used.first);

			break;

		case ShaderParameterType::StructuredBuffer:

			break;

		case ShaderParameterType::Invalid:

			break;
		}
	}

}

void ShaderParameter::UpdateParameterResource(ID3D11DeviceContext* deviceContext)
{
	switch (m_shaderParameterType)
	{
		case ShaderParameterType::ConstantBuffer:

			this->constantBuffer->UpdateConstantBuffer(deviceContext);

			break;

		case ShaderParameterType::Texture:

			break;

		case ShaderParameterType::Sampler:

			break;

		case ShaderParameterType::StructuredBuffer:

			break;

		case ShaderParameterType::Invalid:

			break;
	}
}

std::string ShaderParameter::GetName() const
{
	return this->m_shaderParameterName;
}

UINT ShaderParameter::GetSlot() const
{
	return this->m_slot;
}

ShaderParameter::ShaderParameterType ShaderParameter::GetShaderParameterType() const
{
	return this->m_shaderParameterType;
}