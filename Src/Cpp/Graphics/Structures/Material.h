#pragma once

#include<vector>
#include<map>

#include"Shader.h"
#include"ShaderParameter.h"
#include"ConstantBufferTypes.h"


class Material 
{
public:
	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="device">DirectX设备</param>
	/// <param name="shaderFilePath">着色器文件路径（多种着色器整合文件）</param>
	Material(ID3D11Device* device, std::wstring shaderFilePath);

	/// <summary>
	/// 获取指定类型着色器
	/// </summary>
	/// <param name="shaderType">着色器类型</param>
	/// <returns>着色器指针</returns>
	Shader* GetShader(Shader::ShaderType shaderType) const;

	/// <summary>
	/// 获取输入布局
	/// </summary>
	/// <returns>输入布局指针</returns>
	ID3D11InputLayout* GetInputLayout() const;

	/// <summary>
	/// 获取 深度-模板 状态
	/// </summary>
	/// <returns>深度-模板 状态指针</returns>
	ID3D11DepthStencilState* GetDepthStencilState() const;

	/// <summary>
	/// 获取混合状态
	/// </summary>
	/// <returns>混合状态指针</returns>
	ID3D11BlendState* GetBlendState() const;

	/// <summary>
	/// 绑定所有着色器至DirectX上下文
	/// </summary>
	/// <param name="deviceContext">DirectX上下文</param>
	void BindShaders(ID3D11DeviceContext* deviceContext);


public:
	//std::vector<ShaderParameter*> shaderParameters;	//材质包含的着色器资源参数列表
	std::map<std::string, ShaderParameter*> shaderParametersMap;	//材质包含的着色器资源参数map

private:
	std::wstring m_shaderFilePath;//着色器文件路径
	std::map<Shader::ShaderType, Shader*> m_shadersMap;//<着色器类型-着色器> map

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_dxInputLayout;//DirectX输入布局对象
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_dxDepthStencilState;//深度模板状态
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_dxBlendState;//混合状态


};