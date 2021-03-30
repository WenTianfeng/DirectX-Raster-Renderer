#include "Material.h"

Material::Material(ID3D11Device* device, std::wstring vertexShaderFilePath, std::wstring pixelShaderFilePath)
{

	HRESULT hr;
//===========创建顶点着色器===========
	this->m_vertexShader.Initialize(device, vertexShaderFilePath);

//===========创建输入布局============
	D3D11_INPUT_ELEMENT_DESC ieds[] =
	{
		{"POSITION", 0 ,DXGI_FORMAT_R32G32B32_FLOAT, 0 , 0 , D3D11_INPUT_PER_VERTEX_DATA , 0 } ,
		{"TEXCOORD", 0 ,DXGI_FORMAT_R32G32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA ,0 } ,
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr= device->CreateInputLayout(ieds, ARRAYSIZE(ieds),
		this->m_vertexShader.GetBlob()->GetBufferPointer(), this->m_vertexShader.GetBlob()->GetBufferSize(), this->m_dxInputLayout.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX Input Layout.");

//===========创建像素着色器===========
	this->m_pixelShader.Initialize(device, pixelShaderFilePath);

//=========创建深度-模板缓冲状态========

	//深度模板测试状态描述
	D3D11_DEPTH_STENCIL_DESC dsc;
	ZeroMemory(&dsc, sizeof(dsc));
	dsc.DepthEnable = true;
	dsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsc.DepthFunc = D3D11_COMPARISON_LESS;
	dsc.StencilEnable = false;
	//……………………其余模板缓冲描述待填写……………………

	hr = device->CreateDepthStencilState(&dsc, m_dxDepthStencilState.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX DepthStencil State.");

//===========创建混合状态============
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = false;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD; //颜色混合运算方法
	rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA; //源颜色因子
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA; //缓冲区颜色因子
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE; //源透明度因子
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO; //缓冲区透明度因子
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD; //透明度混合运算方法
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; //颜色遮罩

	bd.RenderTarget[0] = rtbd;

	hr = device->CreateBlendState(&bd, this->m_dxBlendState.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX Blend State.");

//===========创建常量缓冲============

	this->m_transformMatrixCB.Initialize(device);

}

VertexShader Material::GetVertexShader()
{
	return this->m_vertexShader;
}

PixelShader Material::GetPixelShader()
{
	return this->m_pixelShader;
}

ID3D11InputLayout* Material::GetInputLayout()
{
	return this->m_dxInputLayout.Get();
}

ID3D11DepthStencilState* Material::GetDepthStencilState()
{
	return this->m_dxDepthStencilState.Get();
}

ID3D11BlendState* Material::GetBlendState()
{
	return this->m_dxBlendState.Get();
}

ConstantBuffer<CB_VS_TransformMatrix>& Material::GetConstantBuffer_TransformMatrix()
{
	return this->m_transformMatrixCB;
}
