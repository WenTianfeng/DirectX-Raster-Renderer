#pragma once

#include<string>//wstring
#include <d3d11_1.h>//DirectX 11.1相关引用（https://docs.microsoft.com/en-us/windows/win32/api/d3d11_1/）
#include <wrl/client.h>//ComPtr智能指针相关引用 （https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160）

#include"..\Helpers\ShaderProcessor.h"

class Shader
{
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

	bool Instantiate(ID3D11Device* dxDevice, ShaderType shaderType, std::wstring shaderFilePath, std::string entryPoint, std::string shaderModel);

	void Bind(ID3D11DeviceContext* deviceContext);

	ID3DBlob* GetBlob();

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11HullShader> m_hullShader;
	Microsoft::WRL::ComPtr<ID3D11DomainShader> m_domainShader;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_computeShader;

	ShaderType m_shaderType;
	std::string m_entryPoint;
	std::string m_shaderModel;
	std::wstring m_shaderFilePath;
	Microsoft::WRL::ComPtr<ID3DBlob> m_blob;

};