#pragma once

#include<vector>
#include<string>
#include <wrl/client.h>//ComPtr智能指针相关引用 （https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160）

#include"UI_Component.h"
#include"..\..\Graphics\Structures\ConstantBuffer.h"


enum class ImGuiAvailableVariableType
{
	VT_INVALID,
	VT_BOOL,
	VT_INT,
	VT_FLOAT
};

struct ImGuiAvailableVariable
{
	std::string name;
	ImGuiAvailableVariableType type = ImGuiAvailableVariableType::VT_INVALID;
	ConstantBuffer* ownerBuffer;
	UINT size;

	bool boolVal = false;
	int intVec[4] = { 0 };
	float floatVec[4] = { 1,1,1,1 };

	ImGuiAvailableVariable() {}

	ImGuiAvailableVariable(ConstantBufferVariable constantBufferVariable, ConstantBuffer* constantBuffer):
		ownerBuffer(constantBuffer),
		size(constantBufferVariable.size),
		name(constantBufferVariable.name)
	{
		//如果为浮点数
		if (constantBufferVariable.variableType == D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT)
		{
			type = ImGuiAvailableVariableType::VT_FLOAT;
			ownerBuffer->GetData(floatVec, constantBufferVariable.startOffset, constantBufferVariable.size);

		}
		else if (constantBufferVariable.variableType == D3D_SHADER_VARIABLE_TYPE::D3D_SVT_INT)
		{
			type = ImGuiAvailableVariableType::VT_INT;
			ownerBuffer->GetData(intVec, constantBufferVariable.startOffset, constantBufferVariable.size);
		}
		else if (constantBufferVariable.variableType == D3D_SHADER_VARIABLE_TYPE::D3D_SVT_BOOL)
		{
			type = ImGuiAvailableVariableType::VT_BOOL;
			ownerBuffer->GetData(&boolVal, constantBufferVariable.startOffset, constantBufferVariable.size);
		}
	}

};


class UI_Material
{
public:

	//常量缓冲内变量列表
	std::vector<ImGuiAvailableVariable> imGuiAvailableVariables;

	//纹理列表


public:
	void Render();
};

class UI_MaterialManager : public UI_Component
{
public:
	static std::vector<UI_Material> ui_Materials;

public:
	static void Render();
};