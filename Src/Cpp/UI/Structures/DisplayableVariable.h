#pragma once

#include"..\..\Graphics\Structures\Material.h"


struct DisplayableVariable
{
	//通用成员
	std::string variableName;//变量名
	D3D_SHADER_VARIABLE_TYPE variableType = D3D_SHADER_VARIABLE_TYPE::D3D_SVT_VOID;//变量类型

	//常量缓冲变量类型相关
	ConstantBuffer* ownerConstantBuffer;//对应的常量缓冲指针
	UINT variableSize;//变量字节大小
	bool boolVal = false;
	int intVec[4] = {0};
	float floatVec[4] = {0};

	//纹理类型变量相关成员
	Texture* ownerTexture;//对应的纹理指针
	std::string textureFile;//纹理文件路径


	/// <summary>
	/// 默认构造函数
	/// </summary>
	DisplayableVariable() {}

	/// <summary>
	/// 常量缓冲类型变量构造函数
	/// </summary>
	/// <param name="constantBufferVariable">常量缓冲内变量</param>
	/// <param name="constantBuffer">常量缓冲指针</param>
	DisplayableVariable(ConstantBufferVariable constantBufferVariable, ConstantBuffer* constantBuffer) :
		ownerConstantBuffer(constantBuffer),
		variableSize(constantBufferVariable.size),
		variableName(constantBufferVariable.name),
		variableType(constantBufferVariable.variableType)
	{
		//如果为浮点数
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
	/// 纹理类型变量构造函数
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
	/// 更新 DisplayableVariable 对应的着色器资源参数
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