#pragma once

#include<vector>
#include<map>

#include"Shader.h"
#include"ShaderParameter.h"
#include"ConstantBufferTypes.h"



class Material 
{
public:
	static std::string CullModes[3];
	static std::string FillModes[2];

	static std::string DepthWriteMasks[2];
	static std::string DepthFuncs[8];

	static std::string BlendOps[5];

public:
	Material();

	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="device">DirectX设备</param>
	Material(ID3D11Device* device);

	/// <summary>
	/// 材质实例化
	/// </summary>
	/// <param name="device"></param>
	/// <param name="shaderFilePath">着色器文件路径（多种着色器整合文件）</param>
	/// <returns></returns>
	bool Instantiate(std::string shaderFilePath);

	/// <summary>
	/// 获取材质着色器文件路径
	/// </summary>
	/// <returns></returns>
	std::string GetShaderFilePath();

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
	/// 获取光栅化状态
	/// </summary>
	/// <returns></returns>
	ID3D11RasterizerState* GetRasterizerState() const;

	/// <summary>
	/// 绑定所有着色器至DirectX上下文
	/// </summary>
	/// <param name="deviceContext">DirectX上下文</param>
	void BindShaders(ID3D11DeviceContext* deviceContext);

	void SetupDepthStencilState();
	void ReSetupDepthStencilState();

	void SetupBlendState();
	void ReSetupBlendState();

	void SetupRasterizerState();
	void ReSetupRasterizerState();


public:
	std::map<std::string, ShaderParameter*> shaderParametersMap;	//<着色器参数名-参数>map
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_RENDER_TARGET_BLEND_DESC blendDesc;;
	D3D11_RASTERIZER_DESC rasterizerDesc;

private:
	ID3D11Device* m_dxDevice;//DirectX 设备
	std::string m_shaderFilePath;//着色器文件路径
	std::map<Shader::ShaderType, Shader*> m_shadersMap;//<着色器类型-着色器> map

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_dxInputLayout;//DirectX输入布局对象
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_dxDepthStencilState;//深度模板状态
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_dxBlendState;//混合状态
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_dxRasterizerState;//光栅化状态

};