#pragma once

#include<vector>

#include"Hierarchy.h"
#include"..\Built-in Components\UI_Component.h"
#include"..\Built-in Components\UI_Transform.h"
#include"..\Built-in Components\UI_Attributes.h"
#include"..\Built-in Components\UI_MaterialManager.h"
#include"..\Built-in Components\UI_Camera.h"
#include"..\Built-in Components\UI_Light.h"

#include"..\..\Logic\SceneManager.h"
#include"..\..\Components\Skybox.h"
#include"..\..\Components\Light.h"
#include"..\..\Components\Attributes.h"

class Inspector 
{
public:
	static int preSelected;
	std::vector<std::string> builtInComponentNames = { "Transform","MeshRenderer","MaterialManager" };

public:
	void Render();
	void AddComponentToObject(Object* object, std::string componentName);
};