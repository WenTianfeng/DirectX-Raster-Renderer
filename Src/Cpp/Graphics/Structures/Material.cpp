#include "Material.h"

Material::Material(ID3D11Device* device, std::wstring vertexShaderFilePath, std::wstring pixelShaderFilePath)
{

	HRESULT hr;
//===========����������ɫ��===========
	this->m_vertexShader.Initialize(device, vertexShaderFilePath);

//===========�������벼��============
	D3D11_INPUT_ELEMENT_DESC ieds[] =
	{
		{"POSITION", 0 ,DXGI_FORMAT_R32G32B32_FLOAT, 0 , 0 , D3D11_INPUT_PER_VERTEX_DATA , 0 } ,
		{"TEXCOORD", 0 ,DXGI_FORMAT_R32G32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT , D3D11_INPUT_PER_VERTEX_DATA ,0 } ,
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr= device->CreateInputLayout(ieds, ARRAYSIZE(ieds),
		this->m_vertexShader.GetBlob()->GetBufferPointer(), this->m_vertexShader.GetBlob()->GetBufferSize(), this->m_dxInputLayout.GetAddressOf());
	//������
	COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX Input Layout.");

//===========����������ɫ��===========
	this->m_pixelShader.Initialize(device, pixelShaderFilePath);

//=========�������-ģ�建��״̬========

	//���ģ�����״̬����
	D3D11_DEPTH_STENCIL_DESC dsc;
	ZeroMemory(&dsc, sizeof(dsc));
	dsc.DepthEnable = true;
	dsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsc.DepthFunc = D3D11_COMPARISON_LESS;
	dsc.StencilEnable = false;
	//��������������������ģ�建����������д����������������

	hr = device->CreateDepthStencilState(&dsc, m_dxDepthStencilState.GetAddressOf());
	//������
	COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX DepthStencil State.");

//===========�������״̬============
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = false;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD; //��ɫ������㷽��
	rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA; //Դ��ɫ����
	rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA; //��������ɫ����
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE; //Դ͸��������
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO; //������͸��������
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD; //͸���Ȼ�����㷽��
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; //��ɫ����

	bd.RenderTarget[0] = rtbd;

	hr = device->CreateBlendState(&bd, this->m_dxBlendState.GetAddressOf());
	//������
	COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX Blend State.");

//===========������������============

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
