#include "Material.h"

Material::Material(ID3D11Device* device, std::wstring vertexShaderFilePath, std::wstring pixelShaderFilePath)
{

	HRESULT hr;

//=========================����������ɫ��==========================
	this->m_vertexShader.Instantiate(device, Shader::ShaderType::VertexShader,vertexShaderFilePath,"VS_Main","vs_5_0");

//==========================�������벼��===========================
#pragma region �������벼��
	//��ȡshader�������
	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> shaderReflection;
	hr = D3DReflect(m_vertexShader.GetBlob()->GetBufferPointer(), m_vertexShader.GetBlob()->GetBufferSize(), __uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(shaderReflection.GetAddressOf()));
	COM_ERROR_IF_FAILED(hr, "Failed to reflect vertex shader.");

	//��ȡ��ɫ��������Ϣ
	D3D11_SHADER_DESC shaderDescription;
	hr = shaderReflection->GetDesc(&shaderDescription);
	COM_ERROR_IF_FAILED(hr, "Failed to get description of vertex shader.");

	//��������ṹ��������
	UINT numInputParameters = shaderDescription.InputParameters;

	//���벼���б�
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;

	//������ɫ������Ķ�������ṹ�������벼��
	for (UINT i = 0; i < numInputParameters; ++i)
	{
		D3D11_INPUT_ELEMENT_DESC inputElement = {};//���벼���б�Ԫ��
		D3D11_SIGNATURE_PARAMETER_DESC parameterSignature;//��������ṹÿ��Ԫ�ص�ǩ��

		shaderReflection->GetInputParameterDesc(i, &parameterSignature);

		inputElement.SemanticName = parameterSignature.SemanticName;
		inputElement.SemanticIndex = parameterSignature.SemanticIndex;
		inputElement.InputSlot = 0;
		inputElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA; 
		inputElement.InstanceDataStepRate = 0;
		inputElement.Format = ShaderProcessor::GetDXGIFormat(parameterSignature);

		//�����ʽ��Ч����ֹͣ����
		assert(inputElement.Format != DXGI_FORMAT_UNKNOWN);

		//�����벼��Ԫ�ؼ����б�
		inputElements.push_back(inputElement);
	}

	if (inputElements.size() > 0)
	{
		//ʹ�����벼���б������벼��
		hr = device->CreateInputLayout(inputElements.data(), (UINT)inputElements.size(), m_vertexShader.GetBlob()->GetBufferPointer(), m_vertexShader.GetBlob()->GetBufferSize(), this->m_dxInputLayout.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX Input Layout.");
	}
#pragma endregion

//==========================����������ɫ��==========================

	this->m_pixelShader.Instantiate(device, Shader::ShaderType::PixelShader, pixelShaderFilePath, "PS_Main", "ps_5_0");

//========================�������-ģ�建��״̬=======================

	//���ģ�����״̬����
	D3D11_DEPTH_STENCIL_DESC dsc;
	ZeroMemory(&dsc, sizeof(dsc));
	dsc.DepthEnable = true;
	dsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsc.StencilEnable = false;
	//��������������������ģ�建����������д����������������

	hr = device->CreateDepthStencilState(&dsc, m_dxDepthStencilState.GetAddressOf());
	//������
	COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX DepthStencil State.");


//==========================�������״̬===========================
#pragma region �������״̬
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

#pragma endregion 

//==========================������������===========================

	this->m_transformMatrixCB.Instantiate(device);

}

Shader Material::GetVertexShader()
{
	return this->m_vertexShader;
}

Shader Material::GetPixelShader()
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



