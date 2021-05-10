#pragma once

#include<string>
#include"Shader.h"
#include"ConstantBuffer.h"
#include"Texture.h"
#include"SamplerState.h"

class ShaderParameter
{
/// <summary>
/// ��ɫ����Դ����
/// </summary>
public:
	enum class ShaderParameterType
	{
		Invalid = 0,    // Invalid parameter. Doesn't store a type.
		ConstantBuffer = 1,     // Buffers, ConstantBuffers, StructuredBuffers.
		Texture = 2,    // Texture.
		Sampler = 3,    // Texture sampler.
		StructuredBuffer = 4
	};

public:
	ShaderParameter(ID3D11Device* device, std::string parameterName, ShaderParameterType parameterType, UINT slot);
	ShaderParameter(ID3D11Device* device, std::string parameterName, ShaderParameterType parameterType, UINT slot, UINT byteWidth);
	
	/// <summary>
	/// ����ɫ����Դ��DirectX������
	/// </summary>
	/// <param name="deviceContext">DirectX������</param>
	void Bind(ID3D11DeviceContext* deviceContext);

	//�����ɫ����Դ�İ�
	void UnBind(ID3D11DeviceContext* deviceContext);

	/// <summary>
	/// ������ɫ����Դ����
	/// </summary>
	/// <param name="deviceContext">DirectX������</param>
	void UpdateParameterResource(ID3D11DeviceContext* deviceContext);

public:
	//Get����
	std::string GetName() const;
	UINT GetSlot() const;
	ShaderParameterType GetShaderParameterType() const;

public:
	std::map<Shader::ShaderType, bool> ownerShaderTypesMap = {
		{Shader::ShaderType::VertexShader, false},
		{Shader::ShaderType::PixelShader, false},
		{Shader::ShaderType::TessellationControlShader, false},
		{Shader::ShaderType::TessellationEvaluationShader, false},
		{Shader::ShaderType::GeometryShader, false},
		{Shader::ShaderType::ComputeShader, false}
	};

public:
	ConstantBuffer* constantBuffer;
	Texture* texture;
	SamplerState* samplerState;

private:
	std::string m_shaderParameterName;
	UINT m_slot;
	ShaderParameterType m_shaderParameterType = ShaderParameterType::Invalid;

};