#pragma once

#include<vector>

#include"Hierarchy.h"
#include"..\Built-in Components\UI_Component.h"
#include"..\Built-in Components\UI_Transform.h"
#include"..\Built-in Components\UI_Attributes.h"

class Inspector 
{

public:
	static int preSelected;
public:
	void Initialize();
	void Render();
	void RefreshUIList();
};