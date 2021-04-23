#include "ShaderProcessor.h"

bool ShaderProcessor::ShaderReflectionProcess(ID3DBlob* blob)
{
	HRESULT hr;

	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> shaderReflection;

	hr = D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(shaderReflection.GetAddressOf()));

	D3D11_SIGNATURE_PARAMETER_DESC spd;

	shaderReflection->GetInputParameterDesc(0, &spd);
	D3D11_SHADER_DESC sd;

	shaderReflection->GetDesc(&sd);


	for (UINT i = 0;; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC sibDesc;
		hr = shaderReflection->GetResourceBindingDesc(i, &sibDesc);

		// 读取完变量后会失败，但这并不是失败的调用
		if (FAILED(hr))
			break;

		// 根据sibDesc继续分析...
		OutputDebugString(DataTypeConverter::StringToWideString(sibDesc.Name).c_str());
	}

	return true;
}

HRESULT ShaderProcessor::CreateShaderFromFile(CompileFormat compileFormat, const std::wstring hlslFormatFilePath,
	std::string shaderEntryPoint, std::string shaderModel, ID3DBlob** blob, const std::wstring csoFormatFilePath)

{

	HRESULT hr = S_OK;

	//如果存在.cso格式的着色器文件，则使用D3DReadFileToBlob方法直接生成blob对象
	if (compileFormat == CompileFormat::CSO && D3DReadFileToBlob(csoFormatFilePath.c_str(), blob) == S_OK)
	{
		return hr;
	}
	else
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
		// 设置 D3DCOMPILE_DEBUG 标志用于获取着色器调试信息。该标志可以提升调试体验，但仍然允许着色器进行优化操作
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// 在Debug环境下禁用优化以避免出现一些不合理的情况
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		hr = D3DCompileFromFile(hlslFormatFilePath.c_str(), nullptr, ((ID3DInclude*)(UINT_PTR)1),
			shaderEntryPoint.c_str(), shaderModel.c_str(), dwShaderFlags, 0, blob, nullptr);

		COM_ERROR_IF_FAILED(hr, "Failed to compile shader from file.");

	}

	if (!csoFormatFilePath.empty())
	{
		return D3DWriteBlobToFile(*blob, csoFormatFilePath.c_str(), FALSE);
	}

	return hr;
}

DXGI_FORMAT ShaderProcessor::GetDXGIFormat(const D3D11_SIGNATURE_PARAMETER_DESC& paramDesc)
{
    DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
    if (paramDesc.Mask == 1) // 1 component
    {
        switch (paramDesc.ComponentType)
        {
        case D3D_REGISTER_COMPONENT_UINT32:
        {
            format = DXGI_FORMAT_R32_UINT;
        }
        break;
        case D3D_REGISTER_COMPONENT_SINT32:
        {
            format = DXGI_FORMAT_R32_SINT;
        }
        break;
        case D3D_REGISTER_COMPONENT_FLOAT32:
        {
            format = DXGI_FORMAT_R32_FLOAT;
        }
        break;
        }
    }
    else if (paramDesc.Mask <= 3) // 2 components
    {
        switch (paramDesc.ComponentType)
        {
        case D3D_REGISTER_COMPONENT_UINT32:
        {
            format = DXGI_FORMAT_R32G32_UINT;
        }
        break;
        case D3D_REGISTER_COMPONENT_SINT32:
        {
            format = DXGI_FORMAT_R32G32_SINT;
        }
        break;
        case D3D_REGISTER_COMPONENT_FLOAT32:
        {
            format = DXGI_FORMAT_R32G32_FLOAT;
        }
        break;
        }
    }
    else if (paramDesc.Mask <= 7) // 3 components
    {
        switch (paramDesc.ComponentType)
        {
        case D3D_REGISTER_COMPONENT_UINT32:
        {
            format = DXGI_FORMAT_R32G32B32_UINT;
        }
        break;
        case D3D_REGISTER_COMPONENT_SINT32:
        {
            format = DXGI_FORMAT_R32G32B32_SINT;
        }
        break;
        case D3D_REGISTER_COMPONENT_FLOAT32:
        {
            format = DXGI_FORMAT_R32G32B32_FLOAT;
        }
        break;
        }
    }
    else if (paramDesc.Mask <= 15) // 4 components
    {
        switch (paramDesc.ComponentType)
        {
        case D3D_REGISTER_COMPONENT_UINT32:
        {
            format = DXGI_FORMAT_R32G32B32A32_UINT;
        }
        break;
        case D3D_REGISTER_COMPONENT_SINT32:
        {
            format = DXGI_FORMAT_R32G32B32A32_SINT;
        }
        break;
        case D3D_REGISTER_COMPONENT_FLOAT32:
        {
            format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
        break;
        }
    }

    return format;
}

