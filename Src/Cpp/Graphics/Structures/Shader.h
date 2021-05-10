#pragma once

#include<string>//wstring
#include <d3d11_1.h>//DirectX 11.1相关引用（https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/）
#include <wrl/client.h>//ComPtr智能指针相关引用 （https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160）

#include"..\Helpers\ShaderProcessor.h"	//CreateShaderFromFile 着色器编译相关方法

class Shader
{
/// <summary>
/// 着色器类型枚举
/// </summary>
public:
	enum class ShaderType
	{
		UnknownShaderType = 0,
		VertexShader = 1,
		PixelShader  = 2,
		TessellationControlShader = 3,      // Hull Shader in DirectX
		TessellationEvaluationShader = 4,   // Domain Shader in DirectX
		GeometryShader = 5,
		ComputeShader = 6
	};

public:
	/// <summary>
	/// 构造函数
	/// </summary>
	/// <param name="shaderType">着色器类型</param>
	/// <param name="shaderFilePath">着色器文件路径</param>
	/// <param name="entryPoint">着色器编译入口点</param>
	/// <param name="shaderModel">着色模型</param>
	Shader(ShaderType shaderType, std::string shaderFilePath, std::string entryPoint, std::string shaderModel);

	/// <summary>
	/// 着色器实例化
	/// </summary>
	/// <param name="dxDevice">DirectX设备</param>
	/// <returns>是否实例化成功</returns>
	bool Instantiate(ID3D11Device* dxDevice);

	/// <summary>
	/// 绑定着色器至DirectX上下文
	/// </summary>
	/// <param name="deviceContext">DirectX上下文</param>
	void Bind(ID3D11DeviceContext* deviceContext);

	/// <summary>
	/// 获取着色器编译后的二进制数据
	/// </summary>
	/// <returns>二进制数据指针</returns>
	ID3DBlob* GetBlob();

private:
	/// <summary>
	/// 预置所有类型着色器指针，根据类型选择性实例化
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11HullShader> m_hullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader> m_domainShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_computeShader;

	std::string m_shaderFilePath;//着色器文件路径
	ShaderType m_shaderType = ShaderType::UnknownShaderType;//着色器类型
	std::string m_entryPoint;//着色器编译入口函数
	std::string m_shaderModel;//着色模型版本号
	Microsoft::WRL::ComPtr<ID3DBlob> m_blob;//着色器编译后二进制数据对象

};