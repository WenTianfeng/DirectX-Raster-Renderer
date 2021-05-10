#include "Material.h"

Material::Material(ID3D11Device* device):
	m_dxDevice(device)
{

}

bool Material::Instantiate(std::string shaderFilePath)
{
	//清空成员
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

	//======================遍历着色器map======================
	for (std::map<Shader::ShaderType, Shader*>::value_type value : m_shadersMap)
	{
		//着色器反射对象
		Microsoft::WRL::ComPtr<ID3D11ShaderReflection> shaderReflection;
		hr = D3DReflect(value.second->GetBlob()->GetBufferPointer(), value.second->GetBlob()->GetBufferSize(),
			__uuidof(ID3D11ShaderReflection), reinterpret_cast<void**>(shaderReflection.GetAddressOf()));

		COM_ERROR_IF_FAILED(hr, "Failed to reflect shader.");

		//着色器描述
		D3D11_SHADER_DESC shaderDescription;
		hr = shaderReflection->GetDesc(&shaderDescription);

		COM_ERROR_IF_FAILED(hr, "Failed to get description of shader.");

		//==========================创建输入布局========================== =
		#pragma region 如果遍历至顶点着色器，则依据反射结果创建输入布局


		if (value.first == Shader::ShaderType::VertexShader)
		{
			//顶点输入结构变量数量
			UINT numInputParameters = shaderDescription.InputParameters;

			//输入布局列表
			std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements;

			//根据着色器定义的顶点输入结构创建输入布局
			for (UINT i = 0; i < numInputParameters; ++i)
			{
				D3D11_INPUT_ELEMENT_DESC inputElement = {};//输入布局列表元素
				D3D11_SIGNATURE_PARAMETER_DESC parameterSignature;//顶点输入结构每个元素的签名

				shaderReflection->GetInputParameterDesc(i, &parameterSignature);

				inputElement.SemanticName = parameterSignature.SemanticName;
				inputElement.SemanticIndex = parameterSignature.SemanticIndex;
				inputElement.InputSlot = 0;
				inputElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
				inputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				inputElement.InstanceDataStepRate = 0;
				inputElement.Format = ShaderProcessor::GetDXGIFormat(parameterSignature);

				//如果格式无效，则停止运行
				assert(inputElement.Format != DXGI_FORMAT_UNKNOWN);

				//将输入布局元素加入列表
				inputElements.push_back(inputElement);
			}

			if (inputElements.size() > 0)
			{
				//使用输入布局列表创建输入布局
				hr = m_dxDevice->CreateInputLayout(inputElements.data(), (UINT)inputElements.size(), value.second->GetBlob()->GetBufferPointer(), value.second->GetBlob()->GetBufferSize(), this->m_dxInputLayout.GetAddressOf());
				COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX Input Layout.");
			}

		}

		#pragma endregion

		//==========================读取着色器资源信息===========================
		#pragma region 读取着色器资源信息

		for (UINT i = 0; i < shaderDescription.BoundResources; i++)
		{
			//获取着色器资源绑定信息
			D3D11_SHADER_INPUT_BIND_DESC shaderInputBindDesc;
			hr = shaderReflection->GetResourceBindingDesc(i, &shaderInputBindDesc);

			COM_ERROR_IF_FAILED(hr, "Failed to get description of shader input bind.");

			//寄存器槽位
			UINT slot = shaderInputBindDesc.BindPoint;
			//资源名
			std::string resourceName = shaderInputBindDesc.Name;

			//如果材质当中已经记录了这一资源参数
			if (shaderParametersMap.count(resourceName) != 0)
			{
				//将存在的资源参数的宿主着色器类型扩增
				shaderParametersMap[resourceName]->ownerShaderTypesMap[value.first] = true;

				//跳过这一资源的处理
				continue;
			}

			//根据资源类型分别进行处理
			switch (shaderInputBindDesc.Type)
			{
				//============如果资源类型为 Texture============
			case D3D_SIT_TEXTURE:
			{
				//创建ShaderParameter
				ShaderParameter* shaderParameter = new ShaderParameter(m_dxDevice, resourceName, ShaderParameter::ShaderParameterType::Texture, slot);

				//添加ShaderParameter所属着色器类型
				shaderParameter->ownerShaderTypesMap[value.first] = true;

				//将新的 ShaderParameter 加入到 shaderParametersMap
				shaderParametersMap.insert(std::map<std::string, ShaderParameter*>::value_type(shaderParameter->GetName(), shaderParameter));
			}
			break;

			//============如果资源类型 Sampler============
			case D3D_SIT_SAMPLER:
			{
				//创建ShaderParameter
				ShaderParameter* shaderParameter = new ShaderParameter(m_dxDevice, resourceName, ShaderParameter::ShaderParameterType::Sampler, slot);

				//添加ShaderParameter所属着色器类型
				shaderParameter->ownerShaderTypesMap[value.first] = true;

				//将新的 ShaderParameter 加入到 shaderParametersMap
				shaderParametersMap.insert(std::map<std::string, ShaderParameter*>::value_type(shaderParameter->GetName(), shaderParameter));
			}
			break;

			//============如果资源类型为 StructuredBuffer============
			case D3D_SIT_STRUCTURED:
			{

			}
			break;

			//============如果资源类型为 ConstantBuffer============
			case D3D_SIT_CBUFFER:
			{
				//根据资源名获取着色器的常量缓冲
				ID3D11ShaderReflectionConstantBuffer* srConstantBuffer = shaderReflection->GetConstantBufferByName(shaderInputBindDesc.Name);

				//获取常量缓冲描述
				D3D11_SHADER_BUFFER_DESC shaderBufferDesc;
				hr = srConstantBuffer->GetDesc(&shaderBufferDesc);
				COM_ERROR_IF_FAILED(hr, "Failed to GetDesc of pixel shader constant buffer.");

				//创建ShaderParameter
				ShaderParameter* shaderParameter = new ShaderParameter(m_dxDevice, resourceName, ShaderParameter::ShaderParameterType::ConstantBuffer, slot, shaderBufferDesc.Size);
				//添加ShaderParameter所属着色器类型
				shaderParameter->ownerShaderTypesMap[value.first] = true;

				//==========遍历常量缓冲的所有变量，将变量加入到 ShaderParameter 的 ConstantBuffer 的 map 中=============
				#pragma region 常量缓冲变量处理

				for (UINT j = 0; j < shaderBufferDesc.Variables; ++j)
				{
					//获取对应位置的变量（其实并不是变量）
					ID3D11ShaderReflectionVariable* shaderVariable = srConstantBuffer->GetVariableByIndex(j);

					//获取变量的描述
					D3D11_SHADER_VARIABLE_DESC shaderVariableDesc;
					hr = shaderVariable->GetDesc(&shaderVariableDesc);
					COM_ERROR_IF_FAILED(hr, "Failed to GetDesc of pixel shader constant buffer variable.");

					//获取变量类型描述
					ID3D11ShaderReflectionType* shaderReflectionType = shaderVariable->GetType();
					D3D11_SHADER_TYPE_DESC shaderTypeDesc;
					hr = shaderReflectionType->GetDesc(&shaderTypeDesc);
					COM_ERROR_IF_FAILED(hr, "Failed to GetDesc of pixel shader constant buffer variable type.");

					//生成变量
					ConstantBufferVariable constantBufferVariable(shaderVariableDesc.Name,
						shaderVariableDesc.StartOffset, shaderVariableDesc.Size, shaderTypeDesc.Class, shaderTypeDesc.Type);

					//将变量加入到 shaderParameter的constantBuffer 的 map 中
					shaderParameter->constantBuffer->constantVariablesMap.insert(
						std::map<std::string, ConstantBufferVariable>::value_type(constantBufferVariable.name, constantBufferVariable));

				}

				#pragma endregion

				//将新的 ShaderParameter 加入到 shaderParametersMap
				shaderParametersMap.insert(std::map<std::string, ShaderParameter*>::value_type(shaderParameter->GetName(), shaderParameter));
			}
			break;

			}
		}

	#pragma endregion

	}

	//========================创建深度-模板缓冲状态=======================
	#pragma region 创建深度-模板缓冲状态
	//深度模板测试状态描述
	D3D11_DEPTH_STENCIL_DESC dsc;
	ZeroMemory(&dsc, sizeof(dsc));
	dsc.DepthEnable = true;
	dsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsc.StencilEnable = false;
	//……………………其余模板缓冲描述待填写……………………

	hr = m_dxDevice->CreateDepthStencilState(&dsc, m_dxDepthStencilState.GetAddressOf());
	//错误检查
	COM_ERROR_IF_FAILED(hr, L"Failed to create DirectX DepthStencil State.");

	#pragma endregion

	//==========================创建混合状态===========================
	#pragma region 创建混合状态
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

	hr = m_dxDevice->CreateBlendState(&bd, this->m_dxBlendState.GetAddressOf());
	//错误检查
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




