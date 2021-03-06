#pragma once

#pragma comment(lib, "D3DCompiler.lib")

#include <d3d11_1.h>
#include <wrl/client.h>
#include <d3d11shader.h>
#include <d3dcompiler.h>

#include"..\..\Tools\DataTypeConverter.h"
#include"..\..\Tools\ErrorLogger.h"


class ShaderProcessor
{
public:
	//着色器编译源文件类型
	enum class CompileFormat
	{
		CSO = 0,
		HLSL = 1
	};

public:
	static HRESULT CreateShaderFromFile(CompileFormat compileFormat, const std::wstring hlslFormatFilePath, 
		std::string shaderEntryPoint, std::string shaderModel, ID3DBlob** blob, const std::wstring csoFormatFilePath = L"");

	// Determine DXGI format. Inspired by: http://takinginitiative.net/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/
	static DXGI_FORMAT GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc);

private:


};

