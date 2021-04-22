#pragma once

#include<string>
#include"UI_Component.h"

class UI_Attributes : public UI_Component
{
public:
	static std::string attributes_ObjectName;
	static bool attributes_Active;
	static std::string attributes_Tag;

public:
	static void Render();
	static void UpdateValues(std::string objectName, bool active, std::string tag);
};