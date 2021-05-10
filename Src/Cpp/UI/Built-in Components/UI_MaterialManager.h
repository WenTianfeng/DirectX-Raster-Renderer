#pragma once

#include<vector>
#include<string>
#include <wrl/client.h>//ComPtr����ָ��������� ��https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160��

#include"UI_Component.h"
#include"..\Structures\DisplayableVariable.h"


class UI_Material
{

public:
	std::string shaderFilePath;
	Material* ownerMaterial;

	//��ɫ����Դ�����б�
	std::vector<DisplayableVariable> displayableVariables;

public:
	UI_Material();
	UI_Material(Material* material);

	/// <summary>
	/// ����UI��ʼ��
	/// </summary>
	/// <param name="material">��������</param>
	void Initialize(const Material& material);

	/// <summary>
	/// ����UI��Ⱦ
	/// </summary>
	void Render();

};

class UI_MaterialManager : public UI_Component
{
public:
	static std::vector<UI_Material> ui_Materials;//����UI�б�

public:
	static void Render();
};