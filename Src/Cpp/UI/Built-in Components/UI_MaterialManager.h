#pragma once

#include<vector>
#include<string>
#include <wrl/client.h>//ComPtr����ָ��������� ��https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160��

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

	bool boolVal;
	int intVec[4] = { 0 };
	float floatVec[4] = { 0,0,0,0 };

	ImGuiAvailableVariable() {}

	ImGuiAvailableVariable(ConstantBufferVariable constantBufferVariable, ConstantBuffer* constantBuffer):
		ownerBuffer(constantBuffer),
		size(constantBufferVariable.size),
		name(constantBufferVariable.name)

	{
		//���Ϊ������
		if (constantBufferVariable.variableType == D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT)
		{
			type = ImGuiAvailableVariableType::VT_FLOAT;
		}
		else if (constantBufferVariable.variableType == D3D_SHADER_VARIABLE_TYPE::D3D_SVT_INT)
		{
			type = ImGuiAvailableVariableType::VT_INT;
		}
	}

};


class UI_Material
{
public:

	//���������ڱ����б�
	std::vector<ImGuiAvailableVariable> imGuiAvailableVariables;

	//�����б�


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