#include "Material.h"

Material::Material(ID3D11Device* device):
	m_dxDevice(device)
{

}

bool Material::Instantiate(std::string shaderFilePath)
{
	//��ճ�Ա
	m_dxInputLayout.Reset();
	m_dxDepthStencilState.Reset();
	m_dxBlendState.Reset();
	m_shadersMap.clear();
	shaderParametersMap.clear();


	this->m_shaderFilePath = shaderFilePath;

	HRESULT hr;

	Shader* vertexShader = new Shader(Shader::ShaderType::VertexShader, shaderFilePath, "VS_Main", "vs_5_0");
	Shader* pixelShader = new Shader(Shader::ShaderType::PixelShader, shaderFilePath, "PS_Main", "ps_5_0");
	vertexShader->Instantiate(m_dxDevice);
	pixelShader->Instantiate(m_dxDevice);

	m_shadersMap.insert(std::map<Shader::ShaderType, Shader*>::value_type(Shader::ShaderType::VertexShader, vertexShader));
	m_shadersMap.insert(std::map<Shader::ShaderType, Shader*>::value_type(Shader::ShaderType::PixelShader, pixelShader));

	//======================������ɫ��map======================
	for (std::map<Shader::ShaderType, Shader*>::value_type value : m_shadersMap)
	{
		//��ɫ���������
		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> shaderReflection;
		hr = D3DReflect(value.second->GetBlob()->GetBufferPointer(), value.second->GetBlob()->GetBufferSize(),
			__uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(shaderReflection.GetAddressOf()));

		COM_ERROR_IF_FAILED(hr, "Failed to reflect shader.");

		//��ɫ������
		D3D11_SHADER_DESC shaderDescription;
		hr = shaderReflection->GetDesc(&shaderDescription);

		COM_ERROR_IF_FAILED(hr, "Failed to get description of shader.");

		//==========================�������벼��========================== =
		#pragma region ���������������ɫ���������ݷ������������벼��


		if (value.first == Shader::ShaderType::VertexShader)
		{
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
				hr = m_dxDevice->CreateInputLayout(inputElements.data(), (UINT)inputElements.size(), value.second->GetBlob()->GetBufferPointer(), value.second->GetBlob()->GetBufferSize(), this->m_dxInputLayout.GetAddressOf());
				COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX Input Layout.");
			}

		}

		#pragma endregion

		//==========================��ȡ��ɫ����Դ��Ϣ===========================
		#pragma region ��ȡ��ɫ����Դ��Ϣ

		for (UINT i = 0; i < shaderDescription.BoundResources; i++)
		{
			//��ȡ��ɫ����Դ����Ϣ
			D3D11_SHADER_INPUT_BIND_DESC shaderInputBindDesc;
			hr = shaderReflection->GetResourceBindingDesc(i, &shaderInputBindDesc);

			COM_ERROR_IF_FAILED(hr, "Failed to get description of shader input bind.");

			//�Ĵ�����λ
			UINT slot = shaderInputBindDesc.BindPoint;
			//��Դ��
			std::string resourceName = shaderInputBindDesc.Name;

			//������ʵ����Ѿ���¼����һ��Դ����
			if (shaderParametersMap.count(resourceName) != 0)
			{
				//�����ڵ���Դ������������ɫ����������
				shaderParametersMap[resourceName]->ownerShaderTypesMap[value.first] = true;

				//������һ��Դ�Ĵ���
				continue;
			}

			//������Դ���ͷֱ���д���
			switch (shaderInputBindDesc.Type)
			{
				//============�����Դ����Ϊ Texture============
			case D3D_SIT_TEXTURE:
			{
				//����ShaderParameter
				ShaderParameter* shaderParameter = new ShaderParameter(m_dxDevice, resourceName, ShaderParameter::ShaderParameterType::Texture, slot);

				//���ShaderParameter������ɫ������
				shaderParameter->ownerShaderTypesMap[value.first] = true;

				//���µ� ShaderParameter ���뵽 shaderParametersMap
				shaderParametersMap.insert(std::map<std::string, ShaderParameter*>::value_type(shaderParameter->GetName(), shaderParameter));
			}
			break;

			//============�����Դ���� Sampler============
			case D3D_SIT_SAMPLER:
			{
				//����ShaderParameter
				ShaderParameter* shaderParameter = new ShaderParameter(m_dxDevice, resourceName, ShaderParameter::ShaderParameterType::Sampler, slot);

				//���ShaderParameter������ɫ������
				shaderParameter->ownerShaderTypesMap[value.first] = true;

				//���µ� ShaderParameter ���뵽 shaderParametersMap
				shaderParametersMap.insert(std::map<std::string, ShaderParameter*>::value_type(shaderParameter->GetName(), shaderParameter));
			}
			break;

			//============�����Դ����Ϊ StructuredBuffer============
			case D3D_SIT_STRUCTURED:
			{

			}
			break;

			//============�����Դ����Ϊ ConstantBuffer============
			case D3D_SIT_CBUFFER:
			{
				//������Դ����ȡ��ɫ���ĳ�������
				ID3D11ShaderReflectionConstantBuffer* srConstantBuffer = shaderReflection->GetConstantBufferByName(shaderInputBindDesc.Name);

				//��ȡ������������
				D3D11_SHADER_BUFFER_DESC shaderBufferDesc;
				hr = srConstantBuffer->GetDesc(&shaderBufferDesc);
				COM_ERROR_IF_FAILED(hr, "Failed to GetDesc of pixel shader constant buffer.");

				//����ShaderParameter
				ShaderParameter* shaderParameter = new ShaderParameter(m_dxDevice, resourceName, ShaderParameter::ShaderParameterType::ConstantBuffer, slot, shaderBufferDesc.Size);
				//���ShaderParameter������ɫ������
				shaderParameter->ownerShaderTypesMap[value.first] = true;

				//==========����������������б��������������뵽 ShaderParameter �� ConstantBuffer �� map ��=============
				#pragma region ���������������

				for (UINT j = 0; j < shaderBufferDesc.Variables; ++j)
				{
					//��ȡ��Ӧλ�õı�������ʵ�����Ǳ�����
					ID3D11ShaderReflectionVariable* shaderVariable = srConstantBuffer->GetVariableByIndex(j);

					//��ȡ����������
					D3D11_SHADER_VARIABLE_DESC shaderVariableDesc;
					hr = shaderVariable->GetDesc(&shaderVariableDesc);
					COM_ERROR_IF_FAILED(hr, "Failed to GetDesc of pixel shader constant buffer variable.");

					//��ȡ������������
					ID3D11ShaderReflectionType* shaderReflectionType = shaderVariable->GetType();
					D3D11_SHADER_TYPE_DESC shaderTypeDesc;
					hr = shaderReflectionType->GetDesc(&shaderTypeDesc);
					COM_ERROR_IF_FAILED(hr, "Failed to GetDesc of pixel shader constant buffer variable type.");

					//���ɱ���
					ConstantBufferVariable constantBufferVariable(shaderVariableDesc.Name,
						shaderVariableDesc.StartOffset, shaderVariableDesc.Size, shaderTypeDesc.Class, shaderTypeDesc.Type);

					//���������뵽 shaderParameter��constantBuffer �� map ��
					shaderParameter->constantBuffer->constantVariablesMap.insert(
						std::map<std::string, ConstantBufferVariable>::value_type(constantBufferVariable.name, constantBufferVariable));

				}

				#pragma endregion

				//���µ� ShaderParameter ���뵽 shaderParametersMap
				shaderParametersMap.insert(std::map<std::string, ShaderParameter*>::value_type(shaderParameter->GetName(), shaderParameter));
			}
			break;

			}
		}

	#pragma endregion

	}

	//========================�������-ģ�建��״̬=======================
	#pragma region �������-ģ�建��״̬
	//���ģ�����״̬����
	D3D11_DEPTH_STENCIL_DESC dsc;
	ZeroMemory(&dsc, sizeof(dsc));
	dsc.DepthEnable = true;
	dsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsc.StencilEnable = false;
	//��������������������ģ�建����������д����������������

	hr = m_dxDevice->CreateDepthStencilState(&dsc, m_dxDepthStencilState.GetAddressOf());
	//������
	COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX DepthStencil State.");

	#pragma endregion

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

	hr = m_dxDevice->CreateBlendState(&bd, this->m_dxBlendState.GetAddressOf());
	//������
	COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX Blend State.");

	#pragma endregion 

	return true;
}

std::string Material::GetShaderFilePath()
{
	return this->m_shaderFilePath;
}

Shader* Material::GetShader(Shader::ShaderType shaderType) const
{
	std::map<Shader::ShaderType, Shader*>::const_iterator iter = m_shadersMap.find(shaderType);
	return (iter->second);
}

ID3D11InputLayout* Material::GetInputLayout() const
{
	return this->m_dxInputLayout.Get();
}

ID3D11DepthStencilState* Material::GetDepthStencilState() const
{
	return this->m_dxDepthStencilState.Get();
}

ID3D11BlendState* Material::GetBlendState() const
{
	return this->m_dxBlendState.Get();
}

void Material::BindShaders(ID3D11DeviceContext* deviceContext)
{
	for (std::map<Shader::ShaderType, Shader*>::value_type value : m_shadersMap)
	{
		value.second->Bind(deviceContext);
	}

}




