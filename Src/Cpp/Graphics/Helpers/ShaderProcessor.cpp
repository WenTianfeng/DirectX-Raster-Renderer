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
		// ��ȡ��������ʧ�ܣ����Ⲣ����ʧ�ܵĵ���
		if (FAILED(hr))
			break;

		// ����sibDesc��������...
		OutputDebugString(DataTypeConverter::StringToWideString(sibDesc.Name).c_str());
	}

	return true;
}

ID3D11InputLayout* ShaderProcessor::ObtainInputLayout()
{
	



	return nullptr;
}
