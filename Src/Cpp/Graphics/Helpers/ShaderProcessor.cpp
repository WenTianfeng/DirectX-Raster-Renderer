#include "ShaderProcessor.h"

HRESULT ShaderProcessor::CreateShaderFromFile(CompileFormat compileFormat, const std::wstring hlslFormatFilePath,
	std::string shaderEntryPoint, std::string shaderModel, ID3DBlob** blob, const std::wstring csoFormatFilePath)

{

	HRESULT hr = S_OK;

	//�������.cso��ʽ����ɫ���ļ�����ʹ��D3DReadFileToBlob����ֱ������blob����
	if (compileFormat == CompileFormat::CSO && D3DReadFileToBlob(csoFormatFilePath.c_str(), blob) == S_OK)
	{
		return hr;
	}
	else
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
		// ���� D3DCOMPILE_DEBUG ��־���ڻ�ȡ��ɫ��������Ϣ���ñ�־���������������飬����Ȼ������ɫ�������Ż�����
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// ��Debug�����½����Ż��Ա������һЩ����������
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		hr = D3DCompileFromFile(hlslFormatFilePath.c_str(), nullptr, ((ID3DInclude*)(UINT_PTR)1),
			shaderEntryPoint.c_str(), shaderModel.c_str(), dwShaderFlags, 0, blob, nullptr);

		COM_ERROR_IF_FAILED(hr, L"Failed to compile shader from file : "+ hlslFormatFilePath);

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

