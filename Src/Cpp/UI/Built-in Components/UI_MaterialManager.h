#pragma once

#include<vector>
#include<string>
#include <wrl/client.h>//ComPtr智能指针相关引用 （https://docs.microsoft.com/en-us/cpp/cppcx/wrl/microsoft-wrl-namespace?view=msvc-160）

#include"UI_Component.h"
#include"..\Structures\DisplayableVariable.h"


class UI_Material
{

public:
	std::string shaderFilePath;
	Material* ownerMaterial;

	//着色器资源变量列表
	std::vector<DisplayableVariable> displayableVariables;

public:
	UI_Material();
	UI_Material(Material* material);

	/// <summary>
	/// 材质UI初始化
	/// </summary>
	/// <param name="material">材质引用</param>
	void Initialize(const Material& material);

	/// <summary>
	/// 材质UI渲染
	/// </summary>
	void Render();

};

class UI_MaterialManager : public UI_Component
{
public:
	static std::vector<UI_Material> ui_Materials;//材质UI列表

public:
	static void Render();
};