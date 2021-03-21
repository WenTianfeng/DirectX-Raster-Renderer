#include "Shader.h"

//顶点着色器成员定义=======================================================================================

bool VertexShader::Initialize(ID3D11Device* dxDevice , std::wstring shaderFilePath , D3D11_INPUT_ELEMENT_DESC* inputLayoutDescs , UINT numElements)
{
	HRESULT hr;

	//读取编译好的着色器二进制信息
	hr = D3DReadFileToBlob(shaderFilePath.c_str(), this->m_blobBuffer.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, L"Failed to read Vertex Shader File to Blob : "+ shaderFilePath);

	//创建顶点着色器
	hr = dxDevice->CreateVertexShader(this->m_blobBuffer->GetBufferPointer(), this->m_blobBuffer->GetBufferSize(), nullptr, this->m_vertexShader.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, L"Failed to load Vertex Shader :" + shaderFilePath);


	//创建输入布局
	hr = dxDevice->CreateInputLayout(inputLayoutDescs, numElements, this->m_blobBuffer->GetBufferPointer(), this->m_blobBuffer->GetBufferSize(), this->m_dxInputLayout.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX Input Layout.");


	return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return this->m_vertexShader.Get();
}

ID3DBlob* VertexShader::GetBlob()
{
	return this->m_blobBuffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return this->m_dxInputLayout.Get();
}


//像素着色器成员定义==========================================================================================

bool PixelShader::Initialize(ID3D11Device* dxDevice, std::wstring shaderFilePath)
{
	HRESULT hr;

	//读取编译好的着色器二进制信息
	hr = D3DReadFileToBlob(shaderFilePath.c_str(), this->m_blobBuffer.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, L"Failed to read Pixel Shader File to Blob : " + shaderFilePath);

	//创建像素着色器
	hr = dxDevice->CreatePixelShader(this->m_blobBuffer->GetBufferPointer(), this->m_blobBuffer->GetBufferSize(), nullptr, this->m_pixelShader.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, L"Failed to load Pixel Shader : " + shaderFilePath);

	return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return this->m_pixelShader.Get();
}

ID3DBlob* PixelShader::GetBlob()
{
	return this->m_blobBuffer.Get();
}
