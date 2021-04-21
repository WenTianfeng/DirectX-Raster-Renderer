#pragma once

#include<vector>

#include"..\Built-in Components\UI_Component.h"
#include"..\Built-in Components\UI_Transform.h"

class Inspector 
{

private:

	std::vector<UI_Component*> m_componentUIs;

public:
	void Initialize();
	void Render();
	void RefreshUIList();

};