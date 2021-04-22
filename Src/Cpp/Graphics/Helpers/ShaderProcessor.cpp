#include "ShaderProcessor.h"

bool ShaderProcessor::ShaderPreProcess(ID3DBlob* blob)
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

ID3D11InputLayout* ShaderProcessor::ObtainInputLayout()
{
	



	return nullptr;
}
