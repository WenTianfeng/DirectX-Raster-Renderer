#include "Shader.h"

Shader::Shader(ShaderType shaderType, std::string shaderFilePath, std::string entryPoint, std::string shaderModel):
    m_shaderType(shaderType),
    m_shaderFilePath(shaderFilePath),
    m_entryPoint(entryPoint),
    m_shaderModel(shaderModel)
{
}

bool Shader::Instantiate(ID3D11Device* dxDevice)
{
    HRESULT hr;

    hr = ShaderProcessor::CreateShaderFromFile(ShaderProcessor::CompileFormat::HLSL, DataTypeConverter::StringToWideString(m_shaderFilePath), m_entryPoint, m_shaderModel, m_blob.ReleaseAndGetAddressOf());
    //´íÎó¼ì²é
    COM_ERROR_IF_FAILED(hr, "Failed to create shader blob from file : " + m_shaderFilePath);

    switch (m_shaderType)
    {
    case ShaderType::VertexShader:
        hr = dxDevice->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_vertexShader);
        break;
    case ShaderType::TessellationControlShader:
        hr = dxDevice->CreateHullShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_hullShader);
        break;
    case ShaderType::TessellationEvaluationShader:
        hr = dxDevice->CreateDomainShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_domainShader);
        break;
    case ShaderType::GeometryShader:
        hr = dxDevice->CreateGeometryShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_geometryShader);
        break;
    case ShaderType::PixelShader:
        hr = dxDevice->CreatePixelShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_pixelShader);
        break;
    case ShaderType::ComputeShader:
        hr = dxDevice->CreateComputeShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), nullptr, &m_computeShader);
        break;
    }

    //´íÎó¼ì²é
    COM_ERROR_IF_FAILED(hr, "Failed to create shader : " + m_shaderFilePath);

    return true;
}

void Shader::Bind(ID3D11DeviceContext* deviceContext)
{
    if (m_vertexShader)
    {
        deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
    }
    else if (m_hullShader)
    {
        deviceContext->HSSetShader(m_hullShader.Get(), nullptr, 0);
    }
    else if (m_domainShader)
    {
        deviceContext->DSSetShader(m_domainShader.Get(), nullptr, 0);
    }
    else if (m_geometryShader)
    {
        deviceContext->GSSetShader(m_geometryShader.Get(), nullptr, 0);
    }
    else if (m_pixelShader)
    {
        deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);
    }
    else if (m_computeShader)
    {
        deviceContext->CSSetShader(m_computeShader.Get(), nullptr, 0);
    }
}

ID3DBlob* Shader::GetBlob()
{
	return this->m_blob.Get();
}
