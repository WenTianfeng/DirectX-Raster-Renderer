#include "ShaderParameter.h"

ShaderParameter::ShaderParameter(ID3D11Device* device, std::string parameterName, ShaderParameterType parameterType, UINT slotID):
	m_shaderParameterName(parameterName),
	m_shaderParameterType(parameterType),
	m_slot(slotID),
	constantBuffer(nullptr)
{
	switch (parameterType)
	{
	case ShaderParameter::ShaderParameterType::Invalid:

		break;
	case ShaderParameter::ShaderParameterType::ConstantBuffer:
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

ShaderParameter::ShaderParameter(ID3D11Device* device, std::string parameterName, ShaderParameterType parameterType, UINT slotID, UINT byteWidth):
	m_shaderParameterName(parameterName),
	m_shaderParameterType(parameterType),
	m_slot(slotID),
	constantBuffer(nullptr)
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
	for (std::map<Shader::ShaderType, UINT>::value_type value : ownerShaderTypesMap)
	{
		//���ShaderParameter�����ڴ�������ɫ������������
		if (value.second == 0)
		{
			continue;
		}

		switch (m_shaderParameterType)
		{
			case ShaderParameterType::ConstantBuffer:

				this->constantBuffer->Bind(deviceContext, value.first);

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