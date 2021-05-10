#pragma once

#include"..\..\Graphics\Structures\Material.h"


struct DisplayableVariable
{
	//ͨ�ó�Ա
	std::string variableName;//������
	D3D_SHADER_VARIABLE_TYPE variableType = D3D_SHADER_VARIABLE_TYPE::D3D_SVT_VOID;//��������

	//������������������
	ConstantBuffer* ownerConstantBuffer;//��Ӧ�ĳ�������ָ��
	UINT variableSize;//�����ֽڴ�С
	bool boolVal = false;
	int intVec[4] = {0};
	float floatVec[4] = {0};

	//�������ͱ�����س�Ա
	Texture* ownerTexture;//��Ӧ������ָ��
	std::string textureFile;//�����ļ�·��


	/// <summary>
	/// Ĭ�Ϲ��캯��
	/// </summary>
	DisplayableVariable() {}

	/// <summary>
	/// �����������ͱ������캯��
	/// </summary>
	/// <param name="constantBufferVariable">���������ڱ���</param>
	/// <param name="constantBuffer">��������ָ��</param>
	DisplayableVariable(ConstantBufferVariable constantBufferVariable, ConstantBuffer* constantBuffer) :
		ownerConstantBuffer(constantBuffer),
		variableSize(constantBufferVariable.size),
		variableName(constantBufferVariable.name),
		variableType(constantBufferVariable.variableType)
	{
		//���Ϊ������
		if (variableType == D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT)
		{
			ownerConstantBuffer->GetData(floatVec, constantBufferVariable.startOffset, constantBufferVariable.size);
		}
		else if (variableType == D3D_SHADER_VARIABLE_TYPE::D3D_SVT_INT)
		{
			ownerConstantBuffer->GetData(intVec, constantBufferVariable.startOffset, constantBufferVariable.size);
		}
		else if (variableType == D3D_SHADER_VARIABLE_TYPE::D3D_SVT_BOOL)
		{
			ownerConstantBuffer->GetData(&boolVal, constantBufferVariable.startOffset, constantBufferVariable.size);
		}
	}

	/// <summary>
	/// �������ͱ������캯��
	/// </summary>
	/// <param name="textureVariableName"></param>
	/// <param name="texture"></param>
	DisplayableVariable(std::string textureVariableName, Texture* texture) :
		ownerTexture(texture),
		variableName(textureVariableName),
		textureFile(texture->GetTextureFilePath())
	{
		variableType = D3D_SHADER_VARIABLE_TYPE::D3D_SVT_TEXTURE;
	}

	/// <summary>
	/// ���� DisplayableVariable ��Ӧ����ɫ����Դ����
	/// </summary>
	void UpdateOwnerParameterValues()
	{
		switch (variableType)
		{
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT:
		{
			ConstantBufferVariable cbVariable = ownerConstantBuffer->GetConstantBufferVariableByName(variableName);
			ownerConstantBuffer->SetFloatVec(floatVec, variableSize / sizeof(float), cbVariable.startOffset, cbVariable.size);
		}
		break;

		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_INT:
		{
			ConstantBufferVariable cbVariable = ownerConstantBuffer->GetConstantBufferVariableByName(variableName);
			ownerConstantBuffer->SetIntVec(intVec, variableSize / sizeof(float), cbVariable.startOffset, cbVariable.size);
		}
		break;

		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_BOOL:
		{
			ConstantBufferVariable cbVariable = ownerConstantBuffer->GetConstantBufferVariableByName(variableName);
			ownerConstantBuffer->SetBool(boolVal, cbVariable.startOffset);
		}
		break;

		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_TEXTURE:
		{
			if (textureFile != ownerTexture->GetTextureFilePath())
			{
				ownerTexture->Instantiate(textureFile);
			}
		}
		break;

		default:
			break;
		}
	
	}


};