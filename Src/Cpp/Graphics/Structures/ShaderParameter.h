#pragma once

#include<string>
#include"Shader.h"
#include"ConstantBuffer.h"

class ShaderParameter
{
/// <summary>
/// 着色器资源类型
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
	/// 绑定着色器资源至DirectX上下文
	/// </summary>
	/// <param name="deviceContext">DirectX上下文</param>
	void Bind(ID3D11DeviceContext* deviceContext);

	/// <summary>
	/// 更新着色器资源数据
	/// </summary>
	/// <param name="deviceContext">DirectX上下文</param>
	void UpdateParameterResource(ID3D11DeviceContext* deviceContext);

public:
	//Get方法
	std::string GetName() const;
	UINT GetSlot() const;
	ShaderParameterType GetShaderParameterType() const;

public:
	ConstantBuffer* constantBuffer;
	std::map<Shader::ShaderType, UINT> ownerShaderTypesMap = {
		{Shader::ShaderType::VertexShader, 0},
		{Shader::ShaderType::PixelShader, 0},
		{Shader::ShaderType::TessellationControlShader, 0},
		{Shader::ShaderType::TessellationEvaluationShader, 0},
		{Shader::ShaderType::GeometryShader, 0},
		{Shader::ShaderType::ComputeShader, 0}
	};

private:
	std::string m_shaderParameterName;
	UINT m_slot;
	ShaderParameterType m_shaderParameterType = ShaderParameterType::Invalid;


};